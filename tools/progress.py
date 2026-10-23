#!/usr/bin/env python3
"""SA-MP 0.3.7 R5 1:1 Function Coverage, Auto-Annotation & Markdown Progress Generator.

Tracks progress of reconstructing all functions in `ghidra_export/_index.json`
(and symbol mappings in `ghidra_export/address_map.json`) into `client/saco/` and
`vendor/upstream/`.

Usage examples:
  python3 tools/progress.py                  # Full summary dashboard + per-class breakdown
  python3 tools/progress.py --markdown       # Generate docs/PROGRESS.md report up to 100%
  python3 tools/progress.py --class CNetGame # Show all R5 functions in a class
  python3 tools/progress.py --next           # Recommend next functions/classes to work on
  python3 tools/progress.py --annotate       # Preview automatic '// R5: 0x...' annotations
  python3 tools/progress.py --annotate --apply
"""
from __future__ import annotations

import argparse
import datetime
import json
import sys
from collections import defaultdict
from pathlib import Path

from r5_common import (
    CLASS_TO_FILE,
    REF_DIR,
    ROOT,
    SACO_DIR,
    UPSTREAM_RAKNET_DIR,
    UPSTREAM_SACO_DIR,
    R5FunctionInfo,
    SourceFunctionDef,
    load_r5_database,
    normalize_class,
    scan_tree_functions,
)

PROGRESS_MD_PATH = ROOT / "docs" / "PROGRESS.md"
OBJ_DIR = ROOT / "build" / "obj"
INITIAL_REF_FILES = 516


def make_bar(pct: float, width: int = 20) -> str:
    filled = int(round((pct / 100.0) * width))
    filled = max(0, min(width, filled))
    return "█" * filled + "░" * (width - filled)


def classify_module(target_file: str) -> str:
    if not target_file:
        return "other"
    if target_file.startswith("game/"):
        return "game"
    if target_file.startswith("net/"):
        return "net"
    if target_file.startswith("archive/"):
        return "archive"
    if target_file.startswith("d3dhook/"):
        return "d3dhook"
    if target_file.startswith("d3d9/"):
        return "d3d9 (DXUT)"
    if "raknet/" in target_file:
        return "raknet"
    return "root (UI/Core)"


def is_compiler_crt_or_template(info: R5FunctionInfo) -> bool:
    """Return True if an R5 entry is an MSVC CRT function, SEH unwind stub, or C++ template instantiation."""
    addr_val = int(info.address, 16)
    blob = f"{info.symbol} {info.name} {info.class_name} {info.method_name}"
    if any(
        tok in blob
        for tok in (
            "Catch@",
            "Unwind@",
            "__ehhandler",
            "SEH_",
            "?$",
            "std::",
            "@std@@",
            "DataStructures@@",
            "CGrowableArray",
            "ttmath",
        )
    ):
        return True
    # Static CRT (libcmt.lib) and SEH table region at the tail of samp.dll (0x100C3000+)
    if addr_val >= 0x100C3000 and (info.name.startswith("FUN_") or info.name.startswith("_")):
        return True
    return False


def compute_coverage():
    db = load_r5_database()
    main_defs = scan_tree_functions(SACO_DIR, exclude_dirs={"_reference", "d3d9"})
    ref_defs = scan_tree_functions(REF_DIR) if REF_DIR.exists() else []

    vendor_defs: list[SourceFunctionDef] = []
    d3d9_dir = UPSTREAM_SACO_DIR / "d3d9"
    if d3d9_dir.exists():
        vendor_defs.extend(scan_tree_functions(d3d9_dir))
    if UPSTREAM_RAKNET_DIR.exists():
        vendor_defs.extend(scan_tree_functions(UPSTREAM_RAKNET_DIR))

    r5_by_qual: dict[tuple[str, str], list[R5FunctionInfo]] = defaultdict(list)
    for info in db.values():
        if info.method_name and not info.method_name.startswith("FUN_"):
            r5_by_qual[(info.class_name, info.method_name)].append(info)

    main_by_addr: dict[str, SourceFunctionDef] = {}
    main_by_qual: dict[tuple[str, str], list[SourceFunctionDef]] = defaultdict(list)
    for d in main_defs:
        if d.r5_address:
            main_by_addr[d.r5_address] = d
        main_by_qual[(d.class_name, d.method_name)].append(d)

    vendor_by_addr: dict[str, SourceFunctionDef] = {}
    vendor_by_qual: dict[tuple[str, str], list[SourceFunctionDef]] = defaultdict(list)
    for d in vendor_defs:
        if d.r5_address:
            vendor_by_addr[d.r5_address] = d
        vendor_by_qual[(d.class_name, d.method_name)].append(d)

    ref_by_addr: dict[str, list[SourceFunctionDef]] = defaultdict(list)
    ref_by_qual: dict[tuple[str, str], list[SourceFunctionDef]] = defaultdict(list)
    for d in ref_defs:
        if d.r5_address:
            ref_by_addr[d.r5_address].append(d)
        ref_by_qual[(d.class_name, d.method_name)].append(d)

    status_by_addr: dict[str, str] = {}
    loc_by_addr: dict[str, str] = {}

    for addr, info in db.items():
        key = (info.class_name, info.method_name)
        if addr in main_by_addr:
            status_by_addr[addr] = "ANNOTATED"
            loc_by_addr[addr] = f"{main_by_addr[addr].rel_path}:{main_by_addr[addr].line_number}"
        elif info.method_name and key in main_by_qual:
            status_by_addr[addr] = "IN_SOURCE"
            d0 = main_by_qual[key][0]
            loc_by_addr[addr] = f"{d0.rel_path}:{d0.line_number}"
        elif addr in vendor_by_addr or (info.method_name and key in vendor_by_qual):
            status_by_addr[addr] = "IN_VENDOR"
            d0 = vendor_by_addr.get(addr) or vendor_by_qual[key][0]
            loc_by_addr[addr] = f"{d0.rel_path}:{d0.line_number}"
        elif is_compiler_crt_or_template(info):
            status_by_addr[addr] = "CRT_OR_TEMPLATE"
            loc_by_addr[addr] = "MSVC CRT / STL / Template"
        elif addr in ref_by_addr or (info.method_name and key in ref_by_qual):
            status_by_addr[addr] = "IN_REFERENCE"
            rdefs = ref_by_addr.get(addr) or ref_by_qual.get(key, [])
            loc_by_addr[addr] = f"{rdefs[0].rel_path}:{rdefs[0].line_number}" if rdefs else "_reference"
        elif info.method_name and not info.method_name.startswith("FUN_"):
            status_by_addr[addr] = "NAMED_TODO"
            loc_by_addr[addr] = CLASS_TO_FILE.get(info.class_name, "")
        else:
            status_by_addr[addr] = "UNNAMED_TODO"
            loc_by_addr[addr] = ""

    return db, main_defs, vendor_defs, ref_defs, r5_by_qual, status_by_addr, loc_by_addr


def get_build_stats() -> tuple[int, int, int, list[tuple[str, str, str]]]:
    """Read cached object build statuses from `build/obj/*.json`."""
    from build import discover_units
    units = discover_units(include_vendor=True)
    ok_cnt = 0
    fail_cnt = 0
    unbuilt_cnt = 0
    details: list[tuple[str, str, str]] = []
    for u in units:
        meta_path = OBJ_DIR / (u.obj_name + ".json")
        st = "UNBUILT"
        if meta_path.exists():
            try:
                m = json.loads(meta_path.read_text(encoding="utf-8"))
                st = m.get("status", "UNBUILT")
            except Exception:
                pass
        if st == "OK":
            ok_cnt += 1
        elif st == "FAIL":
            fail_cnt += 1
        else:
            unbuilt_cnt += 1
        details.append((u.module, u.name, st))
    return ok_cnt, fail_cnt, unbuilt_cnt, details


def generate_markdown_report(
    db,
    main_defs,
    vendor_defs,
    ref_defs,
    status_by_addr,
    loc_by_addr,
    out_path: Path = PROGRESS_MD_PATH,
) -> int:
    idx_addrs = [a for a, info in db.items() if info.in_index]
    counts = defaultdict(int)
    idx_counts = defaultdict(int)
    for a, st in status_by_addr.items():
        counts[st] += 1
        if db[a].in_index:
            idx_counts[st] += 1

    total_idx = len(idx_addrs)
    active_code_idx = idx_counts["ANNOTATED"] + idx_counts["IN_SOURCE"] + idx_counts["IN_VENDOR"]
    auto_crt_idx = idx_counts["CRT_OR_TEMPLATE"]
    total_resolved_idx = active_code_idx + auto_crt_idx

    # Group by class
    by_class: dict[str, list[str]] = defaultdict(list)
    for a, info in db.items():
        if info.class_name and status_by_addr[a] != "CRT_OR_TEMPLATE":
            by_class[info.class_name].append(a)

    mod_stats: dict[str, dict[str, int]] = defaultdict(lambda: {"done": 0, "ref": 0, "todo": 0, "total": 0})
    class_rows = []
    known_mod_done = 0
    known_mod_total = 0

    for cls, addrs in by_class.items():
        target = CLASS_TO_FILE.get(cls, "")
        if not target and len(addrs) < 3:
            continue
        c_ann = sum(1 for a in addrs if status_by_addr[a] == "ANNOTATED")
        c_src = sum(1 for a in addrs if status_by_addr[a] in {"IN_SOURCE", "IN_VENDOR"})
        c_ref = sum(1 for a in addrs if status_by_addr[a] == "IN_REFERENCE")
        c_done = c_ann + c_src
        c_todo = len(addrs) - c_done - c_ref
        pct_c = (c_done / len(addrs) * 100.0) if addrs else 0.0
        mod = classify_module(target)
        mod_stats[mod]["done"] += c_done
        mod_stats[mod]["ref"] += c_ref
        mod_stats[mod]["todo"] += c_todo
        mod_stats[mod]["total"] += len(addrs)
        if mod != "other":
            known_mod_done += c_done
            known_mod_total += len(addrs)
        class_rows.append((mod, cls, c_ann, c_src, c_ref, c_todo, len(addrs), pct_c, target))

    # Calculate key percentages
    pct_modules = (known_mod_done / known_mod_total * 100.0) if known_mod_total else 0.0
    pct_binary_resolved = (total_resolved_idx / total_idx * 100.0) if total_idx else 0.0

    ref_files_remaining = len(list(REF_DIR.rglob("*.cpp"))) if REF_DIR.exists() else 0
    ref_files_merged = max(0, INITIAL_REF_FILES - ref_files_remaining)
    pct_ref_merged = (ref_files_merged / INITIAL_REF_FILES * 100.0) if INITIAL_REF_FILES else 100.0

    ok_u, fail_u, unb_u, build_details = get_build_stats()
    tot_u = ok_u + fail_u + unb_u
    pct_build = (ok_u / tot_u * 100.0) if tot_u else 0.0

    # Overall Project Progress (0% - 100%):
    # Weighted across the 3 core completion pillars:
    # 50% R5 Client Module/Class Reconstruction + 30% Binary Function Resolution + 20% Build Compilation
    overall_pct = (0.50 * pct_modules) + (0.30 * pct_binary_resolved) + (0.20 * pct_build)

    now_str = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M UTC")

    lines: list[str] = [
        "# Status & Progress Rekonstruksi SA-MP 0.3.7 R5",
        "",
        f"> Di-generate otomatis oleh `python3 tools/progress.py --markdown` pada **{now_str}**.",
        "",
        "## 🎯 PROGRES KESELURUHAN PROYEK (MENUJU 100%)",
        "",
        f"### `{make_bar(overall_pct, 30)}` **{overall_pct:.1f}% / 100.0%**",
        "",
        "| Pilar Penyelesaian Proyek | Progress Bar | Capaian Saat Ini | Persentase | Target 100% |",
        "| :--- | :--- | ---: | ---: | :--- |",
        f"| **1. Rekonstruksi Class & Modul Utama (`saco` + `raknet` + `d3d9`)** | `{make_bar(pct_modules)}` | **{known_mod_done} / {known_mod_total} fungsi** | **{pct_modules:.1f}%** | Seluruh method class terimplementasi |",
        f"| **2. Cakupan Seluruh Fungsi Binary R5 (`_index.json` 5.265 fungsi)** | `{make_bar(pct_binary_resolved)}` | **{total_resolved_idx} / {total_idx} fungsi** | **{pct_binary_resolved:.1f}%** | Seluruh fungsi R5 (termasuk helper/`FUN_`) terpetakan |",
        f"| **3. Kompilasi Unit MSVC 2003 (`tools/build.py`)** | `{make_bar(pct_build)}` | **{ok_u} / {tot_u} unit `.cpp`** | **{pct_build:.1f}%** | 151/151 unit `.cpp` lolos compile & link ke `samp.dll` |",
        f"| **4. Pembersihan Fragmen `_reference/`** | `{make_bar(pct_ref_merged)}` | **{ref_files_merged} / {INITIAL_REF_FILES} file selesai** | **{pct_ref_merged:.1f}%** | 0 file tersisa di `_reference/` (tersisa {ref_files_remaining} file) |",
        "",
        "### Rincian 5.265 Fungsi di Binary `samp.dll` R5 (`_index.json`)",
        "",
        "| Kategori | Keterangan | Jumlah (`_index.json`) | Persentase dari 5.265 |",
        "| :--- | :--- | ---: | ---: |",
        f"| ✅ `ANNOTATED` | Fungsi di `client/saco/` dengan anotasi `// R5: 0x...` | **{idx_counts['ANNOTATED']}** | {idx_counts['ANNOTATED']/total_idx*100:.1f}% |",
        f"| ✅ `IN_SOURCE` | Fungsi di `client/saco/` (cocok `Class::Method`) | **{idx_counts['IN_SOURCE']}** | {idx_counts['IN_SOURCE']/total_idx*100:.1f}% |",
        f"| ✅ `IN_VENDOR` | Fungsi di `vendor/upstream/` (DXUT & RakNet) | **{idx_counts['IN_VENDOR']}** | {idx_counts['IN_VENDOR']/total_idx*100:.1f}% |",
        f"| ⚙️ `CRT_OR_TEMPLATE` | Otomatis dari Compiler MSVC 2003 (`libcmt.lib`, SEH, STL/Template) | **{idx_counts['CRT_OR_TEMPLATE']}** | {idx_counts['CRT_OR_TEMPLATE']/total_idx*100:.1f}% |",
        f"| 📦 `IN_REFERENCE` | Sudah ada di `client/saco/_reference/`, tinggal digabung | **{idx_counts['IN_REFERENCE']}** | {idx_counts['IN_REFERENCE']/total_idx*100:.1f}% |",
        f"| ⏳ `NAMED_TODO` | Fungsi bernama di Ghidra yang belum masuk source utama | **{idx_counts['NAMED_TODO']}** | {idx_counts['NAMED_TODO']/total_idx*100:.1f}% |",
        f"| ⏳ `UNNAMED_TODO` | Fungsi `FUN_10xxxxxx` belum bernama di Ghidra | **{idx_counts['UNNAMED_TODO']}** | {idx_counts['UNNAMED_TODO']/total_idx*100:.1f}% |",
        f"| **TOTAL BINARY R5** | **Seluruh fungsi di `samp.dll` 0.3.7 R5** | **{total_idx}** | **100.0%** |",
        "",
        "---",
        "",
        "## 2. Progress per Subsystem / Modul",
        "",
        "| Modul | Progress | Selesai (`Done`) | Di `_reference/` | Belum (`Todo`) | Total Fungsi Class |",
        "| :--- | :--- | ---: | ---: | ---: | ---: |",
    ]

    mod_order = ["game", "net", "root (UI/Core)", "archive", "d3dhook", "d3d9 (DXUT)", "raknet", "other"]
    for mod in mod_order:
        if mod not in mod_stats:
            continue
        st = mod_stats[mod]
        tot = st["total"]
        pct_m = (st["done"] / tot * 100.0) if tot else 0.0
        lines.append(
            f"| **{mod}** | `{make_bar(pct_m, 15)}` **{pct_m:5.1f}%** | {st['done']} | {st['ref']} | {st['todo']} | {tot} |"
        )
    lines.append(
        f"| **TOTAL MODUL UTAMA** | `{make_bar(pct_modules, 15)}` **{pct_modules:5.1f}%** | **{known_mod_done}** | - | **{known_mod_total - known_mod_done}** | **{known_mod_total}** |"
    )

    lines.extend([
        "",
        "---",
        "",
        "## 3. Progress Lengkap per Class (Menuju 100%)",
        "",
        "| Status | Modul | Class | Progress | Done / Total | Annot | InSrc | InRef | Todo | File Target |",
        "| :---: | :--- | :--- | :--- | ---: | ---: | ---: | ---: | ---: | :--- |",
    ])

    class_rows.sort(key=lambda r: (mod_order.index(r[0]) if r[0] in mod_order else 99, -r[7], r[1]))
    for mod, cls, c_ann, c_src, c_ref, c_todo, tot, pct_c, target in class_rows:
        done = c_ann + c_src
        badge = "✅" if done == tot else ("🔄" if done > 0 or c_ref > 0 else "⏳")
        t_disp = f"`{target}`" if target else "-"
        lines.append(
            f"| {badge} | {mod} | `{cls[:34]}` | `{make_bar(pct_c, 10)}` {pct_c:5.1f}% | **{done}/{tot}** | {c_ann} | {c_src} | {c_ref} | {c_todo} | {t_disp} |"
        )

    lines.extend([
        "",
        "---",
        "",
        "## 4. Cara Memperbarui Dokumen Ini",
        "",
        "Jalankan perintah berikut kapan saja untuk memperbarui anotasi `// R5:` dan me-regenerate laporan ini:",
        "",
        "```bash",
        "python3 tools/progress.py --annotate --apply",
        "python3 tools/progress.py --markdown",
        "```",
        "",
    ])

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"Generated progress documentation at {out_path.relative_to(ROOT)}")
    print(f"  OVERALL PROJECT PROGRESS   : {overall_pct:.1f}% / 100.0%")
    print(f"  1. Main Modules & Classes  : {known_mod_done}/{known_mod_total} ({pct_modules:.1f}%)")
    print(f"  2. Binary Functions (5265) : {total_resolved_idx}/{total_idx} ({pct_binary_resolved:.1f}%)")
    print(f"  3. Build Units Passing     : {ok_u}/{tot_u} ({pct_build:.1f}%)")
    print(f"  4. Reference Files Merged  : {ref_files_merged}/{INITIAL_REF_FILES} ({pct_ref_merged:.1f}%)")
    return 0


def cmd_summary(db, main_defs, vendor_defs, ref_defs, status_by_addr, loc_by_addr) -> int:
    idx_addrs = [a for a, info in db.items() if info.in_index]
    counts = defaultdict(int)
    idx_counts = defaultdict(int)
    for a, st in status_by_addr.items():
        counts[st] += 1
        if db[a].in_index:
            idx_counts[st] += 1

    total_idx = len(idx_addrs)
    active_code_idx = idx_counts["ANNOTATED"] + idx_counts["IN_SOURCE"] + idx_counts["IN_VENDOR"]
    total_resolved_idx = active_code_idx + idx_counts["CRT_OR_TEMPLATE"]
    pct_resolved = (total_resolved_idx / total_idx * 100.0) if total_idx else 0.0

    by_class: dict[str, list[str]] = defaultdict(list)
    for a, info in db.items():
        if info.class_name and status_by_addr[a] != "CRT_OR_TEMPLATE":
            by_class[info.class_name].append(a)

    known_mod_done = 0
    known_mod_total = 0
    for cls, addrs in by_class.items():
        target = CLASS_TO_FILE.get(cls, "")
        if target:
            c_done = sum(1 for a in addrs if status_by_addr[a] in {"ANNOTATED", "IN_SOURCE", "IN_VENDOR"})
            known_mod_done += c_done
            known_mod_total += len(addrs)

    pct_modules = (known_mod_done / known_mod_total * 100.0) if known_mod_total else 0.0
    ok_u, fail_u, unb_u, _ = get_build_stats()
    tot_u = ok_u + fail_u + unb_u
    pct_build = (ok_u / tot_u * 100.0) if tot_u else 0.0
    overall_pct = (0.50 * pct_modules) + (0.30 * pct_resolved) + (0.20 * pct_build)

    print("========================================================================================")
    print(f"  OVERALL PROJECT PROGRESS:  [{make_bar(overall_pct, 25)}]  {overall_pct:.1f}% / 100.0%")
    print("========================================================================================")
    print(f"  1. Main Modules & Classes : {known_mod_done:4d} / {known_mod_total:4d} functions ({pct_modules:.1f}%)")
    print(f"  2. Binary R5 Resolution   : {total_resolved_idx:4d} / {total_idx:4d} functions ({pct_resolved:.1f}%)")
    print(f"  3. MSVC 2003 Build Units  : {ok_u:4d} / {tot_u:4d} units OK  ({pct_build:.1f}%)")
    print("----------------------------------------------------------------------------------------")
    print(f"  [ANNOTATED]       Explicit '// R5: 0x...' in client/saco/ : {idx_counts['ANNOTATED']:4d}  (all: {counts['ANNOTATED']:4d})")
    print(f"  [IN_SOURCE]       Matched by Class::Method in client/saco/: {idx_counts['IN_SOURCE']:4d}  (all: {counts['IN_SOURCE']:4d})")
    print(f"  [IN_VENDOR]       Present in vendor/upstream (DXUT/RakNet): {idx_counts['IN_VENDOR']:4d}  (all: {counts['IN_VENDOR']:4d})")
    print(f"  [CRT_OR_TEMPLATE] Compiler CRT / SEH / STL Templates      : {idx_counts['CRT_OR_TEMPLATE']:4d}  (all: {counts['CRT_OR_TEMPLATE']:4d})")
    print(f"  [IN_REFERENCE]    Available in client/saco/_reference/    : {idx_counts['IN_REFERENCE']:4d}  (all: {counts['IN_REFERENCE']:4d})")
    print(f"  [NAMED_TODO]      Named in Ghidra, missing in source      : {idx_counts['NAMED_TODO']:4d}  (all: {counts['NAMED_TODO']:4d})")
    print(f"  [UNNAMED_TODO]    Unnamed FUN_10xxxxxx in Ghidra          : {idx_counts['UNNAMED_TODO']:4d}  (all: {counts['UNNAMED_TODO']:4d})")
    print()

    print(f"{'Class Name':30} {'Annot':>6} {'InSrc':>6} {'InRef':>6} {'Todo':>6} {'Total':>6}  {'Target File'}")
    print("-" * 88)
    rows = []
    for cls, addrs in by_class.items():
        c_ann = sum(1 for a in addrs if status_by_addr[a] == "ANNOTATED")
        c_src = sum(1 for a in addrs if status_by_addr[a] in {"IN_SOURCE", "IN_VENDOR"})
        c_ref = sum(1 for a in addrs if status_by_addr[a] == "IN_REFERENCE")
        c_todo = len(addrs) - c_ann - c_src - c_ref
        target = CLASS_TO_FILE.get(cls, "")
        if target or len(addrs) >= 5:
            rows.append((cls, c_ann, c_src, c_ref, c_todo, len(addrs), target))

    rows.sort(key=lambda r: (0 if r[6] else 1, -(r[1] + r[2] + r[3]), r[0]))
    for cls, c_ann, c_src, c_ref, c_todo, tot, target in rows[:45]:
        print(f"{cls[:30]:30} {c_ann:6d} {c_src:6d} {c_ref:6d} {c_todo:6d} {tot:6d}  {target}")
    if len(rows) > 45:
        print(f"... ({len(rows) - 45} more classes; use --class <Name> or --markdown for full table)")
    return 0


def cmd_class(cls_query: str, db, status_by_addr, loc_by_addr) -> int:
    norm_q = normalize_class(cls_query)
    matches = [
        info for info in db.values()
        if info.class_name.lower() == norm_q.lower() or cls_query.lower() in info.class_name.lower()
    ]
    if not matches:
        print(f"No R5 functions found for class '{cls_query}'.", file=sys.stderr)
        return 1

    matches.sort(key=lambda x: x.address)
    print(f"=== R5 Functions for '{cls_query}' ({len(matches)} functions) ===")
    for info in matches:
        st = status_by_addr[info.address]
        loc = loc_by_addr[info.address]
        loc_str = f" -> {loc}" if loc else ""
        print(f"  0x{info.address}  [{st:15}]  {info.qualified_name:42}{loc_str}")
    return 0


def cmd_next(limit: int, db, status_by_addr, loc_by_addr) -> int:
    print(f"=== Top {limit} Recommended Next Functions to Merge or Reconstruct ===")
    print("\n1. Ready in `_reference/` (can be merged directly into `client/saco/`):")
    ref_items = [
        info for a, info in db.items()
        if status_by_addr[a] == "IN_REFERENCE" and (info.class_name in CLASS_TO_FILE or not info.class_name)
    ]
    ref_items.sort(key=lambda x: (0 if x.class_name in CLASS_TO_FILE else 1, x.class_name, x.address))
    for info in ref_items[:limit]:
        target = CLASS_TO_FILE.get(info.class_name, "client/saco/")
        print(f"  0x{info.address}  {info.qualified_name:38}  from {loc_by_addr[info.address]} -> {target}")

    print("\n2. Named R5 functions missing from `client/saco/` (reconstruct via `tools/decompile.py`):")
    todo_items = [
        info for a, info in db.items()
        if status_by_addr[a] == "NAMED_TODO" and info.class_name in CLASS_TO_FILE
    ]
    todo_items.sort(key=lambda x: (x.class_name, x.address))
    for info in todo_items[:limit]:
        target = CLASS_TO_FILE.get(info.class_name, "")
        print(f"  0x{info.address}  {info.qualified_name:38}  target: client/saco/{target}")
    return 0


def cmd_annotate(apply: bool, db, main_defs, r5_by_qual) -> int:
    """Find unannotated functions in `client/saco/` that uniquely map to an R5 address."""
    by_file: dict[Path, list[tuple[SourceFunctionDef, str]]] = defaultdict(list)

    for d in main_defs:
        if d.r5_address:
            continue
        key = (d.class_name, d.method_name)
        r5_candidates = r5_by_qual.get(key, [])
        same_qual_in_file = [
            x for x in main_defs
            if x.file_path == d.file_path and x.class_name == d.class_name and x.method_name == d.method_name
        ]
        if len(r5_candidates) == 1 and len(same_qual_in_file) == 1:
            by_file[d.file_path].append((d, r5_candidates[0].address))

    total_matches = sum(len(v) for v in by_file.values())
    print(f"Found {total_matches} unambiguous unannotated functions across {len(by_file)} file(s) in client/saco/.")

    for fpath, items in sorted(by_file.items()):
        rel = fpath.relative_to(ROOT)
        print(f"\n  {rel} ({len(items)} function(s)):")
        for d, addr in items:
            print(f"    line {d.line_number:4d}: // R5: 0x{addr.upper()}  ({d.qualified_name})")

        if apply:
            raw = fpath.read_bytes()
            try:
                text = raw.decode("utf-8")
                enc = "utf-8"
            except UnicodeDecodeError:
                text = raw.decode("latin1")
                enc = "latin1"

            nl = "\r\n" if "\r\n" in text else "\n"
            lines = text.splitlines()
            for d, addr in sorted(items, key=lambda x: x[0].line_number, reverse=True):
                idx = d.line_number - 1
                lines.insert(idx, f"// R5: 0x{addr.upper()}")
            fpath.write_bytes((nl.join(lines) + nl).encode(enc))

    if apply and total_matches > 0:
        print(f"\nApplied {total_matches} '// R5: 0x...' annotations to {len(by_file)} file(s).")
    elif not apply and total_matches > 0:
        print("\nRun with `--annotate --apply` to write these annotations into client/saco/.")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--markdown", "-md", action="store_true", help="Generate docs/PROGRESS.md full progress documentation")
    parser.add_argument("--output", "-o", default=str(PROGRESS_MD_PATH), help="Output path for --markdown (default: docs/PROGRESS.md)")
    parser.add_argument("--class", dest="cls", help="Inspect R5 coverage for a specific class (e.g. CPlayerPool)")
    parser.add_argument("--next", action="store_true", help="Recommend next functions to merge/reconstruct")
    parser.add_argument("--limit", type=int, default=20, help="Max items for --next (default: 20)")
    parser.add_argument("--annotate", action="store_true", help="Find unannotated functions in client/saco/ with unique R5 addresses")
    parser.add_argument("--apply", action="store_true", help="Write '// R5: 0x...' comments when used with --annotate")
    args = parser.parse_args()

    db, main_defs, vendor_defs, ref_defs, r5_by_qual, status_by_addr, loc_by_addr = compute_coverage()

    if args.markdown:
        return generate_markdown_report(
            db, main_defs, vendor_defs, ref_defs, status_by_addr, loc_by_addr, Path(args.output)
        )
    if args.annotate:
        return cmd_annotate(args.apply, db, main_defs, r5_by_qual)
    if args.cls:
        return cmd_class(args.cls, db, status_by_addr, loc_by_addr)
    if args.next:
        return cmd_next(args.limit, db, status_by_addr, loc_by_addr)
    return cmd_summary(db, main_defs, vendor_defs, ref_defs, status_by_addr, loc_by_addr)


if __name__ == "__main__":
    sys.exit(main())
