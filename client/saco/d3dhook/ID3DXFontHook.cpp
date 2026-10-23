
#include "ID3DXFontHook.h"
#include "../main.h"

// Reconstructed actual R5 globals and helpers; no new fake scratch allocation.
extern ID3DXSprite *r5FontActualSprite;
extern DWORD r5FontEmbeddedColor;
extern int r5FontCharacterIndex;
extern wchar_t r5FontWideText[20000];
extern char *r5FontOriginalText;
extern char *r5FontStrippedText;
extern void R5FontConvertAnsiToWide(char *, wchar_t *, int);
extern void RemoveColorEmbedsFromString(char *);
class R5FontSpriteProxy;
extern R5FontSpriteProxy r5FontSpriteProxy;

// R5: 0x10073BE0
INT __stdcall ID3DXFontHook::DrawTextA(LPD3DXSPRITE pSprite, LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
 if (pSprite)
 {
  r5FontActualSprite=pSprite;
  r5FontEmbeddedColor=Color;
  r5FontCharacterIndex=0;
  if (strlen(pString)>100000)
   return 0;
  strcpy(r5FontOriginalText,pString);
  strcpy(r5FontStrippedText,pString);
  R5FontConvertAnsiToWide(r5FontOriginalText,r5FontWideText,20000);
  RemoveColorEmbedsFromString(r5FontStrippedText);
  return m_pD3DFont->DrawTextA(reinterpret_cast<ID3DXSprite *>(&r5FontSpriteProxy),
    r5FontStrippedText,strlen(r5FontStrippedText),pRect,Format,Color);
 }
 else
  return m_pD3DFont->DrawTextA(NULL,pString,Count,pRect,Format,Color);
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

