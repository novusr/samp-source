# Linux Setup (Wine + MSVC 2003)

Klien dikompilasi dengan **Microsoft Visual C++ Toolkit 2003** (`cl.exe`
13.10.3077), compiler yang sama dengan era R5. Ini menjaga ABI, layout struct
(`/Zp1`), CRT dan perilaku C++ tetap kompatibel dengan GTA:SA dan R5. Hasil build
**tidak perlu** identik byte-per-byte dengan `samp.dll` asli.

## Prasyarat

| Komponen | Catatan |
|---|---|
| Wine 6.0+ | Diuji dengan Wine 11.19 (Arch Linux) |
| Visual C++ Toolkit 2003 | `cl.exe`, `link.exe` |
| DirectX 9 SDK | `d3d9.h`, `d3dx9.h`, `d3d9.lib`, `d3dx9.lib`, `dxguid.lib` |
| Windows Platform SDK (2003) | `comctl32.lib`, `wsock32.lib`, `winmm.lib`, `rc.exe` |
| BASS | `bass.lib` (`bass.h` sudah ada di `client/saco/`) |
| ghidra-bridge | Untuk query ekspor Ghidra (lihat [GHIDRA_BRIDGE_GUIDE.md](../GHIDRA_BRIDGE_GUIDE.md)) |

```bash
sudo pacman -S wine        # Arch / CachyOS
sudo apt install wine      # Ubuntu / Debian
```

## 1. Binary referensi

`samp.dll` R5 asli dipakai sebagai referensi dekompilasi dan pembanding perilaku.
Tidak di-commit.

```bash
mkdir -p private
cp /path/to/samp.dll private/samp.dll
sha256sum private/samp.dll
# b72b5dbe725f81864ca3f78bc7063bda56cc05fc7188af822fa7a754432553a2
```

## 2. Compiler MSVC 2003

```bash
curl -L "https://archive.org/download/microsoft-visual-c-toolkit-2003/VCToolkitSetup.exe" \
     -o /tmp/VCToolkitSetup.exe
wine /tmp/VCToolkitSetup.exe /s /v"/qn"
rm /tmp/VCToolkitSetup.exe
```

Lokasi: `~/.wine/drive_c/Program Files (x86)/Microsoft Visual C++ Toolkit 2003/bin/cl.exe`.
Versi/hash yang dipakai tercatat di [`config/toolchain.json`](../config/toolchain.json).

## 3. Flag kompilasi

Berdasarkan konfigurasi Release `vendor/upstream/saco/saco.vcproj`:

```
/nologo /c /Ox /Og /Ob1 /Oi /Ot /Oy /MT /Zp1 /EHsc /Gy
/D WIN32 /D NDEBUG /D _WINDOWS /D _USRDLL /D SACO_EXPORTS /D SAMPCLI
```

Link (DLL):

```
dxguid.lib d3d9.lib d3dx9.lib comctl32.lib wsock32.lib winmm.lib bass.lib
```

`/Zp1` wajib: banyak struct jaringan dan GTA:SA bergantung pada packing 1 byte.

## 4. Build

> [!NOTE]
> Skrip build DLL penuh belum dibuat. Tool lama (`tools/build_linux.py`,
> `verify.py`, dll.) khusus byte matching dan sudah dihapus dari branch ini;
> masih tersedia di branch `main`.

Untuk mengecek satu file sementara waktu:

```bash
MSVC="$HOME/.wine/drive_c/Program Files (x86)/Microsoft Visual C++ Toolkit 2003"
WINEDEBUG=-all wine "$MSVC/bin/cl.exe" /nologo /c /Ox /MT /Zp1 /EHsc \
  /I "Z:$MSVC/include" /D WIN32 /D NDEBUG /D SAMPCLI \
  client/saco/net/playerpool.cpp /Fobuild/
```

## Troubleshooting

- **`cl.exe` tidak ditemukan** — pastikan installer Toolkit selesai dan path di atas ada.
- **Header DirectX/Windows tidak ditemukan** — Toolkit 2003 tidak menyertakan
  Platform SDK maupun DirectX SDK; instal terpisah dan tambahkan `/I` & `/LIBPATH`.
- **`err:ole:...` saat install** — kosmetik dari Wine, bisa diabaikan.
