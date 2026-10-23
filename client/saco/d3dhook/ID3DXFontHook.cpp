
#include "ID3DXFontHook.h"
#include "../main.h"
#include "../game/util.h"
#include <stdlib.h>

// Reconstructed actual R5 globals and helpers; no new fake scratch allocation.
extern ID3DXSprite *r5FontActualSprite;
extern DWORD r5FontEmbeddedColor;
extern int r5FontCharacterIndex;
extern wchar_t r5FontWideText[20000];
extern char *r5FontOriginalText;
extern char *r5FontStrippedText;

// Real initialized static object 10102BF0 -> compiler-produced VFT EA744/56.
R5FontSpriteProxy r5FontSpriteProxy;

// R5: 0x100739A0
void R5FontConvertAnsiToWide(char *source, wchar_t *destination, int capacity)
{
	memset(destination, 0, capacity * sizeof(wchar_t));
	int required = MultiByteToWideChar(CP_ACP, 0, source, strlen(source), NULL, 0);
	if (required < capacity)
		MultiByteToWideChar(CP_ACP, 0, source, strlen(source), destination, required);
}

// R5: 0x10073A10
void R5FontConsumeAnsiColorEmbeds()
{
	while (r5FontOriginalText[r5FontCharacterIndex] &&
	      (r5FontOriginalText[r5FontCharacterIndex] == ' ' ||
	       r5FontOriginalText[r5FontCharacterIndex] == '\t' ||
	       r5FontOriginalText[r5FontCharacterIndex] == '\n' ||
	       r5FontOriginalText[r5FontCharacterIndex] == '\r'))
		++r5FontCharacterIndex;

	DWORD color = GetColorFromEmbedCode(r5FontOriginalText + r5FontCharacterIndex);
	while (color != 0xFFFFFFFF)
	{
		r5FontEmbeddedColor = color | 0xFF000000;
		r5FontCharacterIndex += 8;
		while (r5FontOriginalText[r5FontCharacterIndex] &&
		      (r5FontOriginalText[r5FontCharacterIndex] == ' ' ||
		       r5FontOriginalText[r5FontCharacterIndex] == '\t' ||
		       r5FontOriginalText[r5FontCharacterIndex] == '\n' ||
		       r5FontOriginalText[r5FontCharacterIndex] == '\r'))
			++r5FontCharacterIndex;
		color = GetColorFromEmbedCode(r5FontOriginalText + r5FontCharacterIndex);
	}

	if (r5FontOriginalText[r5FontCharacterIndex])
	{
		int length = mblen(r5FontOriginalText + r5FontCharacterIndex, MB_CUR_MAX);
		if (length < 0)
			++r5FontCharacterIndex;
		else
			r5FontCharacterIndex += length;
	}
}

// R5: 0x10073AE0
void R5FontConsumeWideColorEmbeds()
{
	while (r5FontWideText[r5FontCharacterIndex] &&
	      (r5FontWideText[r5FontCharacterIndex] == L' ' ||
	       r5FontWideText[r5FontCharacterIndex] == L'\t' ||
	       r5FontWideText[r5FontCharacterIndex] == L'\n' ||
	       r5FontWideText[r5FontCharacterIndex] == L'\r'))
		++r5FontCharacterIndex;

	DWORD color = GetColorFromEmbedCode(r5FontWideText + r5FontCharacterIndex);
	while (color != 0xFFFFFFFF)
	{
		r5FontEmbeddedColor = color | 0xFF000000;
		r5FontCharacterIndex += 8;
		while (r5FontWideText[r5FontCharacterIndex] &&
		      (r5FontWideText[r5FontCharacterIndex] == L' ' ||
		       r5FontWideText[r5FontCharacterIndex] == L'\t' ||
		       r5FontWideText[r5FontCharacterIndex] == L'\n' ||
		       r5FontWideText[r5FontCharacterIndex] == L'\r'))
			++r5FontCharacterIndex;
		color = GetColorFromEmbedCode(r5FontWideText + r5FontCharacterIndex);
	}

	if (r5FontOriginalText[r5FontCharacterIndex])
		++r5FontCharacterIndex;
}

// R5: 0x10073BC0
HRESULT __stdcall R5FontSpriteProxy::Draw(IDirect3DTexture9 *texture, const RECT *rect,
	const D3DXVECTOR3 *center, const D3DXVECTOR3 *position, D3DCOLOR color)
{
	R5FontConsumeWideColorEmbeds();
	return r5FontActualSprite->Draw(texture, rect, center, position, r5FontEmbeddedColor);
}

// R5: 0x10073BE0
INT __stdcall ID3DXFontHook::DrawTextA(LPD3DXSPRITE pSprite, LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	if (pSprite)
	{
		r5FontActualSprite = pSprite;
		r5FontEmbeddedColor = Color;
		r5FontCharacterIndex = 0;
		if (strlen(pString) > 100000)
			return 0;
		strcpy(r5FontOriginalText, pString);
		strcpy(r5FontStrippedText, pString);
		R5FontConvertAnsiToWide(r5FontOriginalText, r5FontWideText, 20000);
		RemoveColorEmbedsFromString(r5FontStrippedText);
		return m_pD3DFont->DrawTextA(reinterpret_cast<ID3DXSprite *>(&r5FontSpriteProxy),
			r5FontStrippedText, strlen(r5FontStrippedText), pRect, Format, Color);
	}
	else
		return m_pD3DFont->DrawTextA(NULL, pString, Count, pRect, Format, Color);
}

// R5: 0x10073CD0
HRESULT __stdcall ID3DXFontHook::QueryInterface(REFIID iid, LPVOID *ppv)
{
	return m_pD3DFont->QueryInterface(iid, ppv);
}

// R5: 0x10073CE0
ULONG __stdcall ID3DXFontHook::AddRef()
{
	return m_pD3DFont->AddRef();
}

// R5: 0x10073CF0
ULONG __stdcall ID3DXFontHook::Release()
{
	return m_pD3DFont->Release();
}

// R5: 0x10073D00
HRESULT __stdcall ID3DXFontHook::GetDevice(LPDIRECT3DDEVICE9 *ppDevice)
{
	return m_pD3DFont->GetDevice(ppDevice);
}

// R5: 0x10073D10
HRESULT __stdcall ID3DXFontHook::GetDescA(D3DXFONT_DESCA *pDesc)
{
	return m_pD3DFont->GetDescA(pDesc);
}

// R5: 0x10073D20
HRESULT __stdcall ID3DXFontHook::GetDescW(D3DXFONT_DESCW *pDesc)
{
	return m_pD3DFont->GetDescW(pDesc);
}

// R5: 0x10073D30
BOOL __stdcall ID3DXFontHook::GetTextMetricsA(TEXTMETRICA *pTextMetrics)
{
	return m_pD3DFont->GetTextMetricsA(pTextMetrics);
}

// R5: 0x10073D40
BOOL __stdcall ID3DXFontHook::GetTextMetricsW(TEXTMETRICW *pTextMetrics)
{
	return m_pD3DFont->GetTextMetricsW(pTextMetrics);
}

// R5: 0x10073D50
HDC __stdcall ID3DXFontHook::GetDC()
{
	return m_pD3DFont->GetDC();
}

// R5: 0x10073D60
HRESULT __stdcall ID3DXFontHook::GetGlyphData(UINT Glyph, LPDIRECT3DTEXTURE9 *ppTexture, RECT *pBlackBox, POINT *pCellInc)
{
	return m_pD3DFont->GetGlyphData(Glyph, ppTexture, pBlackBox, pCellInc);
}

// R5: 0x10073D70
HRESULT __stdcall ID3DXFontHook::PreloadCharacters(UINT First, UINT Last)
{
	return m_pD3DFont->PreloadCharacters(First, Last);
}

// R5: 0x10073D80
HRESULT __stdcall ID3DXFontHook::PreloadGlyphs(UINT First, UINT Last)
{
	return m_pD3DFont->PreloadGlyphs(First, Last);
}

// R5: 0x10073D90
HRESULT __stdcall ID3DXFontHook::PreloadTextA(LPCSTR pString, INT Count)
{
	return m_pD3DFont->PreloadTextA(pString, Count);
}

// R5: 0x10073DA0
HRESULT __stdcall ID3DXFontHook::PreloadTextW(LPCWSTR pString, INT Count)
{
	return m_pD3DFont->PreloadTextW(pString, Count);
}

// R5: 0x10073DB0
INT __stdcall ID3DXFontHook::DrawTextW(LPD3DXSPRITE pSprite, LPCWSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	return m_pD3DFont->DrawTextW(pSprite, pString, Count, pRect, Format, Color);
}

// R5: 0x10073DC0
HRESULT __stdcall ID3DXFontHook::OnLostDevice()
{
	return m_pD3DFont->OnLostDevice();
}

// R5: 0x10073DD0
HRESULT __stdcall ID3DXFontHook::OnResetDevice()
{
	return m_pD3DFont->OnResetDevice();
}

// R5: 0x10073DE0
HRESULT __stdcall R5FontSpriteProxy::QueryInterface(REFIID iid, void **ppv)
{
	return r5FontActualSprite->QueryInterface(iid, ppv);
}

// R5: 0x10073DF0
ULONG __stdcall R5FontSpriteProxy::AddRef()
{
	return r5FontActualSprite->AddRef();
}

// R5: 0x10073E00
ULONG __stdcall R5FontSpriteProxy::Release()
{
	return r5FontActualSprite->Release();
}

// R5: 0x10073E10
HRESULT __stdcall R5FontSpriteProxy::GetDevice(IDirect3DDevice9 **device)
{
	return r5FontActualSprite->GetDevice(device);
}

// R5: 0x10073E20
HRESULT __stdcall R5FontSpriteProxy::GetTransform(D3DXMATRIX *transform)
{
	return r5FontActualSprite->GetTransform(transform);
}

// R5: 0x10073E30
HRESULT __stdcall R5FontSpriteProxy::SetTransform(const D3DXMATRIX *transform)
{
	return r5FontActualSprite->SetTransform(transform);
}

// R5: 0x10073E40
HRESULT __stdcall R5FontSpriteProxy::SetWorldViewRH(const D3DXMATRIX *world, const D3DXMATRIX *view)
{
	return r5FontActualSprite->SetWorldViewRH(world, view);
}

// R5: 0x10073E50
HRESULT __stdcall R5FontSpriteProxy::SetWorldViewLH(const D3DXMATRIX *world, const D3DXMATRIX *view)
{
	return r5FontActualSprite->SetWorldViewLH(world, view);
}

// R5: 0x10073E60
HRESULT __stdcall R5FontSpriteProxy::Begin(DWORD flags)
{
	return r5FontActualSprite->Begin(flags);
}

// R5: 0x10073E70
HRESULT __stdcall R5FontSpriteProxy::Flush()
{
	return r5FontActualSprite->Flush();
}

// R5: 0x10073E80
HRESULT __stdcall R5FontSpriteProxy::End()
{
	return r5FontActualSprite->End();
}

// R5: 0x10073E90
HRESULT __stdcall R5FontSpriteProxy::OnLostDevice()
{
	return r5FontActualSprite->OnLostDevice();
}

// R5: 0x10073EA0
HRESULT __stdcall R5FontSpriteProxy::OnResetDevice()
{
	return r5FontActualSprite->OnResetDevice();
}

// R5: 0x10073EC0
R5FontSpriteProxy::R5FontSpriteProxy()
{
}
