# Status & Progress Rekonstruksi SA-MP 0.3.7 R5

> Di-generate otomatis oleh `python3 tools/progress.py --markdown` pada **2026-10-23 22:36 UTC**.

## 🎯 PROGRES KESELURUHAN PROYEK (MENUJU 100%)

### `████████████████████████░░░░░░` **79.0% / 100.0%**

| Pilar Penyelesaian Proyek | Progress Bar | Capaian Saat Ini | Persentase | Target 100% |
| :--- | :--- | ---: | ---: | :--- |
| **1. Rekonstruksi Class & Modul Utama (`saco` + `raknet` + `d3d9`)** | `████████████████░░░░` | **1196 / 1503 fungsi** | **79.6%** | Seluruh method class terimplementasi |
| **2. Cakupan Seluruh Fungsi Binary R5 (`_index.json` 5.265 fungsi)** | `█████████████░░░░░░░` | **3390 / 5265 fungsi** | **64.4%** | Seluruh fungsi R5 (termasuk helper/`FUN_`) terpetakan |
| **3. Kompilasi Unit MSVC 2003 (`tools/build.py`)** | `████████████████████` | **150 / 151 unit `.cpp`** | **99.3%** | 151/151 unit `.cpp` lolos compile & link ke `samp.dll` |
| **4. Pembersihan Fragmen `_reference/`** | `█████░░░░░░░░░░░░░░░` | **125 / 516 file selesai** | **24.2%** | 0 file tersisa di `_reference/` (tersisa 391 file) |

### Rincian 5.265 Fungsi di Binary `samp.dll` R5 (`_index.json`)

| Kategori | Keterangan | Jumlah (`_index.json`) | Persentase dari 5.265 |
| :--- | :--- | ---: | ---: |
| ✅ `ANNOTATED` | Fungsi di `client/saco/` dengan anotasi `// R5: 0x...` | **600** | 11.4% |
| ✅ `IN_SOURCE` | Fungsi di `client/saco/` (cocok `Class::Method`) | **7** | 0.1% |
| ✅ `IN_VENDOR` | Fungsi di `vendor/upstream/` (DXUT & RakNet) | **684** | 13.0% |
| ⚙️ `CRT_OR_TEMPLATE` | Otomatis dari Compiler MSVC 2003 (`libcmt.lib`, SEH, STL/Template) | **2099** | 39.9% |
| 📦 `IN_REFERENCE` | Sudah ada di `client/saco/_reference/`, tinggal digabung | **212** | 4.0% |
| ⏳ `NAMED_TODO` | Fungsi bernama di Ghidra yang belum masuk source utama | **331** | 6.3% |
| ⏳ `UNNAMED_TODO` | Fungsi `FUN_10xxxxxx` belum bernama di Ghidra | **1332** | 25.3% |
| **TOTAL BINARY R5** | **Seluruh fungsi di `samp.dll` 0.3.7 R5** | **5265** | **100.0%** |

---

## 2. Progress per Subsystem / Modul

| Modul | Progress | Selesai (`Done`) | Di `_reference/` | Belum (`Todo`) | Total Fungsi Class |
| :--- | :--- | ---: | ---: | ---: | ---: |
| **game** | `██████████████░` ** 96.1%** | 298 | 0 | 12 | 310 |
| **net** | `███████████████` **100.0%** | 51 | 0 | 0 | 51 |
| **root (UI/Core)** | `███████████████` **100.0%** | 94 | 0 | 0 | 94 |
| **archive** | `███████████████` **100.0%** | 1 | 0 | 0 | 1 |
| **d3dhook** | `███████████████` **100.0%** | 19 | 0 | 0 | 19 |
| **d3d9 (DXUT)** | `███████░░░░░░░░` ** 49.6%** | 278 | 1 | 282 | 561 |
| **raknet** | `███████████████` ** 97.4%** | 455 | 0 | 12 | 467 |
| **other** | `███████░░░░░░░░` ** 44.2%** | 72 | 78 | 13 | 163 |
| **TOTAL MODUL UTAMA** | `████████████░░░` ** 79.6%** | **1196** | - | **307** | **1503** |

---

## 3. Progress Lengkap per Class (Menuju 100%)

| Status | Modul | Class | Progress | Done / Total | Annot | InSrc | InRef | Todo | File Target |
| :---: | :--- | :--- | :--- | ---: | ---: | ---: | ---: | ---: | :--- |
| ✅ | game | `CCamera` | `██████████` 100.0% | **14/14** | 14 | 0 | 0 | 0 | `game/camera.cpp` |
| ✅ | game | `CGame` | `██████████` 100.0% | **66/66** | 66 | 0 | 0 | 0 | `game/game.cpp` |
| ✅ | game | `CMenu` | `██████████` 100.0% | **11/11** | 11 | 0 | 0 | 0 | `game/menu.cpp` |
| 🔄 | game | `CPlayerPed` | `██████████`  98.8% | **82/83** | 81 | 1 | 0 | 1 | `game/playerped.cpp` |
| 🔄 | game | `CEntity` | `██████████`  97.8% | **45/46** | 45 | 0 | 0 | 1 | `game/entity.cpp` |
| 🔄 | game | `CVehicle` | `██████████`  97.1% | **33/34** | 33 | 0 | 0 | 1 | `game/vehicle.cpp` |
| 🔄 | game | `CActorPed` | `█████████░`  94.4% | **17/18** | 17 | 0 | 0 | 1 | `game/actorped.cpp` |
| 🔄 | game | `CTask` | `█████████░`  92.3% | **12/13** | 12 | 0 | 0 | 1 | `game/task.cpp` |
| 🔄 | game | `CAudio` | `████████░░`  77.8% | **7/9** | 7 | 0 | 0 | 2 | `game/audio.cpp` |
| 🔄 | game | `CTaskJetpack` | `████████░░`  75.0% | **3/4** | 3 | 0 | 0 | 1 | `game/task.cpp` |
| 🔄 | game | `CTaskGoToPoint` | `███████░░░`  66.7% | **2/3** | 2 | 0 | 0 | 1 | `game/task.cpp` |
| 🔄 | game | `CTaskGoggles` | `███████░░░`  66.7% | **2/3** | 2 | 0 | 0 | 1 | `game/task.cpp` |
| 🔄 | game | `CTaskKillPedOnFootArmed` | `███████░░░`  66.7% | **2/3** | 2 | 0 | 0 | 1 | `game/task.cpp` |
| 🔄 | game | `CTaskTakeDamageFall` | `███████░░░`  66.7% | **2/3** | 2 | 0 | 0 | 1 | `game/task.cpp` |
| ✅ | net | `CActorPool` | `██████████` 100.0% | **6/6** | 6 | 0 | 0 | 0 | `net/actorpool.cpp` |
| ✅ | net | `CGangZonePool` | `██████████` 100.0% | **7/7** | 7 | 0 | 0 | 0 | `net/gangzonepool.cpp` |
| ✅ | net | `CLabelPool` | `██████████` 100.0% | **3/3** | 3 | 0 | 0 | 0 | `net/labelpool.cpp` |
| ✅ | net | `CLocalPlayer` | `██████████` 100.0% | **6/6** | 6 | 0 | 0 | 0 | `net/localplayer.cpp` |
| ✅ | net | `CMenuPool` | `██████████` 100.0% | **8/8** | 8 | 0 | 0 | 0 | `net/menupool.cpp` |
| ✅ | net | `CNetGame` | `██████████` 100.0% | **4/4** | 4 | 0 | 0 | 0 | `net/netgame.cpp` |
| ✅ | net | `CNetPlayer` | `██████████` 100.0% | **2/2** | 2 | 0 | 0 | 0 | `net/netplayer.cpp` |
| ✅ | net | `CObjectPool` | `██████████` 100.0% | **3/3** | 3 | 0 | 0 | 0 | `net/objectpool.cpp` |
| ✅ | net | `CPickupPool` | `██████████` 100.0% | **3/3** | 3 | 0 | 0 | 0 | `net/pickuppool.cpp` |
| ✅ | net | `CPlayerPool` | `██████████` 100.0% | **4/4** | 4 | 0 | 0 | 0 | `net/playerpool.cpp` |
| ✅ | net | `CRemotePlayer` | `██████████` 100.0% | **5/5** | 5 | 0 | 0 | 0 | `net/remoteplayer.cpp` |
| ✅ | root (UI/Core) | `CChatWindow` | `██████████` 100.0% | **4/4** | 4 | 0 | 0 | 0 | `chatwindow.cpp` |
| ✅ | root (UI/Core) | `CCmdWindow` | `██████████` 100.0% | **13/13** | 13 | 0 | 0 | 0 | `cmdwindow.cpp` |
| ✅ | root (UI/Core) | `CConfig` | `██████████` 100.0% | **19/19** | 19 | 0 | 0 | 0 | `config.cpp` |
| ✅ | root (UI/Core) | `CDeathWindow` | `██████████` 100.0% | **12/12** | 12 | 0 | 0 | 0 | `deathwindow.cpp` |
| ✅ | root (UI/Core) | `CFontRender` | `██████████` 100.0% | **10/10** | 10 | 0 | 0 | 0 | `fontrender.cpp` |
| ✅ | root (UI/Core) | `CHttpClient` | `██████████` 100.0% | **11/11** | 11 | 0 | 0 | 0 | `httpclient.cpp` |
| ✅ | root (UI/Core) | `CNewPlayerTags` | `██████████` 100.0% | **7/7** | 7 | 0 | 0 | 0 | `newplayertags.cpp` |
| ✅ | root (UI/Core) | `CPlayerTags` | `██████████` 100.0% | **8/8** | 8 | 0 | 0 | 0 | `playertags.cpp` |
| ✅ | root (UI/Core) | `CScoreBoard` | `██████████` 100.0% | **2/2** | 2 | 0 | 0 | 0 | `scoreboard.cpp` |
| ✅ | root (UI/Core) | `CSpawnScreen` | `██████████` 100.0% | **6/6** | 6 | 0 | 0 | 0 | `spawnscreen.cpp` |
| ✅ | root (UI/Core) | `CUnkClass4` | `██████████` 100.0% | **2/2** | 2 | 0 | 0 | 0 | `unkclass4.cpp` |
| ✅ | archive | `CCryptoContext` | `██████████` 100.0% | **1/1** | 1 | 0 | 0 | 0 | `archive/CryptoContext.cpp` |
| ✅ | d3dhook | `ID3DXFontHook` | `██████████` 100.0% | **18/18** | 18 | 0 | 0 | 0 | `d3dhook/ID3DXFontHook.cpp` |
| ✅ | d3dhook | `IDirect3DDevice9Hook` | `██████████` 100.0% | **1/1** | 1 | 0 | 0 | 0 | `d3dhook/IDirect3DDevice9Hook.cpp` |
| ✅ | d3d9 (DXUT) | `CD3DEnumAdapterInfo` | `██████████` 100.0% | **1/1** | 0 | 1 | 0 | 0 | `d3d9/common/DXUTenum.cpp` |
| ✅ | d3d9 (DXUT) | `CD3DEnumDeviceInfo` | `██████████` 100.0% | **1/1** | 0 | 1 | 0 | 0 | `d3d9/common/DXUTenum.cpp` |
| ✅ | d3d9 (DXUT) | `CD3DEnumeration` | `██████████` 100.0% | **19/19** | 0 | 19 | 0 | 0 | `d3d9/common/DXUTenum.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTDirectionWidget` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTEditBox` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | `d3d9/common/DXUTgui.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTElement` | `██████████` 100.0% | **3/3** | 0 | 3 | 0 | 0 | `d3d9/common/DXUTgui.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTLineManager` | `██████████` 100.0% | **11/11** | 0 | 11 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTListBox` | `██████████` 100.0% | **9/9** | 0 | 9 | 0 | 0 | `d3d9/common/DXUTgui.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTTextHelper` | `██████████` 100.0% | **7/7** | 0 | 7 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| ✅ | d3d9 (DXUT) | `CDXUTTimer` | `██████████` 100.0% | **9/9** | 0 | 9 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| ✅ | d3d9 (DXUT) | `CFirstPersonCamera` | `██████████` 100.0% | **3/3** | 0 | 3 | 0 | 0 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTDialog` | `██████████`  95.1% | **39/41** | 0 | 39 | 0 | 2 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTScrollBar` | `█████████░`  91.7% | **11/12** | 0 | 11 | 0 | 1 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTResourceCache` | `█████████░`  90.5% | **19/21** | 0 | 19 | 0 | 2 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTDialogResourceManager` | `█████████░`  90.0% | **9/10** | 0 | 9 | 0 | 1 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTComboBox` | `█████████░`  87.5% | **21/24** | 0 | 21 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CBaseCamera` | `████████░░`  81.8% | **9/11** | 0 | 9 | 0 | 2 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTSlider` | `████████░░`  75.0% | **9/12** | 0 | 9 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CModelViewerCamera` | `████████░░`  75.0% | **6/8** | 0 | 6 | 0 | 2 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTCheckBox` | `███████░░░`  70.0% | **7/10** | 0 | 7 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CUniBuffer` | `██████░░░░`  64.0% | **16/25** | 0 | 16 | 1 | 8 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CD3DArcBall` | `██████░░░░`  61.5% | **8/13** | 0 | 8 | 0 | 5 | `d3d9/common/DXUTmisc.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTRadioButton` | `██████░░░░`  57.1% | **4/7** | 0 | 4 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTStatic` | `█████░░░░░`  50.0% | **3/6** | 0 | 3 | 0 | 3 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTIMEEditBox` | `█████░░░░░`  46.8% | **22/47** | 0 | 22 | 0 | 25 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTButton` | `████░░░░░░`  44.4% | **4/9** | 0 | 4 | 0 | 5 | `d3d9/common/DXUTgui.cpp` |
| 🔄 | d3d9 (DXUT) | `CDXUTControl` | `███░░░░░░░`  26.1% | **6/23** | 0 | 6 | 0 | 17 | `d3d9/common/DXUTgui.cpp` |
| ⏳ | d3d9 (DXUT) | `DXUTState` | `░░░░░░░░░░`   0.0% | **0/197** | 0 | 0 | 0 | 197 | `d3d9/common/DXUT.cpp` |
| ✅ | raknet | `CheckSum` | `██████████` 100.0% | **4/4** | 0 | 4 | 0 | 0 | `../vendor/upstream/raknet/CheckSum.cpp` |
| ✅ | raknet | `ConnectionGraph` | `██████████` 100.0% | **32/32** | 0 | 32 | 0 | 0 | `../vendor/upstream/raknet/ConnectionGraph.cpp` |
| ✅ | raknet | `ConsoleServer` | `██████████` 100.0% | **6/6** | 0 | 6 | 0 | 0 | `../vendor/upstream/raknet/ConsoleServer.cpp` |
| ✅ | raknet | `DataBlockEncryptor` | `██████████` 100.0% | **5/5** | 0 | 5 | 0 | 0 | `../vendor/upstream/raknet/DataBlockEncryptor.cpp` |
| ✅ | raknet | `DataCompressor` | `██████████` 100.0% | **2/2** | 0 | 2 | 0 | 0 | `../vendor/upstream/raknet/DataCompressor.cpp` |
| ✅ | raknet | `DirectoryDeltaTransfer` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | `../vendor/upstream/raknet/DirectoryDeltaTransfer.cpp` |
| ✅ | raknet | `FileList` | `██████████` 100.0% | **13/13** | 0 | 13 | 0 | 0 | `../vendor/upstream/raknet/FileList.cpp` |
| ✅ | raknet | `FileListTransfer` | `██████████` 100.0% | **12/12** | 0 | 12 | 0 | 0 | `../vendor/upstream/raknet/FileListTransfer.cpp` |
| ✅ | raknet | `LightweightDatabaseClient` | `██████████` 100.0% | **7/7** | 0 | 7 | 0 | 0 | `../vendor/upstream/raknet/LightweightDatabaseClient.cpp` |
| ✅ | raknet | `LightweightDatabaseServer` | `██████████` 100.0% | **18/18** | 0 | 18 | 0 | 0 | `../vendor/upstream/raknet/LightweightDatabaseServer.cpp` |
| ✅ | raknet | `RPCMap` | `██████████` 100.0% | **6/6** | 0 | 6 | 0 | 0 | `../vendor/upstream/raknet/RPCMap.cpp` |
| ✅ | raknet | `RakClient` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | `../vendor/upstream/raknet/RakClient.cpp` |
| ✅ | raknet | `RakPeer` | `██████████` 100.0% | **64/64** | 0 | 64 | 0 | 0 | `../vendor/upstream/raknet/RakPeer.cpp` |
| ✅ | raknet | `RakServer` | `██████████` 100.0% | **1/1** | 0 | 1 | 0 | 0 | `../vendor/upstream/raknet/RakServer.cpp` |
| ✅ | raknet | `ReliabilityLayer` | `██████████` 100.0% | **32/32** | 0 | 32 | 0 | 0 | `../vendor/upstream/raknet/ReliabilityLayer.cpp` |
| ✅ | raknet | `ReplicaManager` | `██████████` 100.0% | **33/33** | 0 | 33 | 0 | 0 | `../vendor/upstream/raknet/ReplicaManager.cpp` |
| ✅ | raknet | `Router` | `██████████` 100.0% | **9/9** | 0 | 9 | 0 | 0 | `../vendor/upstream/raknet/Router.cpp` |
| ✅ | raknet | `SimpleMutex` | `██████████` 100.0% | **2/2** | 0 | 2 | 0 | 0 | `../vendor/upstream/raknet/SimpleMutex.cpp` |
| ✅ | raknet | `SocketLayer` | `██████████` 100.0% | **4/4** | 0 | 4 | 0 | 0 | `../vendor/upstream/raknet/SocketLayer.cpp` |
| ✅ | raknet | `StringCompressor` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | `../vendor/upstream/raknet/StringCompressor.cpp` |
| ✅ | raknet | `StringTable` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | `../vendor/upstream/raknet/StringTable.cpp` |
| ✅ | raknet | `SystemAddressList` | `██████████` 100.0% | **7/7** | 0 | 7 | 0 | 0 | `../vendor/upstream/raknet/SystemAddressList.cpp` |
| ✅ | raknet | `TCPInterface` | `██████████` 100.0% | **12/12** | 0 | 12 | 0 | 0 | `../vendor/upstream/raknet/TCPInterface.cpp` |
| ✅ | raknet | `TableSerializer` | `██████████` 100.0% | **6/6** | 0 | 6 | 0 | 0 | `../vendor/upstream/raknet/TableSerializer.cpp` |
| ✅ | raknet | `TelnetTransport` | `██████████` 100.0% | **13/13** | 0 | 13 | 0 | 0 | `../vendor/upstream/raknet/TelnetTransport.cpp` |
| 🔄 | raknet | `BitStream` | `██████████`  97.3% | **36/37** | 0 | 36 | 0 | 1 | `../vendor/upstream/raknet/BitStream.cpp` |
| 🔄 | raknet | `CommandParserInterface` | `█████████░`  93.8% | **15/16** | 0 | 15 | 0 | 1 | `../vendor/upstream/raknet/CommandParserInterface.cpp` |
| 🔄 | raknet | `LogCommandParser` | `█████████░`  93.3% | **14/15** | 0 | 14 | 0 | 1 | `../vendor/upstream/raknet/LogCommandParser.cpp` |
| 🔄 | raknet | `NetworkIDGenerator` | `█████████░`  92.9% | **13/14** | 0 | 13 | 0 | 1 | `../vendor/upstream/raknet/NetworkIDGenerator.cpp` |
| 🔄 | raknet | `PacketLogger` | `█████████░`  92.9% | **13/14** | 0 | 13 | 0 | 1 | `../vendor/upstream/raknet/PacketLogger.cpp` |
| 🔄 | raknet | `PluginInterface` | `█████████░`  90.9% | **10/11** | 0 | 10 | 0 | 1 | `../vendor/upstream/raknet/PluginInterface.cpp` |
| 🔄 | raknet | `FullyConnectedMesh` | `█████████░`  85.7% | **6/7** | 0 | 6 | 0 | 1 | `../vendor/upstream/raknet/FullyConnectedMesh.cpp` |
| 🔄 | raknet | `RakNetCommandParser` | `█████████░`  85.7% | **6/7** | 0 | 6 | 0 | 1 | `../vendor/upstream/raknet/RakNetCommandParser.cpp` |
| 🔄 | raknet | `InternalPacketPool` | `████████░░`  80.0% | **4/5** | 0 | 4 | 0 | 1 | `../vendor/upstream/raknet/InternalPacketPool.cpp` |
| 🔄 | raknet | `PacketFileLogger` | `████████░░`  75.0% | **3/4** | 0 | 3 | 0 | 1 | `../vendor/upstream/raknet/PacketFileLogger.cpp` |
| 🔄 | raknet | `PacketConsoleLogger` | `██████░░░░`  60.0% | **3/5** | 0 | 3 | 0 | 2 | `../vendor/upstream/raknet/PacketConsoleLogger.cpp` |
| ✅ | other | `HuffmanEncodingTree` | `██████████` 100.0% | **8/8** | 0 | 8 | 0 | 0 | - |
| ✅ | other | `NatPunchthrough` | `██████████` 100.0% | **14/14** | 0 | 14 | 0 | 0 | - |
| ✅ | other | `R5TextDrawPool` | `██████████` 100.0% | **4/4** | 4 | 0 | 0 | 0 | - |
| ✅ | other | `TEABlockEncryptor` | `██████████` 100.0% | **9/9** | 0 | 9 | 0 | 0 | - |
| ✅ | other | `Table` | `██████████` 100.0% | **20/20** | 0 | 20 | 0 | 0 | - |
| 🔄 | other | `CSHA1` | `█████████░`  90.9% | **10/11** | 0 | 10 | 0 | 1 | - |
| 🔄 | other | `DatabaseCellUpdate` | `█████░░░░░`  50.0% | **2/4** | 0 | 2 | 0 | 2 | - |
| 🔄 | other | `DatabaseFilter` | `█████░░░░░`  50.0% | **2/4** | 0 | 2 | 0 | 2 | - |
| 🔄 | other | `R5ChatPagingView` | `████░░░░░░`  40.0% | **2/5** | 2 | 0 | 3 | 0 | - |
| 🔄 | other | `R5VehiclePlateRenderer` | `██░░░░░░░░`  25.0% | **1/4** | 1 | 0 | 3 | 0 | - |
| 🔄 | other | `CPickupPoolMore` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 4 | 0 | - |
| ⏳ | other | `DatabaseTable` | `░░░░░░░░░░`   0.0% | **0/3** | 0 | 0 | 0 | 3 | - |
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
| 🔄 | other | `RPCObjectOpsView` | `░░░░░░░░░░`   0.0% | **0/4** | 0 | 0 | 4 | 0 | - |

---

## 4. Cara Memperbarui Dokumen Ini

Jalankan perintah berikut kapan saja untuk memperbarui anotasi `// R5:` dan me-regenerate laporan ini:

```bash
python3 tools/progress.py --annotate --apply
python3 tools/progress.py --markdown
```
