#!/usr/bin/env python3
"""Delink verified functions from reference samp.dll into COFF objects for objdiff comparison."""
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

from binary import PE, need

DIR32, REL32 = 0x06, 0x14
CODE_FLAGS = 0x60500020  # IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | 16-byte align


def build_coff_for_functions(functions: list[dict], pe: PE) -> bytes:
    """Build a COFF .obj file containing the given functions from the PE binary."""
    symbols = []  # (name, value, section_num, type, storage_class)
    sections = [] # (code_bytes, relocs_bytes, reloc_count)
    externs = {}

    # Define function symbols first (1-based section indices)
    for i, fn in enumerate(functions):
        symbols.append((fn["symbol"], 0, i + 1, 0x20, 2))

    # Process each function body and its relocations
    for i, fn in enumerate(functions):
        rva = fn["rva"]
        size = fn["size"]
        code = bytearray(pe.read(rva, size))
        relocs = bytearray()

        # Find absolute relocations in this function from PE relocation table
        fn_abs_relocs = sorted([r for r in pe.relocations if rva <= r < rva + size])
        for abs_r in fn_abs_relocs:
            off = abs_r - rva
            # Read the 32-bit target address from PE code
            target_addr = struct.unpack_from("<I", code, off)[0]
            sym_name = f"__ref_0x{target_addr:08X}"
            if sym_name not in externs:
                externs[sym_name] = len(symbols)
                symbols.append((sym_name, 0, 0, 0, 2))
            sym_idx = externs[sym_name]
            relocs += struct.pack("<IIH", off, sym_idx, DIR32)

        sections.append((bytes(code), bytes(relocs), len(fn_abs_relocs)))

    # Build symbol table & string table
    strings = bytearray(4)
    table = bytearray()
    for name, value, sec_num, typ, cls in symbols:
        raw_name = name.encode("ascii", "replace")
        if len(raw_name) <= 8:
            field = raw_name.ljust(8, b"\0")
        else:
            field = struct.pack("<II", 0, len(strings))
            strings += raw_name + b"\0"
        table += struct.pack("<8sIhHBB", field, value, sec_num, typ, cls, 0)
    struct.pack_into("<I", strings, 0, len(strings))

    # Calculate layout offsets
    num_sections = len(sections)
    header_size = 20 + 40 * num_sections
    at = header_size

    section_headers = bytearray()
    body = bytearray()

    for code_bytes, relocs_bytes, reloc_count in sections:
        raw_size = len(code_bytes)
        reloc_size = len(relocs_bytes)
        raw_ptr = at
        reloc_ptr = at + raw_size if reloc_count > 0 else 0

        section_headers += struct.pack(
            "<8sIIIIIIHHI",
            b".text\0\0\0",
            0,
            0,
            raw_size,
            raw_ptr,
            reloc_ptr,
            0,
            reloc_count,
            0,
            CODE_FLAGS,
        )
        body += code_bytes + relocs_bytes
        at += raw_size + reloc_size

    # COFF File Header: Machine (0x14C = i386), NumberOfSections, TimeDateStamp,
    # PointerToSymbolTable, NumberOfSymbols, SizeOfOptionalHeader, Characteristics
    file_header = struct.pack(
        "<HHIIIHH",
        0x14C,
        num_sections,
        0,
        at,
        len(symbols),
        0,
        0,
    )

    return file_header + section_headers + body + table + strings


def main():
    manifest_path = ROOT / "config/manifest.json"
    targets_path = ROOT / "config/unreversed_targets.json"
    samp_dll_path = ROOT / "private/samp.dll"
    out_dir = ROOT / "build/target"
    out_dir.mkdir(parents=True, exist_ok=True)

    manifest = json.loads(manifest_path.read_text())
    pe = PE(samp_dll_path, manifest.get("target_sha256"))

    all_functions = list(manifest.get("functions", []))
    if targets_path.exists():
        unreversed = json.loads(targets_path.read_text())
        existing_symbols = {fn["symbol"] for fn in all_functions}
        added = 0
        for fn in unreversed.get("functions", []):
            if fn["symbol"] not in existing_symbols:
                all_functions.append(fn)
                existing_symbols.add(fn["symbol"])
                added += 1
        print(f"Loaded {added} unreversed target functions from {targets_path.relative_to(ROOT)}")

    by_unit = {}
    for fn in all_functions:
        unit = fn.get("unit", "game")
        by_unit.setdefault(unit, []).append(fn)

    print(f"Delinking functions from {samp_dll_path} for {len(by_unit)} units...")
    for unit, fns in sorted(by_unit.items()):
        coff_bytes = build_coff_for_functions(fns, pe)
        out_path = out_dir / f"{unit}.obj"
        out_path.write_bytes(coff_bytes)
        print(f"  -> Generated {out_path.relative_to(ROOT)} ({len(fns)} functions, {len(coff_bytes)} bytes)")

    print("PASS: Delink complete.")


if __name__ == "__main__":
    main()
