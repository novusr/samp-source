# Status & Progress Rekonstruksi SA-MP 0.3.7 R5

> Di-generate otomatis oleh `python3 tools/progress.py --markdown` pada **2026-10-23 21:12 UTC**.

## 1. Ringkasan Menuju 100%

| Metrik Target | Progress Bar | Angka | Persentase |
| :--- | :--- | ---: | ---: |
| **Unit Kompilasi MSVC 2003 (`tools/build.py`)** | `████████████████████` | **150 / 151 OK** | **99.3%** |
| **Fungsi R5 Teridentifikasi (Named Symbols)** | `██████░░░░░░░░░░░░░░` | **1209 / 3739** | **32.3%** |
| **Seluruh Fungsi Binary R5 (`_index.json` termasuk `FUN_`)** | `█████░░░░░░░░░░░░░░░` | **1209 / 5265** | **22.96%** |
| **Cakupan + Siap Merge dari `_reference/`** | `█████░░░░░░░░░░░░░░░` | **1427 / 5265** | **27.10%** |

### Rincian Status Fungsi R5

| Status | Keterangan | Jumlah (`_index.json`) | Jumlah (Semua Simbol) |
| :--- | :--- | ---: | ---: |
| `ANNOTATED` | Sudah ada di `client/saco/` dengan anotasi `// R5: 0x...` | **553** | 681 |
| `IN_SOURCE` | Sudah ada di `client/saco/` (cocok `Class::Method`) | **14** | 17 |
| `IN_VENDOR` | Sudah ada di `vendor/upstream/` (DXUT / RakNet) | **642** | 906 |
| `IN_REFERENCE` | Tersedia di `client/saco/_reference/` (392 file tersisa) | **218** | 249 |
| `NAMED_TODO` | Nama fungsi/class sudah diketahui di Ghidra, belum ada di source | **2312** | 2451 |
| `UNNAMED_TODO` | Fungsi `FUN_10xxxxxx` belum bernama / helper internal / CRT | **1526** | 3774 |
| **TOTAL** | | **5265** | **8078** |

---

## 2. Progress per Subsystem / Modul

| Modul | Progress | Selesai (`Done`) | Di `_reference/` | Belum (`Todo`) | Total Fungsi Class |
| :--- | :--- | ---: | ---: | ---: | ---: |
| **game** | `██████████████░` ** 92.3%** | 286 | 0 | 24 | 310 |
| **net** | `██████████████░` ** 94.1%** | 48 | 0 | 3 | 51 |
| **root (UI/Core)** | `███████████████` ** 97.9%** | 92 | 0 | 2 | 94 |
| **archive** | `███████████████` **100.0%** | 1 | 0 | 0 | 1 |
| **d3dhook** | `███████████████` **100.0%** | 19 | 0 | 0 | 19 |
| **d3d9 (DXUT)** | `███████░░░░░░░░` ** 48.6%** | 277 | 1 | 292 | 570 |
| **raknet** | `██████████████░` ** 93.6%** | 412 | 0 | 28 | 440 |
| **other** | `██░░░░░░░░░░░░░` ** 11.3%** | 58 | 79 | 377 | 514 |

---

## 3. Progress Lengkap per Class (Menuju 100%)

| Status | Modul | Class | Progress | Done / Total | Annot | InSrc | InRef | Todo | File Target |
| :---: | :--- | :--- | :--- | ---: | ---: | ---: | ---: | ---: | :--- |
| ✅ | game | `CMenu` | `██████████` 100.0% | **11/11** | 11 | 0 | 0 | 0 | `game/menu.cpp` |
| 🔄 | game | `CPlayerPed` | `██████████`  98.8% | **82/83** | 81 | 1 | 0 | 1 | `game/playerped.cpp` |
| 🔄 | game | `CActorPed` | `█████████░`  94.4% | **17/18** | 17 | 0 | 0 | 1 | `game/actorped.cpp` |
| 🔄 | game | `CVehicle` | `█████████░`  94.1% | **32/34** | 32 | 0 | 0 | 2 | `game/vehicle.cpp` |
| 🔄 | game | `CGame` | `█████████░`  93.9% | **62/66** | 62 | 0 | 0 | 4 | `game/game.cpp` |
| 🔄 | game | `CEntity` | `█████████░`  93.5% | **43/46** | 43 | 0 | 0 | 3 | `game/entity.cpp` |
| 🔄 | game | `CCamera` | `█████████░`  92.9% | **13/14** | 13 | 0 | 0 | 1 | `game/camera.cpp` |
| 🔄 | game | `CTask` | `█████████░`  92.3% | **12/13** | 7 | 5 | 0 | 1 | `game/task.cpp` |
| 🔄 | game | `CAudio` | `████████░░`  77.8% | **7/9** | 7 | 0 | 0 | 2 | `game/audio.cpp` |
| 🔄 | game | `CTaskJetpack` | `████████░░`  75.0% | **3/4** | 1 | 2 | 0 | 1 | `game/task.cpp` |
| 🔄 | game | `CTaskGoToPoint` | `███░░░░░░░`  33.3% | **1/3** | 1 | 0 | 0 | 2 | `game/task.cpp` |
| 🔄 | game | `CTaskGoggles` | `███░░░░░░░`  33.3% | **1/3** | 1 | 0 | 0 | 2 | `game/task.cpp` |
| 🔄 | game | `CTaskKillPedOnFootArmed` | `███░░░░░░░`  33.3% | **1/3** | 1 | 0 | 0 | 2 | `game/task.cpp` |
| 🔄 | game | `CTaskTakeDamageFall` | `███░░░░░░░`  33.3% | **1/3** | 1 | 0 | 0 | 2 | `game/task.cpp` |
| ✅ | net | `CActorPool` | `██████████` 100.0% | **6/6** | 6 | 0 | 0 | 0 | `net/actorpool.cpp` |
| ✅ | net | `CGangZonePool` | `██████████` 100.0% | **7/7** | 7 | 0 | 0 | 0 | `net/gangzonepool.cpp` |
| ✅ | net | `CLabelPool` | `██████████` 100.0% | **3/3** | 3 | 0 | 0 | 0 | `net/labelpool.cpp` |
| ✅ | net | `CMenuPool` | `██████████` 100.0% | **8/8** | 8 | 0 | 0 | 0 | `net/menupool.cpp` |
| ✅ | net | `CNetGame` | `██████████` 100.0% | **4/4** | 4 | 0 | 0 | 0 | `net/netgame.cpp` |
| ✅ | net | `CNetPlayer` | `██████████` 100.0% | **2/2** | 2 | 0 | 0 | 0 | `net/netplayer.cpp` |
| ✅ | net | `CObjectPool` | `██████████` 100.0% | **3/3** | 3 | 0 | 0 | 0 | `net/objectpool.cpp` |
| ✅ | net | `CPickupPool` | `██████████` 100.0% | **3/3** | 3 | 0 | 0 | 0 | `net/pickuppool.cpp` |
| ✅ | net | `CRemotePlayer` | `██████████` 100.0% | **5/5** | 5 | 0 | 0 | 0 | `net/remoteplayer.cpp` |
| 🔄 | net | `CLocalPlayer` | `████████░░`  83.3% | **5/6** | 5 | 0 | 0 | 1 | `net/localplayer.cpp` |
| 🔄 | net | `CPlayerPool` | `█████░░░░░`  50.0% | **2/4** | 2 | 0 | 0 | 2 | `net/playerpool.cpp` |
| ✅ | root (UI/Core) | `CChatWindow` | `██████████` 100.0% | **4/4** | 4 | 0 | 0 | 0 | `chatwindow.cpp` |
| ✅ | root (UI/Core) | `CCmdWindow` | `██████████` 100.0% | **13/13** | 13 | 0 | 0 | 0 | `cmdwindow.cpp` |
| ✅ | root (UI/Core) | `CDeathWindow` | `██████████` 100.0% | **12/12** | 12 | 0 | 0 | 0 | `deathwindow.cpp` |
| ✅ | root (UI/Core) | `CFontRender` | `██████████` 100.0% | **10/10** | 7 | 3 | 0 | 0 | `fontrender.cpp` |
| ✅ | root (UI/Core) | `CHttpClient` | `██████████` 100.0% | **11/11** | 11 | 0 | 0 | 0 | `httpclient.cpp` |
| ✅ | root (UI/Core) | `CNewPlayerTags` | `██████████` 100.0% | **7/7** | 7 | 0 | 0 | 0 | `newplayertags.cpp` |
| ✅ | root (UI/Core) | `CPlayerTags` | `██████████` 100.0% | **8/8** | 8 | 0 | 0 | 0 | `playertags.cpp` |
| ✅ | root (UI/Core) | `CScoreBoard` | `██████████` 100.0% | **2/2** | 2 | 0 | 0 | 0 | `scoreboard.cpp` |
| ✅ | root (UI/Core) | `CSpawnScreen` | `██████████` 100.0% | **6/6** | 6 | 0 | 0 | 0 | `spawnscreen.cpp` |
| ✅ | root (UI/Core) | `CUnkClass4` | `██████████` 100.0% | **2/2** | 2 | 0 | 0 | 0 | `unkclass4.cpp` |
| 🔄 | root (UI/Core) | `CConfig` | `█████████░`  89.5% | **17/19** | 17 | 0 | 0 | 2 | `config.cpp` |
| ✅ | archive | `CCryptoContext` | `██████████` 100.0% | **1/1** | 1 | 0 | 0 | 0 | `archive/CryptoContext.cpp` |
| ✅ | d3dhook | `ID3DXFontHook` | `██████████` 100.0% | **18/18** | 18 | 0 | 0 | 0 | `d3dhook/ID3DXFontHook.cpp` |
| ✅ | d3dhook | `IDirect3DDevice9Hook` | `██████████` 100.0% | **1/1** | 1 | 0 | 0 | 0 | `d3dhook/IDirect3DDevice9Hook.cpp` |
| ✅ | d3d9 (DXUT) | `CD3DEnumAdapterInfo` | `██████████` 100.0% | **1/1** | 0 | 1 | 0 | 0 | `d3d9/common/DXUTenum.cpp` |
| ✅ | d3d9 (DXUT) | `CD3DEnumDeviceInfo` | `██████████` 100.0% | **1/1** | 0 | 1 | 0 | 0 | `d3d9/common/DXUTenum.cpp` |
| ✅ | d3d9 (DXUT) | `CD3DEnumeration` | `██████████` 100.0% | **19/19** | 0 | 19 | 0 | 0 | `d3d9/common/DXUTenum.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTDirectionWidget` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTEditBox` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | `d3d9/common/DXUTgui.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTElement` | `██████████` 100.0% | **3/3** | 0 | 3 | 0 | 0 | `d3d9/common/DXUTgui.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTTextHelper` | `██████████` 100.0% | **7/7** | 0 | 7 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTTimer` | `██████████` 100.0% | **9/9** | 0 | 9 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| ✅ | d3d9 (DXUT) | `CFirstPersonCamera` | `██████████` 100.0% | **3/3** | 0 | 3 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTDialog` | `██████████`  95.1% | **39/41** | 0 | 39 | 0 | 2 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTScrollBar` | `█████████░`  91.7% | **11/12** | 0 | 11 | 0 | 1 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTResourceCache` | `█████████░`  90.5% | **19/21** | 0 | 19 | 0 | 2 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTDialogResourceManager` | `█████████░`  90.0% | **9/10** | 0 | 9 | 0 | 1 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTListBox` | `█████████░`  88.9% | **8/9** | 0 | 8 | 0 | 1 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTComboBox` | `█████████░`  87.5% | **21/24** | 0 | 21 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CBaseCamera` | `████████░░`  81.8% | **9/11** | 0 | 9 | 0 | 2 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTSlider` | `████████░░`  75.0% | **9/12** | 0 | 9 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CModelViewerCamera` | `████████░░`  75.0% | **6/8** | 0 | 6 | 0 | 2 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTLineManager` | `███████░░░`  73.3% | **11/15** | 0 | 11 | 0 | 4 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTCheckBox` | `███████░░░`  70.0% | **7/10** | 0 | 7 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CUniBuffer` | `██████░░░░`  64.0% | **16/25** | 0 | 16 | 1 | 8 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CD3DArcBall` | `██████░░░░`  61.5% | **8/13** | 0 | 8 | 0 | 5 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTRadioButton` | `██████░░░░`  57.1% | **4/7** | 0 | 4 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTStatic` | `█████░░░░░`  50.0% | **3/6** | 0 | 3 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTButton` | `████░░░░░░`  44.4% | **4/9** | 0 | 4 | 0 | 5 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTIMEEditBox` | `████░░░░░░`  44.0% | **22/50** | 0 | 22 | 0 | 28 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTControl` | `███░░░░░░░`  26.1% | **6/23** | 0 | 6 | 0 | 17 | `d3d9/common/DXUTgui.cpp` |
| ⏳ | d3d9 (DXUT) | `DXUTState` | `░░░░░░░░░░`   0.0% | **0/199** | 0 | 0 | 0 | 199 | `d3d9/common/DXUT.cpp` |
| ✅ | raknet | `CheckSum` | `██████████` 100.0% | **4/4** | 0 | 4 | 0 | 0 | `../vendor/upstream/raknet/CheckSum.cpp` |
| ✅ | raknet | `ConnectionGraph` | `██████████` 100.0% | **32/32** | 0 | 32 | 0 | 0 | `../vendor/upstream/raknet/ConnectionGraph.cpp` |
| ✅ | raknet | `ConsoleServer` | `██████████` 100.0% | **6/6** | 0 | 6 | 0 | 0 | `../vendor/upstream/raknet/ConsoleServer.cpp` |
| ✅ | raknet | `DataBlockEncryptor` | `██████████` 100.0% | **5/5** | 0 | 5 | 0 | 0 | `../vendor/upstream/raknet/DataBlockEncryptor.cpp` |
| ✅ | raknet | `DataCompressor` | `██████████` 100.0% | **2/2** | 0 | 2 | 0 | 0 | `../vendor/upstream/raknet/DataCompressor.cpp` |
| ✅ | raknet | `DirectoryDeltaTransfer` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | `../vendor/upstream/raknet/DirectoryDeltaTransfer.cpp` |
| ✅ | raknet | `FileList` | `██████████` 100.0% | **13/13** | 0 | 13 | 0 | 0 | `../vendor/upstream/raknet/FileList.cpp` |
| ✅ | raknet | `LightweightDatabaseClient` | `██████████` 100.0% | **7/7** | 0 | 7 | 0 | 0 | `../vendor/upstream/raknet/LightweightDatabaseClient.cpp` |
| ✅ | raknet | `RakClient` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | `../vendor/upstream/raknet/RakClient.cpp` |
| ✅ | raknet | `RakServer` | `██████████` 100.0% | **1/1** | 0 | 1 | 0 | 0 | `../vendor/upstream/raknet/RakServer.cpp` |
| ✅ | raknet | `Router` | `██████████` 100.0% | **9/9** | 0 | 9 | 0 | 0 | `../vendor/upstream/raknet/Router.cpp` |
| ✅ | raknet | `SimpleMutex` | `██████████` 100.0% | **2/2** | 0 | 2 | 0 | 0 | `../vendor/upstream/raknet/SimpleMutex.cpp` |
| ✅ | raknet | `SocketLayer` | `██████████` 100.0% | **4/4** | 0 | 4 | 0 | 0 | `../vendor/upstream/raknet/SocketLayer.cpp` |
| ✅ | raknet | `StringCompressor` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | `../vendor/upstream/raknet/StringCompressor.cpp` |
| ✅ | raknet | `StringTable` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | `../vendor/upstream/raknet/StringTable.cpp` |
| ✅ | raknet | `SystemAddressList` | `██████████` 100.0% | **7/7** | 0 | 7 | 0 | 0 | `../vendor/upstream/raknet/SystemAddressList.cpp` |
| ✅ | raknet | `TCPInterface` | `██████████` 100.0% | **12/12** | 0 | 12 | 0 | 0 | `../vendor/upstream/raknet/TCPInterface.cpp` |
| ✅ | raknet | `TableSerializer` | `██████████` 100.0% | **6/6** | 0 | 6 | 0 | 0 | `../vendor/upstream/raknet/TableSerializer.cpp` |
| ✅ | raknet | `TelnetTransport` | `██████████` 100.0% | **13/13** | 0 | 13 | 0 | 0 | `../vendor/upstream/raknet/TelnetTransport.cpp` |
| 🔄 | raknet | `ReliabilityLayer` | `██████████`  96.9% | **31/32** | 0 | 31 | 0 | 1 | `../vendor/upstream/raknet/ReliabilityLayer.cpp` |
| 🔄 | raknet | `LightweightDatabaseServer` | `█████████░`  94.4% | **17/18** | 0 | 17 | 0 | 1 | `../vendor/upstream/raknet/LightweightDatabaseServer.cpp` |
| 🔄 | raknet | `CommandParserInterface` | `█████████░`  93.8% | **15/16** | 0 | 15 | 0 | 1 | `../vendor/upstream/raknet/CommandParserInterface.cpp` |
| 🔄 | raknet | `NetworkIDGenerator` | `█████████░`  92.9% | **13/14** | 0 | 13 | 0 | 1 | `../vendor/upstream/raknet/NetworkIDGenerator.cpp` |
| 🔄 | raknet | `PacketLogger` | `█████████░`  92.9% | **13/14** | 0 | 13 | 0 | 1 | `../vendor/upstream/raknet/PacketLogger.cpp` |
| 🔄 | raknet | `RakPeer` | `█████████░`  92.6% | **63/68** | 0 | 63 | 0 | 5 | `../vendor/upstream/raknet/RakPeer.cpp` |
| 🔄 | raknet | `PluginInterface` | `█████████░`  90.9% | **10/11** | 0 | 10 | 0 | 1 | `../vendor/upstream/raknet/PluginInterface.cpp` |
| 🔄 | raknet | `LogCommandParser` | `█████████░`  86.7% | **13/15** | 0 | 13 | 0 | 2 | `../vendor/upstream/raknet/LogCommandParser.cpp` |
| 🔄 | raknet | `ReplicaManager` | `█████████░`  86.5% | **32/37** | 0 | 32 | 0 | 5 | `../vendor/upstream/raknet/ReplicaManager.cpp` |
| 🔄 | raknet | `FileListTransfer` | `█████████░`  85.7% | **12/14** | 0 | 12 | 0 | 2 | `../vendor/upstream/raknet/FileListTransfer.cpp` |
| 🔄 | raknet | `FullyConnectedMesh` | `█████████░`  85.7% | **6/7** | 0 | 6 | 0 | 1 | `../vendor/upstream/raknet/FullyConnectedMesh.cpp` |
| 🔄 | raknet | `RPCMap` | `████████░░`  83.3% | **5/6** | 0 | 5 | 0 | 1 | `../vendor/upstream/raknet/RPCMap.cpp` |
| 🔄 | raknet | `InternalPacketPool` | `████████░░`  80.0% | **4/5** | 0 | 4 | 0 | 1 | `../vendor/upstream/raknet/InternalPacketPool.cpp` |
| 🔄 | raknet | `PacketFileLogger` | `████████░░`  75.0% | **3/4** | 0 | 3 | 0 | 1 | `../vendor/upstream/raknet/PacketFileLogger.cpp` |
| 🔄 | raknet | `RakNetCommandParser` | `███████░░░`  71.4% | **5/7** | 0 | 5 | 0 | 2 | `../vendor/upstream/raknet/RakNetCommandParser.cpp` |
| 🔄 | raknet | `PacketConsoleLogger` | `██████░░░░`  60.0% | **3/5** | 0 | 3 | 0 | 2 | `../vendor/upstream/raknet/PacketConsoleLogger.cpp` |
| ✅ | other | `BitStream` | `██████████` 100.0% | **5/5** | 0 | 5 | 0 | 0 | - |
| ✅ | other | `HuffmanEncodingTree` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | - |
| ✅ | other | `NatPunchthrough` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | - |
| ✅ | other | `R5TextDrawPool` | `██████████` 100.0% | **4/4** | 4 | 0 | 0 | 0 | - |
| ✅ | other | `TEABlockEncryptor` | `██████████` 100.0% | **9/9** | 0 | 9 | 0 | 0 | - |
| 🔄 | other | `CSHA1` | `█████████░`  90.9% | **10/11** | 0 | 10 | 0 | 1 | - |
| 🔄 | other | `DatabaseCellUpdate` | `█████░░░░░`  50.0% | **2/4** | 0 | 2 | 0 | 2 | - |
| 🔄 | other | `DatabaseFilter` | `█████░░░░░`  50.0% | **2/4** | 0 | 2 | 0 | 2 | - |
| 🔄 | other | `RakNet` | `█████░░░░░`  50.0% | **2/4** | 0 | 2 | 0 | 2 | - |
| 🔄 | other | `R5ChatPagingView` | `████░░░░░░`  40.0% | **2/5** | 2 | 0 | 3 | 0 | - |
| ⏳ | other | `*>` | `░░░░░░░░░░`   0.0% | **0/28** | 0 | 0 | 0 | 28 | - |
| ⏳ | other | `CGrowableArray<int>` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| 🔄 | other | `CPickupPoolMore` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 4 | 0 | - |
| ⏳ | other | `Cell` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `ColumnDescriptor` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `DXUTCache_Effect>` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `DXUTCache_Font>` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `DXUTCache_Texture>` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `DataStructures` | `░░░░░░░░░░`   0.0% | **0/231** | 0 | 0 | 0 | 231 | - |
| ⏳ | other | `DatabaseTable` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `MapNode` | `░░░░░░░░░░`   0.0% | **0/9** | 0 | 0 | 0 | 9 | - |
| 🔄 | other | `R5CustomModelManagerView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 3 | 0 | - |
| 🔄 | other | `R5FontSpriteProxy` | `░░░░░░░░░░`   0.0% | **0/15** | 0 | 0 | 14 | 1 | - |
| 🔄 | other | `R5MemoryBufferView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 3 | 0 | - |
| 🔄 | other | `R5MemoryReaderView` | `░░░░░░░░░░`   0.0% | **0/8** | 0 | 0 | 7 | 1 | - |
| 🔄 | other | `R5ObjectDestructionView` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 3 | 1 | - |
| 🔄 | other | `R5ObjectEditorView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 3 | 0 | - |
| 🔄 | other | `R5RwClumpDispatchView` | `░░░░░░░░░░`   0.0% | **0/6** | 0 | 0 | 6 | 0 | - |
| 🔄 | other | `R5RwFrameParserView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 1 | 2 | - |
| 🔄 | other | `R5ScoreboardActionsView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 3 | 0 | - |
| 🔄 | other | `R5ScoreboardPlayerQueriesView` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 4 | 0 | - |
| 🔄 | other | `R5SpatialLabelRenderer` | `░░░░░░░░░░`   0.0% | **0/8** | 0 | 0 | 8 | 0 | - |
| 🔄 | other | `R5TextDrawClickView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 3 | 0 | - |
| 🔄 | other | `R5TextDrawConstructionView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 3 | 0 | - |
| 🔄 | other | `R5TextDrawSelectorView` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 3 | 0 | - |
| 🔄 | other | `R5VehiclePlateRenderer` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 4 | 0 | - |
| 🔄 | other | `RPCObjectOpsView` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 4 | 0 | - |
| ⏳ | other | `_D3DDISPLAYMODE>` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 0 | 4 | - |
| ⏳ | other | `_D3DFORMAT>` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `big` | `░░░░░░░░░░`   0.0% | **0/61** | 0 | 0 | 0 | 61 | - |
| ⏳ | other | `int>` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
| ⏳ | other | `std` | `░░░░░░░░░░`   0.0% | **0/5** | 0 | 0 | 0 | 5 | - |

---

## 4. Cara Memperbarui Dokumen Ini

Jalankan perintah berikut kapan saja untuk memperbarui anotasi `// R5:` dan me-regenerate laporan ini:

```bash
python3 tools/progress.py --annotate --apply
python3 tools/progress.py --markdown
```
