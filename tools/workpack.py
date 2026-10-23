#!/usr/bin/env python3
"""SA-MP 0.3.7 R5 compact work-packet generator (token saver for AI agents).

Prints, in ONE command, everything an agent needs to finish a class or file:
only the R5 functions that are NOT yet annotated in client/saco/, plus any
matching `_reference/` code and the Ghidra decompilation (trimmed).

Usage:
  python3 tools/workpack.py CChatWindow
  python3 tools/workpack.py game/audio.cpp
  python3 tools/workpack.py CGame --limit 10 --max-lines 60
  python3 tools/workpack.py CGame --list          # names/addresses only
"""
from __future__ import annotations

import argparse
import json
import sys
from collections import defaultdict

from r5_common import (
    CLASS_TO_FILE,
    GHIDRA_DIR,
    REF_DIR,
    SACO_DIR,
    load_r5_database,
    normalize_class,
    scan_tree_functions,
)


def classes_for_target(target: str) -> list[str]:
    t = target.replace("client/saco/", "").strip()
    if t.endswith((".cpp", ".h")):
        t = t.replace(".h", ".cpp")
        return sorted(c for c, f in CLASS_TO_FILE.items() if f.lower().endswith(t.lower()))
    return [normalize_class(t)]


def trim(text: str, max_lines: int) -> str:
    lines = text.strip().splitlines()
    if len(lines) <= max_lines:
        return "\n".join(lines)
    return "\n".join(lines[:max_lines]) + f"\n  ... ({len(lines) - max_lines} more lines; use tools/decompile.py)"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("target", help="Class (CGame) or file (game/audio.cpp)")
    ap.add_argument("--limit", type=int, default=0, help="Max functions to include (0 = all)")
    ap.add_argument("--max-lines", type=int, default=80, help="Max lines per code block")
    ap.add_argument("--list", action="store_true", help="Only list pending functions")
    ap.add_argument("--no-ghidra", action="store_true", help="Skip Ghidra pseudo-C")
    args = ap.parse_args()

    classes = classes_for_target(args.target)
    if not classes:
        print(f"No class mapped to '{args.target}'", file=sys.stderr)
        return 1

    db = load_r5_database()
    main_defs = scan_tree_functions(SACO_DIR, exclude_dirs={"_reference"})
    ref_defs = scan_tree_functions(REF_DIR) if REF_DIR.exists() else []

    annotated = {d.r5_address for d in main_defs if d.r5_address}
    main_by_key = defaultdict(list)
    for d in main_defs:
        main_by_key[(d.class_name, d.method_name)].append(d)
    ref_by_key = defaultdict(list)
    ref_by_addr = defaultdict(list)
    for d in ref_defs:
        cls = d.class_name
        if cls.startswith("Evaluation"):
            cls = "C" + cls[len("Evaluation"):]
        ref_by_key[(cls, d.method_name)].append(d)
        if d.r5_address:
            ref_by_addr[d.r5_address].append(d)

    cls_set = {c.lower() for c in classes}
    funcs = sorted((i for i in db.values() if i.class_name.lower() in cls_set), key=lambda i: i.address)
    pending = [i for i in funcs if i.address not in annotated]
    done = len(funcs) - len(pending)

    print(f"# WORKPACK {args.target}  classes={','.join(classes)}")
    print(f"# R5 functions: {len(funcs)}  done(annotated): {done}  pending: {len(pending)}")
    for c in classes:
        if c in CLASS_TO_FILE:
            print(f"# file: client/saco/{CLASS_TO_FILE[c]}")
    if args.limit:
        pending = pending[: args.limit]

    for info in pending:
        key = (info.class_name, info.method_name)
        m = main_by_key.get(key, [])
        r = ref_by_addr.get(info.address) or ref_by_key.get(key, [])
        state = "UNANNOTATED" if m else ("REF_ONLY" if r else "MISSING")
        loc = f" @ {m[0].rel_path}:{m[0].line_number}" if m else ""
        print(f"\n## 0x{info.address.upper()} {info.qualified_name} [{state}]{loc}")
        if args.list:
            continue
        if r:
            print(f"### ref {r[0].rel_path}:{r[0].line_number}")
            print(trim(r[0].code, args.max_lines))
        if not args.no_ghidra and not m:
            p = GHIDRA_DIR / f"{info.address}.json"
            if p.exists():
                dec = (json.loads(p.read_text(encoding="utf-8")).get("decompiled") or "").strip()
                if dec:
                    print("### ghidra")
                    print(trim(dec, args.max_lines))
    print("\n# Next: edit file, add '// R5: 0x...' above each definition, then "
          "python3 tools/build.py --unit <file>")
    return 0


if __name__ == "__main__":
    sys.exit(main())
