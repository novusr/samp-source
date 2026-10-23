<div align="center">

# SAMP Source

**Rekonstruksi penuh klien SA-MP 0.3.7 R5 ke C++ yang dapat dibaca dan dimainkan.**

SA-MP 0.3.7 R5 · C++ · MSVC 2003 · AI-assisted reverse engineering

</div>

## Tujuan

Mendekompilasi **seluruh** fungsi `samp.dll` 0.3.7 R5 dan menulis ulang dalam C++
sehingga menghasilkan DLL yang **berfungsi identik** dengan R5 dan bisa dipakai
bermain di server R5.

- **1:1 secara fungsi** — setiap fungsi, class, RPC, hook, patch dan tabel data
  di R5 ada di source, termasuk kode yang tidak pernah dipanggil di R5.
- **Bukan byte matching** — hasil kompilasi tidak harus identik byte-per-byte.
  Yang wajib sama adalah perilaku: layout struct, protokol jaringan, alamat
  memori GTA:SA, hook, string dan konstanta.

> Arah ini menggantikan pendekatan *matching decompilation* sebelumnya. Riwayat
> lengkap pekerjaan matching (bukti, manifest, tool verifikasi) tetap ada di
> branch `main` dan tag/branch `checkpoint-3-2`.

## Struktur

| Path | Isi |
|---|---|
| [`client/saco/`](client/saco/) | Source utama klien (game, net, GUI/DXUT, archive, d3dhook, dll.). |
| [`client/saco/_reference/`](client/saco/_reference/) | Potongan kode yang dulu sudah terverifikasi byte-exact (`closure_*`, `eval_*`, `matching/`). Bahan untuk digabung ke source utama, lalu dihapus. |
| [`client/archive/`](client/archive/) | Kunci arsip (`pkey.h`, `skey.h`). |
| [`vendor/upstream/`](vendor/upstream/) | RakNet dan source referensi upstream (read-only). |
| [`tools/`](tools/) | Tool rekonstruksi fungsional: `build.py`, `progress.py`, `decompile.py`, `merge_ref.py`. |
| [`config/toolchain.json`](config/toolchain.json) | Versi & hash compiler MSVC 2003 serta flag default. |
| [`docs/`](docs/) | Dokumentasi setup dan alur kerja. |
| `ghidra_export/`, `private/`, `build/` | Lokal saja (di-ignore): ekspor Ghidra, `samp.dll` asli, hasil build. |

## Dokumentasi

- [Alur kerja rekonstruksi](docs/workflow.md) — cara mendekompilasi, menggabungkan `_reference/`, & menulis ulang fungsi.
- [Setup Linux (Wine + MSVC 2003)](docs/linux-setup.md) — penggunaan `tools/build.py`.
- [Panduan Ghidra Bridge](GHIDRA_BRIDGE_GUIDE.md) — query hasil ekspor Ghidra secara offline.
- [Aturan proyek](AGENTS.md)

## Status

| Milestone | Status |
|---|---|
| Ekspor Ghidra R5 (5.265 fungsi) | Selesai |
| Tooling build & tracking (`tools/`) | Selesai |
| Anotasi awal `// R5:` otomatis di `client/saco/` | 617 fungsi teranotasi |
| Gabungkan `_reference/` ke source utama | Sedang berjalan |
| Dekompilasi seluruh fungsi R5 | Sedang berjalan |
| DLL bisa dimainkan di server R5 | Target akhir |

## Credits

Dibangun di atas karya komunitas, termasuk
[dashr9230/SA-MP](https://github.com/dashr9230/SA-MP). Binary asli, compiler dan
hasil build tidak disertakan.
