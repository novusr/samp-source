#!/usr/bin/env python3
"""Shared utilities for SA-MP 0.3.7 R5 functional reconstruction tools.

Provides fast offline access to `ghidra_export/` (`_index.json`, `address_map.json`,
per-function JSONs, `_strings.json`, `_structs.json`) and C++ source function scanning
across `client/saco/` and `client/saco/_reference/`.
"""
from __future__ import annotations

import dataclasses
import json
import re
from pathlib import Path
from typing import Any

ROOT = Path(__file__).resolve().parent.parent
GHIDRA_DIR = ROOT / "ghidra_export"
INDEX_PATH = GHIDRA_DIR / "_index.json"
ADDRESS_MAP_PATH = GHIDRA_DIR / "address_map.json"
SACO_DIR = ROOT / "client" / "saco"
REF_DIR = SACO_DIR / "_reference"
UPSTREAM_SACO_DIR = ROOT / "vendor" / "upstream" / "saco"
UPSTREAM_RAKNET_DIR = ROOT / "vendor" / "upstream" / "raknet"

# Canonical class name aliases between early Ghidra symbols and client/saco/
CLASS_ALIASES: dict[str, str] = {
    "ActorPool": "CActorPool",
    "PlayerPool": "CPlayerPool",
    "LocalPlayer": "CLocalPlayer",
    "Game": "CGame",
    "VehiclePool": "CVehiclePool",
    "ObjectPool": "CObjectPool",
    "PickupPool": "CPickupPool",
    "GangZonePool": "CGangZonePool",
    "MenuPool": "CMenuPool",
    "TextDrawPool": "CTextDrawPool",
    "LabelPool": "CLabelPool",
    "NetGame": "CNetGame",
    "ChatWindow": "CChatWindow",
    "CmdWindow": "CCmdWindow",
    "DeathWindow": "CDeathWindow",
    "SpawnScreen": "CSpawnScreen",
    "ScoreBoard": "CScoreBoard",
    "PlayerTags": "CPlayerTags",
    "NewPlayerTags": "CNewPlayerTags",
    "FontRender": "CFontRender",
    "AudioStream": "CAudioStream",
    "HelpDialog": "CHelpDialog",
    "NetStats": "CNetStats",
    "SvrNetStats": "CSvrNetStats",
}

# Mapping from canonical class name to primary source file relative to client/saco/
CLASS_TO_FILE: dict[str, str] = {
    "CActorPed": "game/actorped.cpp",
    "CPlayerPed": "game/playerped.cpp",
    "CEntity": "game/entity.cpp",
    "CVehicle": "game/vehicle.cpp",
    "CCamera": "game/camera.cpp",
    "CAudio": "game/audio.cpp",
    "CGame": "game/game.cpp",
    "CMenu": "game/menu.cpp",
    "CTask": "game/task.cpp",
    "CTaskJetpack": "game/task.cpp",
    "CTaskGoToPoint": "game/task.cpp",
    "CTaskKillPedOnFootArmed": "game/task.cpp",
    "CTaskTakeDamageFall": "game/task.cpp",
    "CTaskGoggles": "game/task.cpp",
    "CActorPool": "net/actorpool.cpp",
    "CPlayerPool": "net/playerpool.cpp",
    "CVehiclePool": "net/vehiclepool.cpp",
    "CObjectPool": "net/objectpool.cpp",
    "CPickupPool": "net/pickuppool.cpp",
    "CGangZonePool": "net/gangzonepool.cpp",
    "CMenuPool": "net/menupool.cpp",
    "CTextDrawPool": "net/textdrawpool.cpp",
    "CLabelPool": "net/labelpool.cpp",
    "CLocalPlayer": "net/localplayer.cpp",
    "CRemotePlayer": "net/remoteplayer.cpp",
    "CNetPlayer": "net/netplayer.cpp",
    "CNetGame": "net/netgame.cpp",
    "CChatWindow": "chatwindow.cpp",
    "CCmdWindow": "cmdwindow.cpp",
    "CDeathWindow": "deathwindow.cpp",
    "CSpawnScreen": "spawnscreen.cpp",
    "CScoreBoard": "scoreboard.cpp",
    "CPlayerTags": "playertags.cpp",
    "CNewPlayerTags": "newplayertags.cpp",
    "CFontRender": "fontrender.cpp",
    "CAudioStream": "audiostream.cpp",
    "CHelpDialog": "helpdialog.cpp",
    "CNetStats": "netstats.cpp",
    "CSvrNetStats": "svrnetstats.cpp",
    "CConfig": "config.cpp",
    "CFileSystem": "filesystem.cpp",
    "CHttpClient": "httpclient.cpp",
    "CLabel": "label.cpp",
    "CArchiveFS": "archive/ArchiveFS.cpp",
    "CCryptoContext": "archive/CryptoContext.cpp",
    "CHasher": "archive/Hasher.cpp",
    "CKeyPair": "archive/KeyPair.cpp",
    "CSigner": "archive/Signer.cpp",
    "CTinyEncrypt": "archive/TinyEncrypt.cpp",
    "ID3DXFontHook": "d3dhook/ID3DXFontHook.cpp",
    "IDirect3DDevice9Hook": "d3dhook/IDirect3DDevice9Hook.cpp",
    "CUnkClass3": "unkclass3.cpp",
    "CUnkClass4": "unkclass4.cpp",
    "CUnkClass5": "unkclass5.cpp",
    "CUnkClass8": "unkclass8.cpp",
    "CUnkClass10": "unkclass10.cpp",
    "CUnkClass11": "unkclass11.cpp",
    "CUnkClass12": "unkclass12.cpp",
    "CUnkClass13": "unkclass13.cpp",
    "CUnkClass14": "unkclass14.cpp",
    "CUnkClass15": "unkclass15.cpp",
    # DXUT (vendor/upstream/saco/d3d9/common/)
    "CDXUTDialog": "d3d9/common/DXUTgui.cpp",
    "CDXUTDialogResourceManager": "d3d9/common/DXUTgui.cpp",
    "CDXUTControl": "d3d9/common/DXUTgui.cpp",
    "CDXUTButton": "d3d9/common/DXUTgui.cpp",
    "CDXUTStatic": "d3d9/common/DXUTgui.cpp",
    "CDXUTCheckBox": "d3d9/common/DXUTgui.cpp",
    "CDXUTRadioButton": "d3d9/common/DXUTgui.cpp",
    "CDXUTComboBox": "d3d9/common/DXUTgui.cpp",
    "CDXUTSlider": "d3d9/common/DXUTgui.cpp",
    "CDXUTEditBox": "d3d9/common/DXUTgui.cpp",
    "CDXUTIMEEditBox": "d3d9/common/DXUTgui.cpp",
    "CDXUTListBox": "d3d9/common/DXUTgui.cpp",
    "CDXUTScrollBar": "d3d9/common/DXUTgui.cpp",
    "CDXUTElement": "d3d9/common/DXUTgui.cpp",
    "CUniBuffer": "d3d9/common/DXUTgui.cpp",
    "CD3DEnumeration": "d3d9/common/DXUTenum.cpp",
    "CD3DEnumAdapterInfo": "d3d9/common/DXUTenum.cpp",
    "CD3DEnumDeviceInfo": "d3d9/common/DXUTenum.cpp",
    "CDXUTResourceCache": "d3d9/common/DXUTmisc.cpp",
    "CDXUTTimer": "d3d9/common/DXUTmisc.cpp",
    "CDXUTLineManager": "d3d9/common/DXUTmisc.cpp",
    "CDXUTTextHelper": "d3d9/common/DXUTmisc.cpp",
    "CDXUTDirectionWidget": "d3d9/common/DXUTmisc.cpp",
    "CD3DArcBall": "d3d9/common/DXUTmisc.cpp",
    "CBaseCamera": "d3d9/common/DXUTmisc.cpp",
    "CFirstPersonCamera": "d3d9/common/DXUTmisc.cpp",
    "CModelViewerCamera": "d3d9/common/DXUTmisc.cpp",
    "CD3DSettingsDlg": "d3d9/common/DXUTSettingsDlg.cpp",
    "DXUTState": "d3d9/common/DXUT.cpp",
    # RakNet (vendor/upstream/raknet/)
    "RakPeer": "../vendor/upstream/raknet/RakPeer.cpp",
    "RakClient": "../vendor/upstream/raknet/RakClient.cpp",
    "RakServer": "../vendor/upstream/raknet/RakServer.cpp",
    "ReliabilityLayer": "../vendor/upstream/raknet/ReliabilityLayer.cpp",
    "Router": "../vendor/upstream/raknet/Router.cpp",
    "RPCMap": "../vendor/upstream/raknet/RPCMap.cpp",
    "ConnectionGraph": "../vendor/upstream/raknet/ConnectionGraph.cpp",
    "StringCompressor": "../vendor/upstream/raknet/StringCompressor.cpp",
    "StringTable": "../vendor/upstream/raknet/StringTable.cpp",
    "DataBlockEncryptor": "../vendor/upstream/raknet/DataBlockEncryptor.cpp",
    "SocketLayer": "../vendor/upstream/raknet/SocketLayer.cpp",
    "SimpleMutex": "../vendor/upstream/raknet/SimpleMutex.cpp",
    "SystemAddressList": "../vendor/upstream/raknet/SystemAddressList.cpp",
    "TableSerializer": "../vendor/upstream/raknet/TableSerializer.cpp",
    "TCPInterface": "../vendor/upstream/raknet/TCPInterface.cpp",
    "TelnetTransport": "../vendor/upstream/raknet/TelnetTransport.cpp",
    "ConsoleServer": "../vendor/upstream/raknet/ConsoleServer.cpp",
    "CommandParserInterface": "../vendor/upstream/raknet/CommandParserInterface.cpp",
    "RakNetCommandParser": "../vendor/upstream/raknet/RakNetCommandParser.cpp",
    "LogCommandParser": "../vendor/upstream/raknet/LogCommandParser.cpp",
    "PacketLogger": "../vendor/upstream/raknet/PacketLogger.cpp",
    "PacketConsoleLogger": "../vendor/upstream/raknet/PacketConsoleLogger.cpp",
    "PacketFileLogger": "../vendor/upstream/raknet/PacketFileLogger.cpp",
    "FullyConnectedMesh": "../vendor/upstream/raknet/FullyConnectedMesh.cpp",
    "DirectoryDeltaTransfer": "../vendor/upstream/raknet/DirectoryDeltaTransfer.cpp",
    "FileList": "../vendor/upstream/raknet/FileList.cpp",
    "FileListTransfer": "../vendor/upstream/raknet/FileListTransfer.cpp",
    "DataCompressor": "../vendor/upstream/raknet/DataCompressor.cpp",
    "CheckSum": "../vendor/upstream/raknet/CheckSum.cpp",
    "InternalPacketPool": "../vendor/upstream/raknet/InternalPacketPool.cpp",
    "NetworkIDGenerator": "../vendor/upstream/raknet/NetworkIDGenerator.cpp",
    "PluginInterface": "../vendor/upstream/raknet/PluginInterface.cpp",
    "RakNetworkFactory": "../vendor/upstream/raknet/RakNetworkFactory.cpp",
    "RakNetTransport": "../vendor/upstream/raknet/RakNetTransport.cpp",
    "ReplicaManager": "../vendor/upstream/raknet/ReplicaManager.cpp",
    "LightweightDatabaseClient": "../vendor/upstream/raknet/LightweightDatabaseClient.cpp",
    "LightweightDatabaseCommon": "../vendor/upstream/raknet/LightweightDatabaseCommon.cpp",
    "LightweightDatabaseServer": "../vendor/upstream/raknet/LightweightDatabaseServer.cpp",
}


def normalize_addr(addr: str | int) -> str:
    """Normalize an address to an 8-digit lowercase hex string (e.g. '100010b0')."""
    if isinstance(addr, int):
        if addr < 0x10000000:
            addr += 0x10000000
        return f"{addr:08x}"
    s = addr.strip().lower()
    if s.startswith("0x"):
        s = s[2:]
    if s.startswith("fun_"):
        s = s[4:]
    val = int(s, 16)
    if val < 0x10000000:
        val += 0x10000000
    return f"{val:08x}"


def normalize_class(cls: str) -> str:
    """Normalize class name to canonical client/saco class name."""
    if not cls:
        return ""
    clean = cls.strip().split()[-1]
    return CLASS_ALIASES.get(clean, clean)


def demangle_msvc_symbol(sym: str, full_name: str = "", raw_class: str = "", raw_name: str = "") -> tuple[str, str]:
    """Extract (canonical_class, method_or_func_name) from MSVC mangled symbol or metadata."""
    sym = (sym or "").strip()
    full_name = (full_name or "").strip()

    # 1. Constructor: ??0ClassName@@...
    if sym.startswith("??0"):
        m = re.match(r"^\?\?0([A-Za-z0-9_]+)@", sym)
        if m:
            cls = normalize_class(m.group(1))
            return cls, cls
    # 2. Destructor: ??1ClassName@@...
    if sym.startswith("??1"):
        m = re.match(r"^\?\?1([A-Za-z0-9_]+)@", sym)
        if m:
            cls = normalize_class(m.group(1))
            return cls, f"~{cls}"
    # 3. Scalar/vector deleting destructor: ??_GClassName@@ or ??_EClassName@@
    if sym.startswith(("??_G", "??_E")):
        m = re.match(r"^\?\?_[GE]([A-Za-z0-9_]+)@", sym)
        if m:
            cls = normalize_class(m.group(1))
            return cls, f"~{cls}_deleting"
    # 4. Regular C++ method or global: ?Method@Class@@... or ?Func@@Y...
    if sym.startswith("?"):
        m = re.match(r"^\?([A-Za-z0-9_]+)@([A-Za-z0-9_]+)@@", sym)
        if m:
            return normalize_class(m.group(2)), m.group(1)
        m2 = re.match(r"^\?([A-Za-z0-9_]+)@@", sym)
        if m2:
            return "", m2.group(1)
    # 5. C/stdcall/fastcall symbol: _Func@12 or @Func@8
    if sym.startswith(("_", "@")):
        m = re.match(r"^[_@]([A-Za-z0-9_]+?)(?:@\d+)?$", sym)
        if m:
            return "", m.group(1)

    # 6. Check full_name like "void CChatWindow::Log" or "PlayerPool.GetSlotState"
    if "::" in full_name and not full_name.startswith("?"):
        parts = full_name.split("::", 1)
        cls = normalize_class(parts[0])
        meth = parts[1].split("(")[0].strip()
        return cls, meth
    if "." in full_name and not full_name.startswith("?"):
        parts = full_name.split(".", 1)
        cls = normalize_class(parts[0])
        meth = parts[1].split("(")[0].strip()
        if meth == "ctor":
            meth = cls
        elif meth == "dtor":
            meth = f"~{cls}"
        return cls, meth

    cls = normalize_class(raw_class)
    meth = raw_name.strip()
    if meth == "ctor" and cls:
        meth = cls
    elif meth == "dtor" and cls:
        meth = f"~{cls}"
    return cls, meth


@dataclasses.dataclass
class R5FunctionInfo:
    address: str  # 8-char lowercase hex e.g. '100010b0'
    name: str
    class_name: str
    method_name: str
    symbol: str
    num_callers: int = 0
    num_callees: int = 0
    in_index: bool = True

    @property
    def qualified_name(self) -> str:
        if self.class_name and self.method_name:
            return f"{self.class_name}::{self.method_name}"
        return self.method_name or self.name


@dataclasses.dataclass
class SourceFunctionDef:
    file_path: Path
    rel_path: str
    class_name: str
    method_name: str
    line_number: int  # 1-indexed line of signature start
    end_line: int
    r5_address: str | None  # 8-char lowercase hex if '// R5: 0x...' precedes it
    code: str

    @property
    def qualified_name(self) -> str:
        if self.class_name:
            return f"{self.class_name}::{self.method_name}"
        return self.method_name


def load_r5_database() -> dict[str, R5FunctionInfo]:
    """Load and merge `_index.json` and `address_map.json` from `ghidra_export/`."""
    idx_data: dict[str, Any] = {}
    am_data: dict[str, Any] = {}

    if INDEX_PATH.exists():
        idx_data = json.loads(INDEX_PATH.read_text(encoding="utf-8"))
    if ADDRESS_MAP_PATH.exists():
        am_data = json.loads(ADDRESS_MAP_PATH.read_text(encoding="utf-8"))

    all_addrs = sorted(set(idx_data.keys()) | set(am_data.keys()))
    db: dict[str, R5FunctionInfo] = {}

    for addr in all_addrs:
        idx_entry = idx_data.get(addr, {})
        am_entry = am_data.get(addr, {})
        sym = am_entry.get("symbol", "")
        full_name = am_entry.get("full_name", "") or idx_entry.get("name", "")
        raw_cls = am_entry.get("class", "")
        raw_name = am_entry.get("name", "") or idx_entry.get("name", f"FUN_{addr}")

        cls, meth = demangle_msvc_symbol(sym, full_name, raw_cls, raw_name)
        # If index has a non-FUN_ name and address_map had nothing, parse index name
        idx_name = idx_entry.get("name", "")
        if (not meth or meth.startswith("FUN_")) and idx_name and not idx_name.startswith("FUN_"):
            cls2, meth2 = demangle_msvc_symbol(idx_name, idx_name, "", idx_name)
            if meth2:
                cls, meth = cls2, meth2

        db[addr] = R5FunctionInfo(
            address=addr,
            name=raw_name,
            class_name=cls,
            method_name=meth,
            symbol=sym,
            num_callers=idx_entry.get("num_callers", 0),
            num_callees=idx_entry.get("num_callees", 0),
            in_index=addr in idx_data,
        )
    return db


_COMMENT_STRING_RE = re.compile(
    r'//[^\r\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
    re.MULTILINE,
)

# Matches top-level C++ method or function definitions
_FUNC_HEADER_RE = re.compile(
    r"(?m)^(?P<indent>[ \t]*)"
    r"(?!if\b|for\b|while\b|switch\b|catch\b|else\b|return\b|sizeof\b|typedef\b|struct\b|class\b|enum\b|union\b|namespace\b)"
    r"(?P<prefix>(?:[A-Za-z_][\w:<>\*\s,&]*?\s+|\b))?"
    r"(?:(?P<cls>[A-Za-z_]\w*)::)?(?P<meth>~?[A-Za-z_]\w*)\s*"
    r"\((?P<args>[^;{}]*)\)\s*(?:const\s*)?\{",
)

_R5_ANNOT_RE = re.compile(r"//\s*R5:\s*0x([0-9A-Fa-f]{6,8})", re.IGNORECASE)


def extract_source_functions(file_path: Path, base_dir: Path = ROOT) -> list[SourceFunctionDef]:
    """Parse a C++ source file and extract all function definitions and `// R5:` annotations."""
    raw_bytes = file_path.read_bytes()
    try:
        source = raw_bytes.decode("utf-8")
    except UnicodeDecodeError:
        source = raw_bytes.decode("latin1")

    # Mask comments & string literals with spaces (preserving newlines and character offsets)
    def _mask(m: re.Match[str]) -> str:
        s = m.group(0)
        return "".join("\n" if c == "\n" else " " for c in s)

    masked = _COMMENT_STRING_RE.sub(_mask, source)

    # Precompute line start offsets
    line_starts = [0]
    for i, ch in enumerate(source):
        if ch == "\n":
            line_starts.append(i + 1)

    def offset_to_line(offset: int) -> int:
        # 1-indexed line number
        lo, hi = 0, len(line_starts) - 1
        while lo <= hi:
            mid = (lo + hi) // 2
            if line_starts[mid] <= offset:
                lo = mid + 1
            else:
                hi = mid - 1
        return hi + 1

    lines = source.splitlines()
    results: list[SourceFunctionDef] = []

    for m in _FUNC_HEADER_RE.finditer(masked):
        meth = m.group("meth")
        if meth in {"if", "for", "while", "switch", "catch", "asm", "__asm"}:
            continue
        raw_cls = m.group("cls") or ""
        cls = normalize_class(raw_cls)
        if raw_cls and meth == raw_cls:
            meth = cls
        elif raw_cls and meth == f"~{raw_cls}":
            meth = f"~{cls}"

        start_pos = m.start()
        brace_open = m.end() - 1
        depth = 1
        at = brace_open + 1
        n = len(masked)
        while depth > 0 and at < n:
            ch = masked[at]
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
            at += 1
        if depth != 0:
            continue

        start_line = offset_to_line(start_pos)
        end_line = offset_to_line(at - 1)

        # Check up to 5 preceding lines for '// R5: 0x...'
        r5_addr: str | None = None
        lookback_start = max(0, start_line - 6)
        for ln_idx in range(start_line - 2, lookback_start - 1, -1):
            if 0 <= ln_idx < len(lines):
                line_str = lines[ln_idx].strip()
                am = _R5_ANNOT_RE.search(line_str)
                if am:
                    r5_addr = normalize_addr(am.group(1))
                    break
                # Stop lookback if we hit another function's closing brace
                if line_str.endswith("}") or (line_str and not line_str.startswith("//") and not line_str.startswith("/*") and not line_str.startswith("*")):
                    break

        try:
            rel_path = str(file_path.relative_to(base_dir)).replace("\\", "/")
        except ValueError:
            rel_path = str(file_path)

        results.append(
            SourceFunctionDef(
                file_path=file_path,
                rel_path=rel_path,
                class_name=cls,
                method_name=meth,
                line_number=start_line,
                end_line=end_line,
                r5_address=r5_addr,
                code=source[start_pos:at],
            )
        )

    return results


def scan_tree_functions(directory: Path, exclude_dirs: set[str] | None = None) -> list[SourceFunctionDef]:
    """Scan all `.cpp` files in `directory` and return extracted function definitions."""
    exclude_dirs = exclude_dirs or set()
    defs: list[SourceFunctionDef] = []
    for path in sorted(directory.rglob("*.cpp")):
        if any(part in exclude_dirs for part in path.parts):
            continue
        defs.extend(extract_source_functions(path))
    return defs
