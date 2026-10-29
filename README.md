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

**Current Active Pipeline: 228 functions matched (12,735 bytes) · 204 functions formally verified in manifest (11,605 bytes).**

Total reference `.text` section: **930,756 bytes (8,020 functions)**. Reconstructed source is continuously compiled with Microsoft Visual C++ Toolkit 2003 and validated against reference `samp.dll` (`b72b5dbe725f81864ca3f78bc7063bda56cc05fc7188af822fa7a754432553a2`).

Accepted and in-progress coverage includes parts of:

- **Game and client code:** actor lifecycle, entity geometry, camera/audio wrappers, native pools, player state, weapon skills, commands, menus, RenderWare and shared utilities.
- **Interface & Networking:** dialog lifecycle, command window, packet handling, and RakNet containers.
- **Mathematical & Security Utilities:** vector geometry, TEA block encryption, MD5 transform, and SHA1 digest routines.

*(Note: Legacy upstream research recorded experimental batches up to CP3.2; this repository tracks strictly clean, reproducible C++ units with fail-closed boundary and relocation checks under native Linux.)*

### Active Reconstruction Pipeline (Linux / Parallel Multi-Worker)

The active workspace source tree (`src/`) is continuously verified by `tools/verify.py` and matched using `tools/delink.py` and `decompdecomp`:

- **Verified Functions in Active Manifest:** **204 / 204 functions PASS (11,605 unique code bytes)**
- **Unit Distribution (Verified):**
  - `game`: 89 functions matched (e.g. `CMenu::CMenu` [264B], `TEABlockEncryptor::EncryptBlock` [380B], `TEABlockEncryptor::DecryptBlock` [389B], `CD3DArcBall::Reset` [265B])
  - `pools`: 58 functions matched (e.g. `GamePool_Ped_GetAt` [35B], `CPlayerPed::GetHealth` [24B], `CGangZonePool` [40B])
  - `camera`: 47 functions matched (e.g. `Util::MD5Transform` [2,162B], `fhicl::SHA1::ProcessMessageBlock` [1,571B], `CCamera::GetMatrix` [113B])
  - `actors`: 10 functions matched (e.g. `ActorPool` and `CActorPed` lifecycle, immunity, and animations)

- **decompdecomp Agent Swarm Pipeline (`objdiff`):**
  - **1,546 functions** in active pipeline (159,476 code bytes)
  - **228 functions matched** (100% byte-exact)
  - **1,309 functions queued open** for multi-agent matching
  - **9 functions marked `asm`** (original SA-MP inline assembly exceptions)

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
