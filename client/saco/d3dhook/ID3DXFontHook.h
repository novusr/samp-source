
#pragma once
#ifndef ID3DXFONTHOOK_H
#define ID3DXFONTHOOK_H

#include <d3d9.h>
#include <d3dx9.h>

interface ID3DXFontHook : public ID3DXFont
{
public:
	HRESULT __stdcall QueryInterface(REFIID iid, LPVOID *ppv);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall GetDevice(LPDIRECT3DDEVICE9 *ppDevice);
	HRESULT __stdcall GetDescA(D3DXFONT_DESCA *pDesc);
	HRESULT __stdcall GetDescW(D3DXFONT_DESCW *pDesc);
	BOOL __stdcall GetTextMetricsA(TEXTMETRICA *pTextMetrics);
	BOOL __stdcall GetTextMetricsW(TEXTMETRICW *pTextMetrics);
	HDC __stdcall GetDC();
	HRESULT __stdcall GetGlyphData(UINT Glyph, LPDIRECT3DTEXTURE9 *ppTexture, RECT *pBlackBox, POINT *pCellInc);
	HRESULT __stdcall PreloadCharacters(UINT First, UINT Last);
	HRESULT __stdcall PreloadGlyphs(UINT First, UINT Last);
	HRESULT __stdcall PreloadTextA(LPCSTR pString, INT Count);
	HRESULT __stdcall PreloadTextW(LPCWSTR pString, INT Count);
	INT __stdcall DrawTextA(LPD3DXSPRITE pSprite, LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color);
	INT __stdcall DrawTextW(LPD3DXSPRITE pSprite, LPCWSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color);
	HRESULT __stdcall OnLostDevice();
	HRESULT __stdcall OnResetDevice();

	ID3DXFont *m_pD3DFont;
};

// Global font and color embed state
extern DWORD r5FontEmbeddedColor;
extern int r5FontCharacterIndex;
extern wchar_t r5FontWideText[20000];
extern char *r5FontOriginalText;
extern char *r5FontStrippedText;
extern ID3DXSprite *r5FontActualSprite;

// R5: 0x100739A0
void R5FontConvertAnsiToWide(char *source, wchar_t *destination, int capacity);

// R5: 0x10073A10
void R5FontConsumeAnsiColorEmbeds();

// R5: 0x10073AE0
void R5FontConsumeWideColorEmbeds();

class R5FontSpriteProxy : public ID3DXSprite
{
public:
	R5FontSpriteProxy();

	HRESULT __stdcall QueryInterface(REFIID iid, void **ppv);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();
	HRESULT __stdcall GetDevice(IDirect3DDevice9 **device);
	HRESULT __stdcall GetTransform(D3DXMATRIX *transform);
	HRESULT __stdcall SetTransform(const D3DXMATRIX *transform);
	HRESULT __stdcall SetWorldViewRH(const D3DXMATRIX *world, const D3DXMATRIX *view);
	HRESULT __stdcall SetWorldViewLH(const D3DXMATRIX *world, const D3DXMATRIX *view);
	HRESULT __stdcall Begin(DWORD flags);
	HRESULT __stdcall Draw(IDirect3DTexture9 *texture, const RECT *rect,
	                       const D3DXVECTOR3 *center, const D3DXVECTOR3 *position,
	                       D3DCOLOR color);
	HRESULT __stdcall Flush();
	HRESULT __stdcall End();
	HRESULT __stdcall OnLostDevice();
	HRESULT __stdcall OnResetDevice();
};

extern R5FontSpriteProxy r5FontSpriteProxy;

#endif
