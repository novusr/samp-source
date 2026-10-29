#!/usr/bin/env python3
"""Linux native build runner for SA-MP R5 matching using Wine and MSVC 2003 Toolkit."""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent

MSVC_DEFAULT_DIR = Path.home() / ".wine/drive_c/Program Files (x86)/Microsoft Visual C++ Toolkit 2003"


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    h.update(path.read_bytes())
    return h.hexdigest()


def find_toolchain(msvc_dir: Path):
    bin_dir = msvc_dir / "bin"
    if not (bin_dir / "cl.exe").exists():
        raise FileNotFoundError(
            f"MSVC Toolkit 2003 cl.exe not found at {bin_dir / 'cl.exe'}.\n"
            "Ensure Visual C++ Toolkit 2003 is installed in Wine."
        )
    return bin_dir


def build_run(run: str, msvc_dir: Path = MSVC_DEFAULT_DIR, target_units=None):
    if not run or not all(c.isalnum() or c in "_-" for c in run):
        raise ValueError(f"Invalid run identifier: {run}")

    bin_dir = find_toolchain(msvc_dir)
    toolchain_config = json.loads((ROOT / "config/toolchain.json").read_text())
    options = toolchain_config["options"]
    manifest_path = ROOT / "config/manifest.json"
    if manifest_path.exists():
        manifest_data = json.loads(manifest_path.read_text())
        units = sorted({fn["unit"] for fn in manifest_data.get("functions", [])})
    else:
        units = ["pools", "actors", "camera", "game"]

    if target_units:
        units = [u for u in units if u in target_units]
        if not units:
            units = target_units

    out_dir = ROOT / "build" / run
    out_src_dir = out_dir / "src"
    out_src_dir.mkdir(parents=True, exist_ok=True)

    # Optimization: Keep wineserver persistent in background to avoid warm-up cold starts
    try:
        subprocess.run(["wineserver", "-p"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except Exception:
        pass

    wine_env = dict(os.environ)
    wine_env["WINEDEBUG"] = "-all"
    wine_env["WINEDLLOVERRIDES"] = "mscoree,mshtml="
    if "WINEPREFIX" not in wine_env:
        wine_env["WINEPREFIX"] = str(Path.home() / ".wine")

    records = []
    cl_exe = bin_dir / "cl.exe"

    for unit in units:
        src_path = ROOT / "src" / f"{unit}.cpp"
        dest_src = out_src_dir / f"{unit}.cpp"
        dest_obj = out_dir / f"{unit}.obj"

        shutil.copy2(src_path, dest_src)

        wine_src = f"Z:{src_path.resolve()}"
        wine_obj = f"Z:{dest_obj.resolve()}"

        cmd = [
            "wine",
            str(cl_exe),
            *options,
            f"/Fo{wine_obj}",
            wine_src,
        ]

        proc = subprocess.run(
            cmd,
            cwd=str(out_dir),
            env=wine_env,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            errors="replace",
        )
        if proc.returncode != 0:
            print(proc.stdout)
            raise RuntimeError(f"Compilation failed for {unit} (code {proc.returncode})")

        if not dest_obj.exists():
            raise FileNotFoundError(f"Missing output object: {dest_obj}")

        records.append({
            "unit": unit,
            "source_sha256": sha256_file(src_path),
            "object_sha256": sha256_file(dest_obj),
        })
        print(f"Compiled {unit}.cpp -> {dest_obj.name}")

    build_json_path = out_dir / "build.json"
    if target_units and build_json_path.exists():
        try:
            existing_meta = json.loads(build_json_path.read_text(encoding="utf-8"))
            unit_map = {r["unit"]: r for r in existing_meta.get("units", [])}
            for rec in records:
                unit_map[rec["unit"]] = rec
            merged_records = [unit_map[k] for k in sorted(unit_map.keys())]
        except Exception:
            merged_records = records
    else:
        merged_records = records

    build_meta = {
        "run": run,
        "utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        "options": options,
        "toolchain": toolchain_config["tools"],
        "units": merged_records,
    }

    build_json_path.write_text(json.dumps(build_meta, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: compiled {len(units)} units into {out_dir}")
    return out_dir


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", default="baseline", help="Build run identifier (e.g. baseline, repeat)")
    parser.add_argument("--msvc-dir", default=str(MSVC_DEFAULT_DIR), help="Path to MSVC Toolkit 2003 directory")
    parser.add_argument("targets", nargs="*", help="Optional targets to build")
    args, _ = parser.parse_known_args()

    target_units = [Path(t).stem for t in args.targets] if args.targets else None
    build_run(args.run, Path(args.msvc_dir), target_units)


if __name__ == "__main__":
    main()
