# Linux Build Setup (Wine + MSVC 2003)

Proyek ini membutuhkan compiler asli **Microsoft Visual C++ Toolkit 2003** (`cl.exe`
versi `13.10.3077`) agar kode yang dikompilasi menghasilkan byte yang identik dengan
`samp.dll` R5. Panduan ini menjelaskan cara menyiapkan lingkungan kerja di **Linux**
tanpa Virtual Machine Windows, menggunakan Wine.

---

## Prasyarat

| Komponen | Minimum | Catatan |
|---|---|---|
| Wine | 6.0+ | Diuji dengan Wine 11.19 di Arch Linux |
| Python | 3.11 | Via pyenv atau paket distro |
| p7zip / 7z | 22.0+ | Untuk mengekstrak arsip (opsional) |
| gh (GitHub CLI) | 2.0+ | Opsional, untuk push ke fork |

Instal Wine di distro Anda:
```bash
# Arch Linux / CachyOS
sudo pacman -S wine

# Ubuntu / Debian
sudo apt install wine
```

---

## Langkah 1 — Binary Referensi (`private/samp.dll`)

File referensi **tidak ikut di-commit** (ada di `.gitignore`). Anda harus menyediakan
sendiri file `samp.dll` SA-MP 0.3.7 R5 yang asli.

SHA-256 yang wajib cocok:
```
b72b5dbe725f81864ca3f78bc7063bda56cc05fc7188af822fa7a754432553a2
```

Salin ke folder `private/` (folder ini diabaikan oleh git):
```bash
mkdir -p private
cp /path/to/your/samp.dll private/samp.dll
sha256sum private/samp.dll   # Verifikasi hash
```

---

## Langkah 2 — Python Virtual Environment

```bash
# Gunakan Python 3.11 (via pyenv jika perlu)
/path/to/python3.11 -m venv .venv
source .venv/bin/activate

# Instal dependensi Python
pip install -r requirements.txt
```

Dependensi yang diinstal:
- `capstone==5.0.9` — Disassembler x86 untuk verifikasi instruksi
- `unicorn==2.1.4` — Emulator CPU untuk ABI tests
- `python-idb==0.8.0` — Pembaca database IDA Pro

---

## Langkah 3 — Compiler MSVC 2003 via Wine

Visual C++ Toolkit 2003 (versi gratis dari MSVC 7.1) tersedia di Internet Archive.
Paket ini berisi `cl.exe` versi `13.10.3077` yang identik dengan yang digunakan untuk
mengompilasi `samp.dll` R5 asli.

```bash
# Unduh installer
curl -L "https://archive.org/download/microsoft-visual-c-toolkit-2003/VCToolkitSetup.exe" \
     -o /tmp/VCToolkitSetup.exe

# Instal secara senyap melalui Wine (WINEPREFIX default: ~/.wine)
wine /tmp/VCToolkitSetup.exe /s /v"/qn"

# Bersihkan installer
rm /tmp/VCToolkitSetup.exe
```

Setelah instalasi, compiler berada di:
```
~/.wine/drive_c/Program Files (x86)/Microsoft Visual C++ Toolkit 2003/bin/cl.exe
```

**Verifikasi hash toolchain** (wajib cocok dengan `config/toolchain.json`):
```bash
cd "$HOME/.wine/drive_c/Program Files (x86)/Microsoft Visual C++ Toolkit 2003/bin"
sha256sum cl.exe c1xx.dll c2.dll link.exe
```

| File | SHA-256 yang Diharapkan |
|---|---|
| `cl.exe` | `2ecf86a3edfd3deae498e08298e210e984537ce9e11759930561e43f40bd2515` |
| `c1xx.dll` | `353f3d5dcd050b876e1c044b99c4bf47958d255f395a4919f406a19921be4dfa` |
| `c2.dll` | `bcd28f39b1798b07989c30a1df471cf87fc20e3802d130901a08f83800e6b81e` |
| `link.exe` | `0d5f9712d0da843d787bf4ce4e678abc6b737940996706b2c86101eac9df3e3c` |

---

## Langkah 4 — Build dan Verifikasi

Gunakan script runner Linux yang disediakan di `tools/build_linux.py`:

```bash
# Build dua kali dengan image base berbeda (diperlukan uji reprodusibilitas)
.venv/bin/python tools/build_linux.py --run baseline
.venv/bin/python tools/build_linux.py --run repeat

# Verifikasi bahwa hasil kompilasi cocok byte-per-byte dengan samp.dll
.venv/bin/python tools/verify.py --run baseline
.venv/bin/python tools/verify.py --run repeat
```

Output yang diharapkan dari `verify.py`:
```
RAW_EXACT                 PlayerPool.GetSlotState            26 bytes
RAW_EXACT                 PlayerPool.GetAt                   36 bytes
...
PASS: 15/15 functions, 540 unique code bytes
```

Jalankan unit tests verifier:
```bash
.venv/bin/python -m unittest tests/test_verifier.py
# Ran 7 tests -> OK
```

---

## Catatan Teknis

### Kenapa Wine, Bukan Cross-Compiler?

Cross-compiler seperti MinGW / Clang tidak bisa digunakan karena:
- Compiler yang berbeda menghasilkan *code generation* yang berbeda
- Proyek ini mensyaratkan kecocokan **byte demi byte** dengan binary asli
- Hanya `cl.exe` `13.10.3077` dengan flags `/Ox /Ob1 /Ot /Zp1` yang mereproduksi
  output yang identik

### Wine vs Virtual Machine Windows

| | VM Windows | Wine |
|---|---|---|
| Konsumsi RAM | 4–8 GB | ~20 MB |
| Beban CPU | Berat | Hampir nol |
| Kecepatan compile 1 file | 1–5 detik | < 1 detik |
| Ruang disk | 20–40 GB | ~150 MB |

Wine menjalankan binary Win32 secara langsung menggunakan ABI layer — tidak ada
emulasi CPU. Hasil `.obj` yang dihasilkan identik 100% dengan yang dihasilkan
di Windows asli menggunakan compiler yang sama.

### Struktur `build/`

Setiap build menghasilkan folder `build/<run>/` berisi:
```
build/baseline/
  build.json       # Metadata build: hash source, hash object, toolchain
  src/             # Snapshot source saat build
  pools.obj
  actors.obj
  camera.obj
  game.obj
```

Folder `build/` ada di `.gitignore` dan tidak di-commit.

### Flag Compiler Wajib

Flag yang digunakan (sesuai `config/toolchain.json`):
```
/nologo /c /Ox /Og /Ob1 /Oi /Ot /Oy /MT /Zp1 /EHsc /Gy
```

Mengubah satu pun flag ini akan menghasilkan instruksi yang berbeda dari R5 asli
dan tidak akan lolos verifikasi.

---

## Troubleshooting

**`cl.exe` tidak ditemukan**  
Pastikan Wine terinstal dan VCToolkitSetup telah dijalankan. Cek:
```bash
ls "$HOME/.wine/drive_c/Program Files (x86)/Microsoft Visual C++ Toolkit 2003/bin/cl.exe"
```

**Hash toolchain tidak cocok**  
Anda memiliki versi `cl.exe` yang berbeda. Hanya build `13.10.3077` dari
*Visual C++ Toolkit 2003* yang diterima.

**`private/samp.dll` tidak ditemukan**  
Buat folder `private/` dan salin file `samp.dll` R5 yang asli ke dalamnya.
Proyek tidak menyertakan binary asli karena alasan hak cipta.

**Error Wine saat install (`err:ole:...`)**  
Pesan error tersebut bersifat kosmetik dari Wine dan bisa diabaikan. Selama
`cl.exe` ada di path yang benar, instalasi berhasil.
