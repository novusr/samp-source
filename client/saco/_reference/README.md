# _reference

Potongan kode dari era byte matching yang sudah terbukti identik dengan R5:

- `closure/` — fungsi beserta dependensinya, per area (GUI, DXUT, IME, chat, model, dll.).
- `eval/` — eksperimen/evaluasi fungsi (termasuk RakNet di `eval_vendor_*`).
- `matching/` — fungsi game/pool hasil `prepare_checkpoint32.py`.

File di sini **tidak ikut build** dan path `#include`-nya bisa rusak karena
sudah dipindah. Cara pakai: salin fungsinya ke file utama di `client/saco/`,
beri anotasi `// R5: 0x...`, lalu hapus fragmennya dari sini.
