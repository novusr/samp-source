
#include "main.h"

// R5: 0x1006D9B0
CUnkClass13::CUnkClass13(IDirect3DDevice9 *pD3DDevice)
{
	field_20 = 0;
	m_pD3DSurface = NULL;
	m_pD3DTexture = NULL;
	m_pD3DRenderToSurface = NULL;
	m_pD3DDevice = pD3DDevice;

	RestoreDeviceObjects();
}

// R5: 0x1006D9E0
CUnkClass13::~CUnkClass13()
{
	DeleteDeviceObjects();
}

// R5: 0x1006D7B0
void CUnkClass13::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pD3DSurface);
	SAFE_RELEASE(m_pD3DTexture);
	SAFE_RELEASE(m_pD3DRenderToSurface);
	SAFE_RELEASE(field_20);
}

// R5: 0x1006D800
void CUnkClass13::RestoreDeviceObjects()
{
	m_pD3DDevice->GetDisplayMode(0, &m_DisplayMode);

	HRESULT hr = D3DXCreateTexture(m_pD3DDevice, 128, 32, 1,
		D3DUSAGE_RENDERTARGET, m_DisplayMode.Format, D3DPOOL_DEFAULT, &m_pD3DTexture);
	if(SUCCEEDED(hr))
	{
		D3DSURFACE_DESC desc;

		m_pD3DTexture->GetSurfaceLevel(0, &m_pD3DSurface);
		m_pD3DSurface->GetDesc(&desc);
		
		D3DXCreateRenderToSurface(m_pD3DDevice, desc.Width, desc.Height,
			desc.Format, TRUE, D3DFMT_D16, &m_pD3DRenderToSurface);
	}
}
extern CFontRender *pDefaultFont;

// R5: 0x1006D880
IDirect3DTexture9 *CUnkClass13::RenderTexture(char *plateText)
{
	if (!m_pD3DDevice || !pDefaultFont || !m_pD3DRenderToSurface || !m_pD3DTexture || !m_pD3DSurface)
		return NULL;

	IDirect3DTexture9 *result = NULL;
	HRESULT hr = D3DXCreateTexture(m_pD3DDevice, 64, 32, 1, 0, m_DisplayMode.Format, D3DPOOL_DEFAULT, &result);
	if (FAILED(hr)) return NULL;

	m_pD3DRenderToSurface->BeginScene(m_pD3DSurface, NULL);
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFBEB6A8, 1.0f, 0);

	RECT textRect = { 0, 3, 128, 32 };
	pDefaultFont->RenderText(plateText, textRect, 0xEE444470);
	m_pD3DRenderToSurface->EndScene(0);

	result->SetAutoGenFilterType(D3DTEXF_LINEAR);

	IDirect3DSurface9 *outputSurface = NULL;
	result->GetSurfaceLevel(0, &outputSurface);
	D3DXLoadSurfaceFromSurface(outputSurface, NULL, NULL, m_pD3DSurface, NULL, NULL, (DWORD)textRect.top, 0);
	outputSurface->Release();

	return result;
}
