
#include "main.h"

extern CFontRender *pDefaultFont;
extern D3DXMATRIX matView, matProj;
extern const SCRIPT_COMMAND r5LabelLineOfSight;

// R5: 0x1006BBB0 (CLabel::CLabel)
CLabel::CLabel(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	D3DXCreateSprite(pD3DDevice, &m_pD3DSprite);
}

// R5: 0x1006BBD0 (CLabel::~CLabel)
CLabel::~CLabel()
{
	SAFE_DELETE(m_pD3DSprite);
}

// R5: 0x1006BBF0 (CLabel::OnLostDevice)
void CLabel::OnLostDevice()
{
	if (m_pD3DSprite) m_pD3DSprite->OnLostDevice();
}

// R5: 0x1006BC00 (CLabel::OnResetDevice)
void CLabel::OnResetDevice()
{
	if (m_pD3DSprite) m_pD3DSprite->OnResetDevice();
}

// R5: 0x1006BC10 (CLabel::IsLineOfSightClear)
BOOL CLabel::IsLineOfSightClear(float x, float y, float z)
{
	CAMERA_AIM *aim = GameGetInternalAim();
	if (aim && ScriptCommand(&r5LabelLineOfSight, x, y, z,
		aim->pos1x, aim->pos1y, aim->pos1z, 1, 0, 0, 1, 0)) return TRUE;
	return FALSE;
}

// R5: 0x1006BC70 (CLabel::Begin)
void CLabel::Begin()
{
	if (m_pD3DSprite) m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

// R5: 0x1006BC80 (CLabel::End)
void CLabel::End()
{
	if (m_pD3DSprite) m_pD3DSprite->End();
}

// R5: 0x1006BC90 (CLabel::Draw)
void CLabel::Draw(D3DXVECTOR3 *position, char *text, DWORD color, BOOL shadow, bool testLOS)
{
	if (!m_pD3DDevice) return;
	if (testLOS && !IsLineOfSightClear(position->x, position->y, position->z)) return;

	D3DVIEWPORT9 viewport;
	m_pD3DDevice->GetViewport(&viewport);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	D3DXVECTOR3 projected;
	D3DXVec3Project(&projected, position, &viewport, &matProj, &matView, &world);
	if (projected.z > 1.0f) return;

	RECT rect;
	rect.left = (LONG)projected.x;
	rect.top = (LONG)projected.y;
	rect.right = rect.left + 1;
	rect.bottom = rect.top + 1;

	pDefaultFont->RenderText(m_pD3DSprite, text, rect, 0x101, color, shadow);
}
