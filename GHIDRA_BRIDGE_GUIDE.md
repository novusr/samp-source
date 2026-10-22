# Panduan Penggunaan Ghidra Bridge untuk AI Agent & Pengembang

Dokumen ini ditujukan sebagai referensi query data binary Ghidra untuk **SA-MP 0.3.7 R5 (`samp.dll`)** secara instan tanpa perlu membuka antarmuka GUI Ghidra.

---

> [!TIP]
> **Query Offline & Super Cepat:**
> `ghidra-bridge` membaca data JSON hasil ekspor Ghidra yang sudah tersedia di `ghidra_export/`. Seluruh perintah query (`decompile`, `search`, `struct`, `xrefs`, `strings`, dll.) berjalan dalam hitungan milidetik tanpa membebani CPU/RAM dan tanpa memerlukan Ghidra GUI yang aktif.

---

## 1. Status Data Ekspor Saat Ini

| Binary | Status Ekspor | Keterangan |
| :--- | :--- | :--- |
| **`samp.dll` (SA-MP 0.3.7-R5)** | **SUDAH DI-EXPORT ALL** ✅ | **5.265 fungsi decompiled**, 8.020 symbol/function mapping, 102 struct, 2 enum, 2.261 globals, dan 3.207 strings siap di-query. |

Direktori ekspor lokal: `ghidra_export/` (sudah diabaikan di `.gitignore`).

---

## 2. File Konfigurasi Proyek

Konfigurasi disimpan di [`ghidra-bridge.yaml`](file:///home/novusr/Documents/R5-Reversed/ghidra-bridge.yaml):

```yaml
ghidra:
  install_dir: /opt/ghidra
  project_dir: /home/novusr/Documents/R5-Reversed/private
  project_name: samp-source
  program_name: samp.dll

paths:
  export_dir: /home/novusr/Documents/R5-Reversed/ghidra_export
  address_map: /home/novusr/Documents/R5-Reversed/ghidra_export/address_map.json

source:
  root: /home/novusr/Documents/R5-Reversed/client/saco
```

---

## 3. Daftar Perintah Query Utama (Read-Only)

Jalankan perintah ini langsung dari terminal di folder proyek:

### 1. Mencari Fungsi (`search`)
Mencari berdasarkan nama class, nama method, atau simbol:
```bash
ghidra-bridge search PlayerPool
ghidra-bridge search ActorPool
ghidra-bridge search CChatWindow
ghidra-bridge search RakClient
```

### 2. Melihat Kode Dekompilasi C/C++ (`decompile`)
Melihat kode hasil dekompilasi Ghidra berdasarkan nama fungsi atau alamat memori:
```bash
# Berdasarkan nama fungsi/symbol:
ghidra-bridge decompile "PlayerPool.GetSlotState"
ghidra-bridge decompile "ActorPool.ctor"

# Berdasarkan alamat RVA atau ImageBase (0x10000000 + RVA):
ghidra-bridge decompile 100010b0
ghidra-bridge decompile 0x100016c0
```

### 3. Cross-References (`xrefs-to` & `xrefs-from`)
- Siapa saja yang memanggil fungsi ini:
  ```bash
  ghidra-bridge xrefs-to 100010b0
  ```
- Fungsi apa saja yang dipanggil oleh fungsi ini:
  ```bash
  ghidra-bridge xrefs-from 100010b0
  ```

### 4. Struktur Data (`struct` & `enum`)
Melihat layout struct dan offset field:
```bash
ghidra-bridge struct CEntity
ghidra-bridge struct ActorPool
```

### 5. Virtual Method Table (`vtable`)
Melihat daftar virtual method dari suatu class:
```bash
ghidra-bridge vtable CEntity
```

### 6. Variabel Global (`global`)
Mengecek informasi global variable berdasarkan alamat:
```bash
ghidra-bridge global 1026bf10
```

### 7. Pencarian String (`strings`)
Mencari string di dalam binary beserta fungsi yang mereferensikannya:
```bash
ghidra-bridge strings "SAMP"
ghidra-bridge strings "SetWeather"
ghidra-bridge strings "Connecting"
```

### 8. Menemukan Fungsi Pembungkus Alamat Crash / Instruction (`containing`)
Menemukan fungsi apa yang mencakup address tertentu:
```bash
ghidra-bridge containing 100010c5
```

### 9. Konteks Lengkap untuk LLM Reasoning (`context`)
Menghasilkan ringkasan JSON lengkap (callers, callees, strings, globals):
```bash
ghidra-bridge context 100010b0
```

---

## 4. Prosedur Ekspor Ulang Manual (Hanya jika Diperlukan)

Data ekspor saat ini sudah lengkap. Jika di kemudian hari Anda menambahkan anotasi, rename tipe data, atau analisis baru di Ghidra GUI dan ingin mengekspor ulang:

> [!WARNING]
> Pastikan Ghidra GUI sudah ditutup sebelum menjalankan ekspor ulang agar tidak terjadi lock conflict pada project file.

```bash
ghidra-bridge export all
```

---

## 5. Alur Kerja Rekonstruksi & Matching Fungsi Leaf

Contoh siklus end-to-end rekonstruksi fungsi leaf (seperti `CPlayerPed::IsCellphoneEnabled`, `CCryptoContext::GetProvider`, dll.):

1. **Analisis & Ranking Kandidat Uncovered:**
   ```bash
   python3 tools/rank_uncovered_candidates.py \
     --proof evidence/checkpoint32/client-router-rpc-acceptance.json \
     --discovery evidence/checkpoint32/michael-player-discovery.json \
     --discovery evidence/checkpoint32/ui-pools-discovery.json \
     --output build/ranked-uncovered.json
   ```

2. **Dekomposisi & Inspeksi Assembly via Ghidra Bridge:**
   ```bash
   ghidra-bridge decompile 100ae0a0
   ghidra-bridge asm 100ae0a0
   ```

3. **Rekonstruksi di Source Code (`src/`):**
   Tuliskan implementasi C++ murni yang mereproduksi register dan layout akses yang identik.

4. **Kompilasi & Verifikasi Exact Matching:**
   ```bash
   python3 tools/build_linux.py --run baseline
   python3 tools/verify.py --run baseline
   ```

