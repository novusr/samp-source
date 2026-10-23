<div align="center">

# SAMP Source

**Rebuilding a multiplayer classic. One exact byte at a time.**

SA-MP 0.3.7 R5 · C++ · AI-assisted reverse engineering

</div>

The goal is simple: rebuild **`samp.dll` from source and produce a file identical to the original**.

SAMP Source is a software preservation project for San Andreas Multiplayer. We reconstruct the code, compile it with the original-era tools, and check the result against R5. Every accepted match comes with evidence you can inspect.

## How it works

**Reconstruct → Compile → Compare → Keep the exact matches.**

Functions are checked in full, including their references, call targets and jump tables. Differences are never hidden to make a result pass, and original machine-code blobs are never pasted into the reconstruction. Documented assembly is used where needed.

## Progress

**402,825 / 930,756 code bytes verified — 43.28% of the original code section.**

Latest accepted checkpoint: **September 6, 2026 · CP3.2**. Coverage counts unique accepted code bytes; overlapping matches and unimplemented dependencies receive no extra credit.

Accepted coverage includes parts of:

- **Game and client code:** actor lifecycle, entity geometry, camera/audio wrappers, native pools, player state, weapon skills, commands, menus, RenderWare and shared utilities.
- **Interface:** dialog lifecycle, drawing and message routing, control rendering and input, text editing, IME composition and resource lookup.
- **Networking:** accepted portions of RakPeer, RakClient, Router, RPC handling, reliability, connection graphs and shared containers.
- **DXUT framework:** timers, resource-cache lifecycle and creation, framework state and shutdown, adapter enumeration, device-settings construction, command-line parsing, camera helpers and render-loop handling.

The latest batch added **5,957 unique exact code bytes**. Its complete-region and relocation/target checks passed, together with **10 negative controls** and the existing ABI checks. See the linked acceptance evidence for scope and limitations.

### Active Reconstruction Pipeline (Linux / Parallel Multi-Worker)

The active workspace source tree (`src/`) is continuously verified by `tools/verify.py` and matched using `tools/r5_matcher.py`:

- **Verified Functions in Active Manifest:** **157 / 157 functions PASS (5,776 unique code bytes)**
- **Unit Distribution:**
  - `game`: 68 functions matched (e.g. `CMenu::CMenu` [264B], `GetVehicleSubtypeFromVehiclePtr` [106B], `CEntity::ApplyTurnSpeed` [28B], `CEntity::GetBoundCentre` [33B])
  - `pools`: 41 functions matched (e.g. `GamePool_Ped_GetAt` [35B], `CPlayerPed::GetHealth` [24B])
  - `camera`: 38 functions matched (e.g. `CCamera::GetMatrix` [113B])
  - `actors`: 10 functions matched (e.g. `CActorPed` lifecycle, immunity, and animations)

The local 0.2.5 reference has contributed source and algorithms used in accepted reconstructions. The latest integration report records **22,342 cumulative directly attributed code bytes**; adapted implementations and shared source are tracked separately.

| Milestone | Status |
| --- | --- |
| Toolchain, binary mapping and initial dependencies | Established |
| Reconstruct and match the client | In progress |
| Integrate a complete client | Ahead |
| Reproduce the entire DLL, byte for byte | The finish line |

This is an early reconstruction, not a playable replacement DLL. Matching individual functions is progress toward full-file identity, not proof of it.

[Verified coverage](evidence/checkpoint32/coverage-current.json) · [Latest accepted batch](evidence/checkpoint32/client-router-rpc-acceptance.json) · [Development log](evidence/checkpoint32/PROGRESS.md)

## Explore or contribute

- **[Source](client/saco/)** — the client reconstruction and imported reference code. Presence here does not imply a verified match.
- **[Tools](tools/)** — compilation, binary comparison and validation.
- **[Evidence](evidence/)** — accepted results and their limits.
- **[Matching rules](AGENTS.md)** — what qualifies as an exact match.

Reproduction requires your own R5 DLL, Visual Studio .NET 2003 and Python. The current build automation uses a Windows VM through Parallels on macOS. The original DLL, build artifacts and compiler are not included.

**Linux users:** A full native Linux workflow using Wine is supported. The Visual C++ Toolkit 2003 (`cl.exe` 13.10.3077) runs correctly under Wine and produces bit-identical object files. See **[docs/linux-setup.md](docs/linux-setup.md)** for the complete setup guide.

## Credits

Built on earlier community work, including [dashr9230/SA-MP](https://github.com/dashr9230/SA-MP), with [source provenance recorded](config/checkpoint32/provenance.json). Prior work provides leads; binary comparisons establish the matches.
