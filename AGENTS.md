# SA-MP R5 functional reconstruction contract

- Objective: decompile **every** function of the frozen SA-MP 0.3.7 R5 client
  (`samp.dll`) and rewrite it as readable C++, producing a playable DLL that is
  compatible with R5 servers. Byte-for-byte / binary matching is NOT required.
- Coverage is 1:1 with R5: every function, class, RPC, hook, patch and data
  table present in R5 must exist in source, including code that R5 never calls
  (dead code, debug helpers, unused features). Do not skip, merge or "improve".
- Behavior must equal R5: same struct layouts and field offsets used by other
  code, same network protocol (packet/RPC IDs, BitStream field order and
  widths), same GTA:SA memory addresses, hooks and patches, same strings,
  constants, file formats and encryption.
- Reference SHA-256: b72b5dbe725f81864ca3f78bc7063bda56cc05fc7188af822fa7a754432553a2.
  Keep proprietary binaries, Ghidra exports and build products in ignored
  `private/`, `ghidra_export/` or `build/`. Never commit a reference executable.
- Write normal C++ that builds with MSVC 2003 (`cl.exe` 13.10.3077). No `_emit`,
  incbin or copied machine-code bytes. Inline assembly only where R5 behavior
  truly needs it (naked hooks, register-level trampolines) and must be commented.
- Annotate each reconstructed function with its R5 address, e.g.
  `// R5: 0x100010B0` directly above the definition, so coverage can be tracked
  against `ghidra_export/`. Name unknown things `sub_XXXXXXXX` / `unkXX` rather
  than inventing semantics.
- Prefer the main source tree (`client/saco/`). `client/saco/_reference/` holds
  previously byte-verified fragments; merge them into the main files instead of
  rewriting them, then delete the fragment.
- Do not alter legacy repositories (`vendor/upstream/`). Copy code out of it.
- Test compatibility only in a separate GTA:SA install against an R5 server.
