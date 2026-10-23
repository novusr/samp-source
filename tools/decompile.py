#!/usr/bin/env python3
"""SA-MP 0.3.7 R5 Fast Decompiler, Disassembler & Source Comparison Tool.

Reads `ghidra_export/` directly in milliseconds and cross-references against
`client/saco/`, `client/saco/_reference/`, and `vendor/upstream/`.

Usage examples:
  python3 tools/decompile.py 0x100010b0
  python3 tools/decompile.py CPlayerPool::GetSlotState --asm --xrefs
  python3 tools/decompile.py --class CActorPool
  python3 tools/decompile.py --class CCamera --scaffold
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

from r5_common import (
    GHIDRA_DIR,
    REF_DIR,
    SACO_DIR,
    R5FunctionInfo,
    load_r5_database,
    normalize_addr,
    normalize_class,
    scan_tree_functions,
)


def load_function_json(addr: str) -> dict | None:
    p = GHIDRA_DIR / f"{addr}.json"
    if not p.exists():
        return None
    return json.loads(p.read_text(encoding="utf-8"))


def resolve_target(query: str, db: dict[str, R5FunctionInfo]) -> list[R5FunctionInfo]:
    q = query.strip()
    # Try hex address
    try:
        addr = normalize_addr(q)
        if addr in db:
            return [db[addr]]
    except ValueError:
        pass

    # Try Class::Method or Class.Method
    q_norm = q.replace(".", "::")
    if "::" in q_norm:
        cls_part, meth_part = q_norm.split("::", 1)
        cls_norm = normalize_class(cls_part)
        exact = [
            info for info in db.values()
            if info.class_name.lower() == cls_norm.lower() and info.method_name.lower() == meth_part.lower()
        ]
        if exact:
            return exact

    # Fallback substring search on qualified name or symbol
    matches = [
        info for info in db.values()
        if q.lower() in info.qualified_name.lower() or q.lower() in info.symbol.lower()
    ]
    return sorted(matches, key=lambda x: x.address)


def print_function_detail(
    info: R5FunctionInfo,
    db: dict[str, R5FunctionInfo],
    show_asm: bool,
    show_xrefs: bool,
    show_source: bool,
    main_defs,
    ref_defs,
) -> None:
    fn_json = load_function_json(info.address)
    print("=" * 78)
    print(f"// R5: 0x{info.address.upper()}  |  {info.qualified_name}")
    if info.symbol:
        print(f"// Symbol: {info.symbol}")
    print("=" * 78)

    if fn_json:
        if show_xrefs:
            callers = fn_json.get("callers", [])
            callees = fn_json.get("callees", [])
            print(f"\n--- Callers ({len(callers)}) ---")
            for c in callers[:25]:
                c_addr = c.get("addr", "")
                c_name = db[c_addr].qualified_name if c_addr in db else c.get("name", c_addr)
                print(f"  0x{c_addr}  {c_name} ({c.get('ref_type', '')})")
            print(f"\n--- Callees ({len(callees)}) ---")
            for c in callees[:25]:
                c_addr = c.get("addr", "")
                c_name = db[c_addr].qualified_name if c_addr in db else c.get("name", c_addr)
                print(f"  0x{c_addr}  {c_name} ({c.get('ref_type', '')})")

        decomp = (fn_json.get("decompiled") or "").strip()
        print("\n--- Ghidra Decompiled C ---")
        print(decomp if decomp else "(no decompilation in export)")

        if show_asm:
            asm_lines = fn_json.get("assembly", [])
            print(f"\n--- x86 Assembly ({len(asm_lines)} instructions) ---")
            for ln in asm_lines:
                print(f"  {ln}")
    else:
        print("(No function JSON file in ghidra_export/ for this address)")

    if show_source:
        key = (info.class_name, info.method_name)
        m_hits = [
            d for d in main_defs
            if d.r5_address == info.address or (info.method_name and (d.class_name, d.method_name) == key)
        ]
        r_hits = [
            d for d in ref_defs
            if d.r5_address == info.address or (info.method_name and (d.class_name, d.method_name) == key)
        ]
        if m_hits:
            for d in m_hits[:2]:
                print(f"\n--- Current Implementation in {d.rel_path}:{d.line_number} ---")
                print(d.code)
        if r_hits:
            for d in r_hits[:2]:
                print(f"\n--- Reference Implementation in {d.rel_path}:{d.line_number} ---")
                print(d.code)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("target", nargs="?", help="Function address (e.g. 0x100010b0) or name (e.g. CPlayerPool::GetSlotState)")
    parser.add_argument("--class", dest="cls", help="Inspect or scaffold all functions in a class")
    parser.add_argument("--asm", action="store_true", help="Include x86 assembly listing")
    parser.add_argument("--xrefs", action="store_true", help="Include callers and callees")
    parser.add_argument("--no-source", action="store_true", help="Do not scan client/saco/ for existing C++ definitions")
    parser.add_argument("--scaffold", action="store_true", help="Output C++ function stubs with '// R5: 0x...' comments")
    args = parser.parse_args()

    db = load_r5_database()

    if args.cls:
        norm_q = normalize_class(args.cls)
        items = [
            info for info in db.values()
            if info.class_name.lower() == norm_q.lower()
        ]
        items.sort(key=lambda x: x.address)
        if not items:
            print(f"No functions found for class '{args.cls}'", file=sys.stderr)
            return 1

        if args.scaffold:
            for info in items:
                fn_json = load_function_json(info.address)
                sig_comment = ""
                if fn_json and fn_json.get("decompiled"):
                    first_ln = [ln.strip() for ln in fn_json["decompiled"].splitlines() if ln.strip()]
                    if first_ln:
                        sig_comment = f" // Ghidra: {first_ln[0]}"
                print(f"// R5: 0x{info.address.upper()}{sig_comment}")
                print(f"void {info.qualified_name}()\n{{\n    // TODO: reconstruct from R5 0x{info.address.upper()}\n}}\n")
            return 0

        main_defs = [] if args.no_source else scan_tree_functions(SACO_DIR, exclude_dirs={"_reference", "d3d9"})
        ref_defs = [] if args.no_source else (scan_tree_functions(REF_DIR) if REF_DIR.exists() else [])
        for info in items:
            print_function_detail(info, db, args.asm, args.xrefs, not args.no_source, main_defs, ref_defs)
        return 0

    if not args.target:
        parser.print_help()
        return 0

    matches = resolve_target(args.target, db)
    if not matches:
        print(f"No R5 function matching '{args.target}'", file=sys.stderr)
        return 1

    if len(matches) > 5:
        print(f"Found {len(matches)} matches for '{args.target}' (showing first 15):")
        for m in matches[:15]:
            print(f"  0x{m.address}  {m.qualified_name:45}  {m.symbol}")
        print("\nSpecify a more exact name or address.")
        return 0

    main_defs = [] if args.no_source else scan_tree_functions(SACO_DIR, exclude_dirs={"_reference", "d3d9"})
    ref_defs = [] if args.no_source else (scan_tree_functions(REF_DIR) if REF_DIR.exists() else [])
    for info in matches:
        print_function_detail(info, db, args.asm, args.xrefs, not args.no_source, main_defs, ref_defs)
    return 0


if __name__ == "__main__":
    sys.exit(main())
