# Alur Kerja Rekonstruksi Fungsional

Target: setiap fungsi R5 punya padanan C++ di `client/saco/` dengan perilaku yang
sama. Tidak ada verifikasi byte-per-byte.

## Daftar Tool (`tools/`)

| Tool | Fungsi |
|---|---|
| [`tools/progress.py`](../tools/progress.py) | Dashboard progres 1:1 terhadap 5.265 fungsi Ghidra (`_index.json` & `address_map.json`), rekomendasi fungsi berikutnya (`--next`), dan auto-annotator `// R5: 0x...` (`--annotate --apply`). |
| [`tools/decompile.py`](../tools/decompile.py) | Dekompilasi & disassembly instan dari `ghidra_export/` + perbandingan langsung dengan kode di `client/saco/` dan `_reference/`. |
| [`tools/merge_ref.py`](../tools/merge_ref.py) | Membandingkan 516 file di `client/saco/_reference/` terhadap file utama di `client/saco/` (`IDENTICAL`, `DIFFERENT`, `MISSING_IN_MAIN`) beserta `diff`-nya. |
| [`tools/build.py`](../tools/build.py) | Kompilasi per unit (`--unit`), per modul (`--module`), atau seluruh proyek (`--all`) menggunakan MSVC 2003 via Wine dengan object cache SHA-256. |

---

## 1. Cek Progres & Pilih Target

Kerjakan per modul/class, bukan per fungsi acak, supaya layout struct konsisten:

```bash
# Ringkasan progres keseluruhan & tabel per class
python3 tools/progress.py

# Lihat semua fungsi R5 untuk satu class
python3 tools/progress.py --class CPlayerPool

# Rekomendasi fungsi berikutnya (yang siap di-merge dari _reference/ atau belum ada)
python3 tools/progress.py --next
```

## 2. Gabungkan Fragmen dari `_reference/` (Jika Ada)

Sebelum menulis ulang fungsi dari nol, cek apakah fungsinya sudah pernah
direkonstruksi di `client/saco/_reference/`:

```bash
# Lihat ringkasan fungsi di _reference/ per file tujuan
python3 tools/merge_ref.py

# Lihat daftar fungsi _reference/ untuk satu file
python3 tools/merge_ref.py --target game/vehicle.cpp

# Bandingkan diff antara _reference/ dan file utama
python3 tools/merge_ref.py --diff CVehicle::IsOccupied
```

Jika sudah ada di `_reference/`, salin/gabungkan ke file utama di `client/saco/`,
pastikan ada komentar `// R5: 0x...`, lalu hapus fragmen di `_reference/`.

## 3. Dekompilasi & Pahami Fungsi R5

Gunakan `tools/decompile.py` (atau `ghidra-bridge`) untuk melihat hasil
dekompilasi C, assembly, dan pemanggil/yang dipanggil:

```bash
# Berdasarkan alamat atau nama Class::Method
python3 tools/decompile.py 0x100010b0 --asm --xrefs
python3 tools/decompile.py CPlayerPool::GetSlotState --asm

# Cetak kerangka (scaffold) C++ untuk seluruh class
python3 tools/decompile.py --class CCamera --scaffold
```

## 4. Tulis Ulang ke C++

```cpp
// R5: 0x100010B0
BOOL CPlayerPool::GetSlotState(PLAYERID playerId)
{
    if (playerId >= MAX_PLAYERS) return FALSE;
    return m_bPlayerSlotState[playerId];
}
```

Wajib dipertahankan sama dengan R5:
- offset field struct yang diakses kode lain / GTA:SA,
- urutan & lebar field BitStream, ID packet/RPC,
- alamat GTA:SA (`game/address.h`), hook & patch,
- string, konstanta, format file, kripto.

Fungsi yang tidak dipanggil di R5 tetap ditulis. Nama yang belum diketahui:
`sub_XXXXXXXX`, `unkXX`.

## 5. Kompilasi & Uji

```bash
# Uji kompilasi 1 file (< 1 detik jika sudah ter-cache)
python3 tools/build.py --unit net/playerpool

# Uji kompilasi 1 modul (game, net, archive, d3dhook, root, d3d9, raknet)
python3 tools/build.py --module net

# Uji kompilasi seluruh client/saco/ secara paralel
python3 tools/build.py --all -j 4

# Lihat status kompilasi terakhir tanpa kompilasi ulang
python3 tools/build.py --status
```
