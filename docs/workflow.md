# Alur Kerja Rekonstruksi Fungsional

Target: setiap fungsi R5 punya padanan C++ di `client/saco/` dengan perilaku yang
sama. Tidak ada verifikasi byte-per-byte.

## 1. Pilih target

Kerjakan per modul/class, bukan per fungsi acak, supaya layout struct konsisten.

```bash
ghidra-bridge search CNetGame          # daftar fungsi class
ghidra-bridge strings "Connecting"     # cari lewat string
ghidra-bridge containing 100010c5      # fungsi yang memuat alamat
```

Cek dulu apakah fungsi sudah ada di:
1. source utama `client/saco/**` (cari `// R5: 0x...`), atau
2. `client/saco/_reference/` — jika ada, **pindahkan** ke file utama, jangan tulis ulang.

## 2. Pahami fungsi

```bash
ghidra-bridge decompile 0x100010b0     # pseudo-C
ghidra-bridge asm 0x100010b0           # assembly (untuk detail yang ambigu)
ghidra-bridge xrefs-to 0x100010b0      # pemanggil
ghidra-bridge xrefs-from 0x100010b0    # yang dipanggil
ghidra-bridge struct CEntity           # layout struct
ghidra-bridge context 0x100010b0       # ringkasan lengkap untuk LLM
```

Bandingkan dengan `vendor/upstream/saco/` (versi lama) untuk nama dan logika.

## 3. Tulis ulang ke C++

```cpp
// R5: 0x100010B0
int CPlayerPool::GetSlotState(PLAYERID id)
{
    if (id >= MAX_PLAYERS) return 0;
    return m_bPlayerSlotState[id];
}
```

Wajib dipertahankan sama dengan R5:
- offset field struct yang diakses kode lain / GTA:SA,
- urutan & lebar field BitStream, ID packet/RPC,
- alamat GTA:SA (`game/address.h`), hook & patch,
- string, konstanta, format file, kripto.

Boleh berbeda: urutan fungsi di file, nama variabel lokal, alokasi register,
inlining, susunan kode selama hasilnya setara.

Fungsi yang tidak dipanggil di R5 tetap ditulis. Nama yang belum diketahui:
`sub_XXXXXXXX`, `unkXX`.

## 4. Build & uji

1. Kompilasi dengan MSVC 2003 via Wine (lihat [linux-setup.md](linux-setup.md)).
2. Uji di instalasi GTA:SA terpisah terhadap server 0.3.7 R5.
3. Untuk hal sensitif (sinkronisasi, RPC), bandingkan perilaku dengan DLL asli.

## 5. Tracking progres

Progres dihitung dari jumlah fungsi di ekspor Ghidra yang alamatnya muncul
sebagai anotasi `// R5: 0x...` di source:

```bash
grep -rhoE '// R5: 0x[0-9A-Fa-f]+' client/saco --include=*.cpp | sort -u | wc -l
```
