#!/usr/bin/env python3
"""SA-MP 0.3.7 R5 1:1 Function Coverage & Auto-Annotation Tool.

Tracks progress of reconstructing all 5,265 functions in `ghidra_export/_index.json`
(and symbol mappings in `ghidra_export/address_map.json`) into `client/saco/`.

Usage examples:
  python3 tools/progress.py                  # Full summary dashboard + per-class breakdown
  python3 tools/progress.py --class CNetGame # Show all R5 functions in a class
  python3 tools/progress.py --file net/playerpool.cpp
  python3 tools/progress.py --next           # Recommend next functions/classes to work on
  python3 tools/progress.py --annotate       # Preview automatic '// R5: 0x...' annotations
  python3 tools/progress.py --annotate --apply
"""
from __future__ import annotations

import argparse
import sys
from collections import defaultdict
from pathlib import Path

from r5_common import (
    CLASS_TO_FILE,
    REF_DIR,
    ROOT,
    SACO_DIR,
    R5FunctionInfo,
    SourceFunctionDef,
    load_r5_database,
    normalize_class,
    scan_tree_functions,
)


def compute_coverage():
    db = load_r5_database()
    main_defs = scan_tree_functions(SACO_DIR, exclude_dirs={"_reference", "d3d9"})
    ref_defs = scan_tree_functions(REF_DIR) if REF_DIR.exists() else []

    # Index R5 functions by (class_name, method_name) and by method_name (for globals)
    r5_by_qual: dict[tuple[str, str], list[R5FunctionInfo]] = defaultdict(list)
    for info in db.values():
        if info.method_name and not info.method_name.startswith("FUN_"):
            r5_by_qual[(info.class_name, info.method_name)].append(info)

    # Index source definitions by R5 address and by (class_name, method_name)
    main_by_addr: dict[str, SourceFunctionDef] = {}
    main_by_qual: dict[tuple[str, str], list[SourceFunctionDef]] = defaultdict(list)
    for d in main_defs:
        if d.r5_address:
            main_by_addr[d.r5_address] = d
        main_by_qual[(d.class_name, d.method_name)].append(d)

    ref_by_addr: dict[str, list[SourceFunctionDef]] = defaultdict(list)
    ref_by_qual: dict[tuple[str, str], list[SourceFunctionDef]] = defaultdict(list)
    for d in ref_defs:
        if d.r5_address:
            ref_by_addr[d.r5_address].append(d)
        ref_by_qual[(d.class_name, d.method_name)].append(d)

    # Categorize each R5 function
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

    return db, main_defs, ref_defs, r5_by_qual, status_by_addr, loc_by_addr


def cmd_summary(db, main_defs, ref_defs, status_by_addr, loc_by_addr) -> int:
    # Count among primary _index.json functions (5,265) and all DB entries
    idx_addrs = [a for a, info in db.items() if info.in_index]
    counts = defaultdict(int)
    idx_counts = defaultdict(int)
    for a, st in status_by_addr.items():
        counts[st] += 1
        if db[a].in_index:
            idx_counts[st] += 1

    total_idx = len(idx_addrs)
    covered_idx = idx_counts["ANNOTATED"] + idx_counts["IN_SOURCE"]
    pct_idx = (covered_idx / total_idx * 100.0) if total_idx else 0.0

    print("=== SA-MP 0.3.7 R5 Functional Reconstruction Coverage ===")
    print(f"Total R5 Decompiled Functions (_index.json): {total_idx}")
    print(f"Total R5 Symbol/Address Map Entries        : {len(db)}")
    print(f"C++ Function Definitions in client/saco/   : {len(main_defs)}")
    print(f"C++ Function Definitions in _reference/    : {len(ref_defs)}")
    print()
    print(f"Status Breakdown (Index / All Symbol Map):")
    print(f"  [ANNOTATED]    Explicit '// R5: 0x...' in client/saco/ : {idx_counts['ANNOTATED']:4d}  (all: {counts['ANNOTATED']:4d})")
    print(f"  [IN_SOURCE]    Matched by Class::Method in client/saco/: {idx_counts['IN_SOURCE']:4d}  (all: {counts['IN_SOURCE']:4d})")
    print(f"  [IN_REFERENCE] Available in client/saco/_reference/    : {idx_counts['IN_REFERENCE']:4d}  (all: {counts['IN_REFERENCE']:4d})")
    print(f"  [NAMED_TODO]   Named in Ghidra, missing in source      : {idx_counts['NAMED_TODO']:4d}  (all: {counts['NAMED_TODO']:4d})")
    print(f"  [UNNAMED_TODO] Unnamed FUN_10xxxxxx in Ghidra          : {idx_counts['UNNAMED_TODO']:4d}  (all: {counts['UNNAMED_TODO']:4d})")
    print(f"  ------------------------------------------------------------------------")
    print(f"  Active Source Coverage (ANNOTATED + IN_SOURCE)         : {covered_idx}/{total_idx} ({pct_idx:.2f}%)")
    print()

    # Per-class table for classes that exist in client/saco/ or have >= 3 functions
    by_class: dict[str, list[str]] = defaultdict(list)
    for a, info in db.items():
        if info.class_name:
            by_class[info.class_name].append(a)

    print(f"{'Class Name':30} {'Annot':>6} {'InSrc':>6} {'InRef':>6} {'Todo':>6} {'Total':>6}  {'Target File'}")
    print("-" * 88)
    rows = []
    for cls, addrs in by_class.items():
        c_ann = sum(1 for a in addrs if status_by_addr[a] == "ANNOTATED")
        c_src = sum(1 for a in addrs if status_by_addr[a] == "IN_SOURCE")
        c_ref = sum(1 for a in addrs if status_by_addr[a] == "IN_REFERENCE")
        c_todo = len(addrs) - c_ann - c_src - c_ref
        target = CLASS_TO_FILE.get(cls, "")
        if target or len(addrs) >= 5:
            rows.append((cls, c_ann, c_src, c_ref, c_todo, len(addrs), target))

    rows.sort(key=lambda r: (0 if r[6] else 1, -(r[1] + r[2] + r[3]), r[0]))
    for cls, c_ann, c_src, c_ref, c_todo, tot, target in rows[:45]:
        print(f"{cls[:30]:30} {c_ann:6d} {c_src:6d} {c_ref:6d} {c_todo:6d} {tot:6d}  {target}")
    if len(rows) > 45:
        print(f"... ({len(rows) - 45} more classes; use --class <Name> to inspect)")
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
        print(f"  0x{info.address}  [{st:12}]  {info.qualified_name:42}{loc_str}")
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
        # Also check how many definitions of this (class_name, method_name) exist in source
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
            # Insert in reverse line order so line numbers stay valid
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
    parser.add_argument("--class", dest="cls", help="Inspect R5 coverage for a specific class (e.g. CPlayerPool)")
    parser.add_argument("--next", action="store_true", help="Recommend next functions to merge/reconstruct")
    parser.add_argument("--limit", type=int, default=20, help="Max items for --next (default: 20)")
    parser.add_argument("--annotate", action="store_true", help="Find unannotated functions in client/saco/ with unique R5 addresses")
    parser.add_argument("--apply", action="store_true", help="Write '// R5: 0x...' comments when used with --annotate")
    args = parser.parse_args()

    db, main_defs, ref_defs, r5_by_qual, status_by_addr, loc_by_addr = compute_coverage()

    if args.annotate:
        return cmd_annotate(args.apply, db, main_defs, r5_by_qual)
    if args.cls:
        return cmd_class(args.cls, db, status_by_addr, loc_by_addr)
    if args.next:
        return cmd_next(args.limit, db, status_by_addr, loc_by_addr)
    return cmd_summary(db, main_defs, ref_defs, status_by_addr, loc_by_addr)


if __name__ == "__main__":
    sys.exit(main())
