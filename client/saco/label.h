
#pragma once

class CLabel
{
private:
	IDirect3DDevice9 *m_pD3DDevice;
	ID3DXSprite *m_pD3DSprite;

public:
	CLabel(IDirect3DDevice9 *pD3DDevice);
	~CLabel();

	void OnLostDevice();
	void OnResetDevice();
	BOOL IsLineOfSightClear(float x, float y, float z);
	void Begin();
	void End();
	void Draw(D3DXVECTOR3 *position, char *text, DWORD color, BOOL shadow, bool testLOS);
};
