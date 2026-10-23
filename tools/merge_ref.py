#!/usr/bin/env python3
"""SA-MP 0.3.7 R5 `_reference/` Fragment Inspector & Merger Helper.

Analyzes the byte-verified C++ fragments in `client/saco/_reference/{closure,eval,matching}`,
compares every function definition against the active files in `client/saco/`, and shows
which functions are identical, modified (R5 adaptations), or missing in `client/saco/`.

Usage examples:
  python3 tools/merge_ref.py                  # Summary of _reference/ functions vs client/saco/
  python3 tools/merge_ref.py --target chatwindow.cpp
  python3 tools/merge_ref.py --diff CChatWindow::AddDebugMessage
"""
from __future__ import annotations

import argparse
import difflib
import re
import sys
from collections import defaultdict

from r5_common import (
    CLASS_TO_FILE,
    REF_DIR,
    SACO_DIR,
    R5FunctionInfo,
    SourceFunctionDef,
    load_r5_database,
    scan_tree_functions,
)


def normalize_code_for_compare(code: str) -> str:
    """Strip comments and normalize whitespace for structural comparison."""
    no_comments = re.sub(r"//[^\r\n]*|/\*[\s\S]*?\*/", "", code)
    tokens = re.findall(r"[A-Za-z_]\w*|\d+|[^\s\w]", no_comments)
    return " ".join(tokens)


def analyze_references():
    db = load_r5_database()
    r5_by_qual: dict[tuple[str, str], list[R5FunctionInfo]] = defaultdict(list)
    for info in db.values():
        if info.method_name:
            r5_by_qual[(info.class_name, info.method_name)].append(info)

    main_defs = scan_tree_functions(SACO_DIR, exclude_dirs={"_reference", "d3d9"})
    ref_defs = scan_tree_functions(REF_DIR) if REF_DIR.exists() else []

    main_by_qual: dict[tuple[str, str], list[SourceFunctionDef]] = defaultdict(list)
    for d in main_defs:
        main_by_qual[(d.class_name, d.method_name)].append(d)

    # Deduplicate ref_defs by (class_name, method_name, normalized_code)
    unique_refs: dict[tuple[str, str], list[SourceFunctionDef]] = defaultdict(list)
    seen_norm: set[tuple[str, str, str]] = set()
    for d in ref_defs:
        # Map Evaluation* classes (e.g. EvaluationPlayerPed) back to C*
        cls = d.class_name
        if cls.startswith("Evaluation"):
            cls = "C" + cls[len("Evaluation"):]
            d.class_name = cls
        norm = normalize_code_for_compare(d.code)
        sig_key = (cls, d.method_name, norm)
        if sig_key in seen_norm:
            continue
        seen_norm.add(sig_key)
        unique_refs[(cls, d.method_name)].append(d)

    records = []
    for (cls, meth), r_list in sorted(unique_refs.items()):
        r_def = r_list[0]
        m_list = main_by_qual.get((cls, meth), [])
        r5_list = r5_by_qual.get((cls, meth), [])
        r5_addr = r5_list[0].address if len(r5_list) == 1 else (r_def.r5_address or None)

        if not m_list:
            state = "MISSING_IN_MAIN"
            target_file = CLASS_TO_FILE.get(cls, "unknown")
            m_def = None
        else:
            m_def = m_list[0]
            target_file = m_def.rel_path.replace("client/saco/", "")
            r_norm = normalize_code_for_compare(r_def.code)
            if any(normalize_code_for_compare(m.code) == r_norm for m in m_list):
                state = "IDENTICAL"
            else:
                state = "DIFFERENT"

        records.append({
            "class": cls,
            "method": meth,
            "qualified": f"{cls}::{meth}" if cls else meth,
            "state": state,
            "target_file": target_file,
            "r5_addr": r5_addr,
            "ref_def": r_def,
            "main_def": m_def,
            "all_ref_defs": r_list,
        })

    return records


def find_fully_merged_files() -> list[tuple[str, int]]:
    """Return `_reference/` files whose every function is already identical in client/saco/."""
    from r5_common import ROOT, extract_source_functions

    main_defs = scan_tree_functions(SACO_DIR, exclude_dirs={"_reference"})
    main_norms: dict[tuple[str, str], set[str]] = defaultdict(set)
    for d in main_defs:
        main_norms[(d.class_name, d.method_name)].add(normalize_code_for_compare(d.code))

    result = []
    for path in sorted(REF_DIR.rglob("*.cpp")):
        defs = extract_source_functions(path)
        if not defs:
            continue
        ok = True
        for d in defs:
            cls = d.class_name
            if cls.startswith("Evaluation"):
                cls = "C" + cls[len("Evaluation"):]
            if normalize_code_for_compare(d.code) not in main_norms.get((cls, d.method_name), set()):
                ok = False
                break
        if ok:
            result.append((str(path.relative_to(ROOT)), len(defs)))
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--clean-identical", action="store_true",
                        help="List _reference/ files whose functions are ALL identical in client/saco/ (add --apply to delete)")
    parser.add_argument("--apply", action="store_true", help="With --clean-identical: actually delete the files")
    parser.add_argument("--target", "-t", help="Filter by target file in client/saco/ (e.g. chatwindow.cpp or game/actorped.cpp)")
    parser.add_argument("--diff", "-d", help="Show unified diff between _reference/ and client/saco/ for a function (e.g. CChatWindow::AddDebugMessage)")
    parser.add_argument("--state", choices=["MISSING_IN_MAIN", "DIFFERENT", "IDENTICAL"], help="Filter by comparison state")
    args = parser.parse_args()

    if args.clean_identical:
        from r5_common import ROOT
        files = find_fully_merged_files()
        print(f"=== {len(files)} _reference/ file(s) fully identical in client/saco/ ===")
        for rel, n in files:
            print(f"  {rel}  ({n} funcs)")
            if args.apply:
                (ROOT / rel).unlink()
        print("Deleted." if args.apply else "Dry run. Re-run with --apply to delete.")
        return 0

    records = analyze_references()

    if args.diff:
        q = args.diff.strip().lower()
        matches = [r for r in records if q in r["qualified"].lower() or q in r["ref_def"].rel_path.lower()]
        if not matches:
            print(f"No reference function matching '{args.diff}'", file=sys.stderr)
            return 1
        for r in matches[:5]:
            addr_str = f"0x{r['r5_addr'].upper()}" if r["r5_addr"] else "unknown"
            print("=" * 78)
            print(f"{r['qualified']}  |  State: {r['state']}  |  R5: {addr_str}")
            print(f"Reference: {r['ref_def'].rel_path}:{r['ref_def'].line_number}")
            if r["main_def"]:
                print(f"Main file: {r['main_def'].rel_path}:{r['main_def'].line_number}")
                print("=" * 78)
                diff = difflib.unified_diff(
                    r["main_def"].code.splitlines(),
                    r["ref_def"].code.splitlines(),
                    fromfile=r["main_def"].rel_path,
                    tofile=r["ref_def"].rel_path,
                    lineterm="",
                )
                print("\n".join(diff) or "(Normalized code is identical)")
            else:
                print(f"Target file: client/saco/{r['target_file']} (MISSING IN MAIN)")
                print("=" * 78)
                if r["r5_addr"]:
                    print(f"// R5: 0x{r['r5_addr'].upper()}")
                print(r["ref_def"].code)
        return 0

    if args.target:
        tq = args.target.strip().lower().replace("client/saco/", "")
        filtered = [r for r in records if tq in r["target_file"].lower()]
        if args.state:
            filtered = [r for r in filtered if r["state"] == args.state]
        print(f"=== Reference Functions for '{args.target}' ({len(filtered)} functions) ===")
        for r in filtered:
            addr = f"0x{r['r5_addr']}" if r["r5_addr"] else "          "
            print(f"  [{r['state']:15}]  {addr}  {r['qualified']:42}  ({r['ref_def'].rel_path}:{r['ref_def'].line_number})")
        return 0

    # Summary by target file
    by_target: dict[str, dict[str, int]] = defaultdict(lambda: {"IDENTICAL": 0, "DIFFERENT": 0, "MISSING_IN_MAIN": 0})
    totals = {"IDENTICAL": 0, "DIFFERENT": 0, "MISSING_IN_MAIN": 0}
    for r in records:
        by_target[r["target_file"]][r["state"]] += 1
        totals[r["state"]] += 1

    print("=== `_reference/` vs `client/saco/` Comparison Summary ===")
    print(f"Unique functions in _reference/ : {len(records)}")
    print(f"  [IDENTICAL]       Already identical in client/saco/     : {totals['IDENTICAL']:4d}")
    print(f"  [DIFFERENT]       Modified in _reference/ (R5-verified) : {totals['DIFFERENT']:4d}")
    print(f"  [MISSING_IN_MAIN] Only in _reference/ (need to copy in) : {totals['MISSING_IN_MAIN']:4d}")
    print()
    print(f"{'Target File (client/saco/)':36} {'Diff':>6} {'Missing':>8} {'Ident':>7} {'Total':>7}")
    print("-" * 70)
    rows = [
        (tf, st["DIFFERENT"], st["MISSING_IN_MAIN"], st["IDENTICAL"], sum(st.values()))
        for tf, st in by_target.items()
    ]
    rows.sort(key=lambda x: (0 if x[0] != "unknown" else 1, -(x[1] + x[2]), x[0]))
    for tf, d_cnt, m_cnt, i_cnt, tot in rows[:40]:
        print(f"{tf[:36]:36} {d_cnt:6d} {m_cnt:8d} {i_cnt:7d} {tot:7d}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
