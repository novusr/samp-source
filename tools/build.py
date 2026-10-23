#!/usr/bin/env python3
"""SA-MP 0.3.7 R5 Functional Build & Compile-Check Tool (Linux Wine + MSVC 2003).

Compiles individual units, modules, or the full client (`client/saco/`,
`vendor/upstream/raknet/`, `vendor/upstream/saco/d3d9/`) using Microsoft Visual C++
Toolkit 2003 (`cl.exe` 13.10.3077) under Wine, with incremental SHA-256 object caching.

Usage examples:
  python3 tools/build.py --unit net/playerpool
  python3 tools/build.py --module net
  python3 tools/build.py --module game
  python3 tools/build.py --all -j 4
  python3 tools/build.py --status
"""
from __future__ import annotations

import argparse
import concurrent.futures
import dataclasses
import datetime
import hashlib
import json
import os
import re
import subprocess
import sys
import time
from pathlib import Path
from typing import Any

ROOT = Path(__file__).resolve().parent.parent
SACO_DIR = ROOT / "client" / "saco"
UPSTREAM_SACO_DIR = ROOT / "vendor" / "upstream" / "saco"
UPSTREAM_RAKNET_DIR = ROOT / "vendor" / "upstream" / "raknet"
BUILD_DIR = ROOT / "build"
OBJ_DIR = BUILD_DIR / "obj"
SHIM_DIR = BUILD_DIR / "psdk_shim"
STATUS_FILE = BUILD_DIR / "build_status.json"

MSVC_DEFAULT_DIR = Path.home() / ".wine/drive_c/Program Files (x86)/Microsoft Visual C++ Toolkit 2003"
WINE_WINDOWS_INC = Path("/usr/include/wine/windows")

# Per-file optimization overrides from vendor/upstream/saco/saco.vcproj (Release|Win32)
# Default Release uses Optimization="3" (/Ox), while these specific files use Optimization="2" (/O2)
O2_OVERRIDE_UNITS = {
    "game/entity.cpp",
    "game/keystuff.cpp",
    "game/patches.cpp",
    "exceptions.cpp",
}

BASE_DEFINES = [
    "/D", "__i386__",
    "/D", "WIN32",
    "/D", "NDEBUG",
    "/D", "_WINDOWS",
    "/D", "_USRDLL",
    "/D", "SACO_EXPORTS",
    "/D", "SAMPCLI",
]


@dataclasses.dataclass
class BuildUnit:
    name: str          # e.g. 'saco/net/playerpool.cpp' or 'raknet/BitStream.cpp'
    module: str        # 'game', 'net', 'archive', 'd3dhook', 'root', 'd3d9', 'raknet'
    src_path: Path
    obj_name: str


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    return sha256_bytes(path.read_bytes())


def ensure_psdk_shim() -> Path:
    """Create compatibility header shims in `build/psdk_shim/` so Wine's Windows headers
    work cleanly with MSVC 2003 (`cl.exe` 13.10.3077, pre-C99 preprocessor).
    """
    SHIM_DIR.mkdir(parents=True, exist_ok=True)
    stamp = SHIM_DIR / ".shim_v2"
    if stamp.exists():
        return SHIM_DIR

    if WINE_WINDOWS_INC.exists():
        winnt_src = WINE_WINDOWS_INC / "winnt.h"
        if winnt_src.exists():
            txt = winnt_src.read_text(encoding="utf-8", errors="ignore")
            # Replace C99 variadic macros unsupported by MSVC 2003 (13.10)
            txt = re.sub(
                r"#define\s+(__WINE_ALLOC_SIZE|__WINE_DEALLOC)\s*\(\s*\.\.\.\s*\)",
                r"#define \1(x)",
                txt,
            )
            # Suppress C4163/C4615 warnings on Wine's newer intrinsics/pragmas
            pragma_prefix = (
                "#pragma warning(disable: 4163 4615 4002)\n"
            )
            (SHIM_DIR / "winnt.h").write_text(pragma_prefix + txt, encoding="utf-8")

        winbase_src = WINE_WINDOWS_INC / "winbase.h"
        if winbase_src.exists():
            txt = winbase_src.read_text(encoding="utf-8", errors="ignore")
            txt = re.sub(r"__WINE_DEALLOC\s*\([^)]*\)", "", txt)
            txt = re.sub(r"__WINE_ALLOC_SIZE\s*\([^)]*\)", "", txt)
            (SHIM_DIR / "winbase.h").write_text(txt, encoding="utf-8")

        # DXUTgui.h includes <dimm.h> and expects INPUTCONTEXT + IMM API types from imm.h/immdev.h
        dimm_content = (
            "#pragma once\n"
            "#include <imm.h>\n"
            "#include <immdev.h>\n"
        )
        (SHIM_DIR / "dimm.h").write_text(dimm_content, encoding="utf-8")

    # Case-insensitive header forwarding for Linux (#include "../saco/archive/archivefs.h")
    saco_arch = SHIM_DIR / "saco" / "archive"
    saco_arch.mkdir(parents=True, exist_ok=True)
    (saco_arch / "archivefs.h").write_text('#pragma once\n#include "archive/ArchiveFS.h"\n', encoding="utf-8")

    stamp.write_text("ok\n", encoding="utf-8")
    return SHIM_DIR


def discover_units(include_vendor: bool = False) -> list[BuildUnit]:
    """Discover all `.cpp` compilation units in `client/saco/` (and optionally `vendor/`)."""
    units: list[BuildUnit] = []

    for cpp in sorted(SACO_DIR.rglob("*.cpp")):
        if "_reference" in cpp.parts:
            continue
        rel = cpp.relative_to(SACO_DIR)
        rel_str = str(rel).replace("\\", "/")
        parts = rel.parts
        if len(parts) == 1:
            module = "root"
        else:
            module = parts[0]
        obj_name = "saco__" + rel_str.replace("/", "__").replace(".cpp", ".obj")
        units.append(BuildUnit(name=rel_str, module=module, src_path=cpp, obj_name=obj_name))

    if include_vendor:
        d3d9_dir = UPSTREAM_SACO_DIR / "d3d9"
        if d3d9_dir.exists():
            for cpp in sorted(d3d9_dir.rglob("*.cpp")):
                # DXUTsound.cpp is not in saco.vcproj
                if cpp.name == "DXUTsound.cpp":
                    continue
                rel = cpp.relative_to(UPSTREAM_SACO_DIR)
                rel_str = str(rel).replace("\\", "/")
                obj_name = "d3d9__" + rel_str.replace("/", "__").replace(".cpp", ".obj")
                units.append(BuildUnit(name=rel_str, module="d3d9", src_path=cpp, obj_name=obj_name))

        if UPSTREAM_RAKNET_DIR.exists():
            for cpp in sorted(UPSTREAM_RAKNET_DIR.glob("*.cpp")):
                rel_str = f"raknet/{cpp.name}"
                obj_name = "raknet__" + cpp.stem + ".obj"
                units.append(BuildUnit(name=rel_str, module="raknet", src_path=cpp, obj_name=obj_name))

    return units


def get_compile_flags(unit: BuildUnit, msvc_dir: Path) -> list[str]:
    """Build the exact `cl.exe` command-line flags for a unit."""
    if unit.name in O2_OVERRIDE_UNITS:
        opt_flags = ["/O2", "/Ob1", "/Oi", "/Ot", "/Oy", "/MT", "/Zp1", "/EHsc", "/Gy"]
    else:
        opt_flags = ["/Ox", "/Og", "/Ob1", "/Oi", "/Ot", "/Oy", "/MT", "/Zp1", "/EHsc", "/Gy"]

    inc_dirs = [
        SHIM_DIR,
        SHIM_DIR / "saco",
        msvc_dir / "include",
        SACO_DIR,
        UPSTREAM_SACO_DIR,
        UPSTREAM_SACO_DIR / "game",
        UPSTREAM_SACO_DIR / "d3d9" / "include",
        UPSTREAM_SACO_DIR / "d3d9" / "common",
        UPSTREAM_RAKNET_DIR,
        ROOT / "client",
    ]
    psdk_inc = msvc_dir / "PlatformSDK" / "include"
    if psdk_inc.exists():
        inc_dirs.append(psdk_inc)
    if WINE_WINDOWS_INC.exists():
        inc_dirs.append(WINE_WINDOWS_INC)

    inc_flags: list[str] = []
    for d in inc_dirs:
        if d.exists():
            inc_flags.extend(["/I", f"Z:{d.resolve()}"])

    return ["/nologo", "/c", *opt_flags, *inc_flags, *BASE_DEFINES]


def compile_unit(
    unit: BuildUnit,
    msvc_dir: Path,
    force: bool = False,
) -> dict[str, Any]:
    """Compile a single `BuildUnit` via Wine + MSVC 2003."""
    OBJ_DIR.mkdir(parents=True, exist_ok=True)
    obj_path = OBJ_DIR / unit.obj_name
    meta_path = OBJ_DIR / (unit.obj_name + ".json")

    src_hash = sha256_file(unit.src_path)
    flags = get_compile_flags(unit, msvc_dir)
    cache_key = sha256_bytes((src_hash + "|" + " ".join(flags)).encode("utf-8"))

    if not force and obj_path.exists() and meta_path.exists():
        try:
            old_meta = json.loads(meta_path.read_text(encoding="utf-8"))
            if old_meta.get("cache_key") == cache_key and old_meta.get("status") == "OK":
                old_meta["cached"] = True
                return old_meta
        except Exception:
            pass

    if obj_path.exists():
        obj_path.unlink()

    cl_exe = msvc_dir / "bin" / "cl.exe"
    wine_src = f"Z:{unit.src_path.resolve()}"
    wine_obj = f"Z:{obj_path.resolve()}"

    cmd = ["wine", str(cl_exe), *flags, f"/Fo{wine_obj}", wine_src]
    env = os.environ.copy()
    env["WINEDEBUG"] = "-all"

    t0 = time.time()
    proc = subprocess.run(
        cmd,
        cwd=str(BUILD_DIR),
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )
    elapsed = round(time.time() - t0, 2)

    # Clean CR/LF and filter noisy echo line
    raw_lines = [ln.rstrip("\r") for ln in proc.stdout.splitlines()]
    diag_lines = [ln for ln in raw_lines if ln.strip() and ln.strip() != unit.src_path.name]
    diagnostics = "\n".join(diag_lines)

    ok = proc.returncode == 0 and obj_path.exists()
    result = {
        "unit": unit.name,
        "module": unit.module,
        "status": "OK" if ok else "FAIL",
        "returncode": proc.returncode,
        "elapsed_s": elapsed,
        "cached": False,
        "cache_key": cache_key,
        "object": str(obj_path.relative_to(ROOT)) if ok else None,
        "object_size": obj_path.stat().st_size if ok else 0,
        "diagnostics": diagnostics,
    }
    meta_path.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    return result


def resolve_unit_query(query: str, units: list[BuildUnit]) -> list[BuildUnit]:
    """Match a user query like 'net/playerpool', 'playerpool.cpp', or 'client/saco/chatwindow.cpp'."""
    q = query.strip().replace("\\", "/")
    if q.startswith("client/saco/"):
        q = q[len("client/saco/"):]
    if not q.endswith(".cpp"):
        q_cpp = q + ".cpp"
    else:
        q_cpp = q

    exact = [u for u in units if u.name == q_cpp or u.src_path.name == q_cpp]
    if exact:
        return exact
    substr = [u for u in units if q.lower() in u.name.lower()]
    return substr


def print_status_report() -> int:
    """Print cached compilation status across all units without running the compiler."""
    units = discover_units(include_vendor=True)
    by_mod: dict[str, list[tuple[BuildUnit, str, str]]] = {}

    ok_count = 0
    fail_count = 0
    unbuilt_count = 0

    for u in units:
        meta_path = OBJ_DIR / (u.obj_name + ".json")
        status = "UNBUILT"
        first_err = ""
        if meta_path.exists():
            try:
                m = json.loads(meta_path.read_text(encoding="utf-8"))
                status = m.get("status", "UNBUILT")
                for ln in (m.get("diagnostics") or "").splitlines():
                    if "error C" in ln or "fatal error" in ln:
                        first_err = ln.strip()
                        break
            except Exception:
                pass
        if status == "OK":
            ok_count += 1
        elif status == "FAIL":
            fail_count += 1
        else:
            unbuilt_count += 1
        by_mod.setdefault(u.module, []).append((u, status, first_err))

    print(f"=== Build Status ({ok_count} OK, {fail_count} FAIL, {unbuilt_count} UNBUILT / {len(units)} total) ===")
    for mod, items in sorted(by_mod.items()):
        m_ok = sum(1 for _, s, _ in items if s == "OK")
        m_fail = sum(1 for _, s, _ in items if s == "FAIL")
        m_unb = sum(1 for _, s, _ in items if s == "UNBUILT")
        print(f"\n[{mod}] {m_ok}/{len(items)} OK ({m_fail} fail, {m_unb} unbuilt)")
        for u, s, err in items:
            if s != "OK":
                suffix = f" -> {err}" if err else ""
                print(f"  {s:7} {u.name}{suffix}")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--unit", "-u", help="Compile a single unit (e.g. net/playerpool or chatwindow.cpp)")
    parser.add_argument("--module", "-m", help="Compile all units in a module (game, net, archive, d3dhook, root, d3d9, raknet)")
    parser.add_argument("--all", "-a", action="store_true", help="Compile all client/saco units (add --vendor to include d3d9 & raknet)")
    parser.add_argument("--vendor", action="store_true", help="Include vendor/upstream (d3d9 & raknet) when using --all")
    parser.add_argument("--status", "-s", action="store_true", help="Show cached build status table without compiling")
    parser.add_argument("--force", "-f", action="store_true", help="Force recompilation ignoring object cache")
    parser.add_argument("--jobs", "-j", type=int, default=4, help="Parallel Wine compiler workers (default: 4)")
    parser.add_argument("--msvc-dir", default=str(MSVC_DEFAULT_DIR), help="Path to MSVC 2003 Toolkit directory")
    args = parser.parse_args()

    if args.status:
        return print_status_report()

    msvc_dir = Path(args.msvc_dir)
    if not (msvc_dir / "bin" / "cl.exe").exists():
        print(f"ERROR: cl.exe not found at {msvc_dir / 'bin' / 'cl.exe'}", file=sys.stderr)
        return 1

    ensure_psdk_shim()
    all_units = discover_units(include_vendor=True)

    target_units: list[BuildUnit] = []
    if args.unit:
        target_units = resolve_unit_query(args.unit, all_units)
        if not target_units:
            print(f"ERROR: No unit matching '{args.unit}'", file=sys.stderr)
            return 1
    elif args.module:
        target_units = [u for u in all_units if u.module == args.module]
        if not target_units:
            mods = sorted({u.module for u in all_units})
            print(f"ERROR: Unknown module '{args.module}'. Available: {', '.join(mods)}", file=sys.stderr)
            return 1
    elif args.all:
        target_units = discover_units(include_vendor=args.vendor)
    else:
        parser.print_help()
        return 0

    print(f"Compiling {len(target_units)} unit(s) with {args.jobs} worker(s)...")
    t0 = time.time()
    results: list[dict[str, Any]] = []

    if len(target_units) == 1 or args.jobs <= 1:
        for u in target_units:
            res = compile_unit(u, msvc_dir, force=args.force)
            results.append(res)
            tag = "CACHED" if res["cached"] else res["status"]
            print(f"[{tag:6}] {u.name} ({res['elapsed_s']}s)")
            if res["status"] != "OK" and res["diagnostics"]:
                print(res["diagnostics"])
    else:
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
            fut_map = {pool.submit(compile_unit, u, msvc_dir, args.force): u for u in target_units}
            for fut in concurrent.futures.as_completed(fut_map):
                u = fut_map[fut]
                res = fut.result()
                results.append(res)
                tag = "CACHED" if res["cached"] else res["status"]
                print(f"[{tag:6}] {u.name} ({res['elapsed_s']}s)")
                if res["status"] != "OK" and len(target_units) <= 15 and res["diagnostics"]:
                    for ln in res["diagnostics"].splitlines()[:8]:
                        print(f"    {ln}")

    results.sort(key=lambda r: r["unit"])
    ok_units = [r for r in results if r["status"] == "OK"]
    fail_units = [r for r in results if r["status"] != "OK"]
    total_time = round(time.time() - t0, 2)

    STATUS_FILE.write_text(
        json.dumps(
            {
                "updated_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
                "total": len(results),
                "ok": len(ok_units),
                "fail": len(fail_units),
                "elapsed_s": total_time,
                "results": results,
            },
            indent=2,
        )
        + "\n",
        encoding="utf-8",
    )

    print(f"\nSummary: {len(ok_units)}/{len(results)} OK, {len(fail_units)} FAIL in {total_time}s")
    return 0 if not fail_units else 1


if __name__ == "__main__":
    sys.exit(main())
