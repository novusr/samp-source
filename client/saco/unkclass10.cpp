#include "main.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern CCmdWindow *pCmdWindow;
extern const int r5RpcEditObject;
extern const int r5RpcEditAttachedObject;

struct R5EditorObjectPoolView {
	int lastObjectId;
	BOOL active[1000];
	CObject *objects[1000];
	CObject *GetAt(WORD id) {
		if (id > 1000) return NULL;
		if (!active[id]) return NULL;
		return objects[id];
	}
};
struct R5EditorPoolsView { BYTE unknown[0xc]; R5EditorObjectPoolView *objects; };
struct R5EditorNetView { BYTE unknown[0x3de]; R5EditorPoolsView *pools; };

static __forceinline struc_97 *GetAttachedInfo(CPlayerPed *ped, int slot)
{
	if (slot < 0 || slot >= 10) return NULL;
	if (!ped->field_4C[slot]) return NULL;
	return &ped->field_74[slot];
}

// R5: 0x10071B60
CUnkClass10::CUnkClass10(IDirect3DDevice9 *pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	field_7C = 0;
	field_113 = -1;
	field_80 = 0;
	field_84 = 0;
	field_78 = 0;
	field_A3 = 0;
	field_A4 = 0;
	field_A5 = 0;
	field_A6 = 0;
	field_A2 = 0;
	field_88 = -1;
	field_9E = GetTickCount();

	D3DXCreateLine(pD3DDevice, &m_pD3DLine);

	D3DXCreateFont(m_pD3DDevice, 22, 0, FW_NORMAL, 1, FALSE,
		SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "SAMPAUX3", &m_pD3DFont1);
	D3DXCreateFont(m_pD3DDevice, 28, 0, FW_NORMAL, 1, FALSE,
		SYMBOL_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "SAMPAUX3", &m_pD3DFont2);
}

// R5: 0x100728B0
void CUnkClass10::SendObjectEdit(int response)
{
	R5EditorObjectPoolView *pool = ((R5EditorNetView*)pNetGame)->pools->objects;
	CObject *object = pool->GetAt(field_88);
	if (object) {
		if (response == 2 && (int)(GetTickCount() - field_9E) < 250) return;
		field_9E = GetTickCount();
		float angleX = field_92.X;
		float angleY = field_92.Y;
		float angleZ = field_92.Z;
		MATRIX4X4 matrix;
		((CEntity*)object)->GetMatrix(&matrix);
		RakNet::BitStream stream;
		if (field_8E) stream.Write1();
		else stream.Write0();
		stream.Write<WORD>(field_88);
		stream.Write(response);
		stream.Write(matrix.pos.X);
		stream.Write(matrix.pos.Y);
		stream.Write(matrix.pos.Z);
		stream.Write(angleX);
		stream.Write(angleY);
		stream.Write(angleZ);
		pNetGame->GetRakClient()->RPC((char*)&r5RpcEditObject, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
	}
}

// R5: 0x10072AC0
void CUnkClass10::SendAttachedEdit(int response)
{
	RakNet::BitStream stream;
	int slot = field_8A;
	struc_97 *info = GetAttachedInfo(pGame->FindPlayerPed(), slot);
	stream.Write(response);
	stream.Write(field_8A);
	stream.Write((const char*)info, 52);
	pNetGame->GetRakClient()->RPC((char*)&r5RpcEditAttachedObject, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
}

// R5: 0x10072BC0
void CUnkClass10::Close(BOOL accept)
{
	int response = 0;
	if (accept) response = 1;
	field_80 = FALSE;
	pGame->ToggleKeyInputsDisabled(0, TRUE);
	if (pCmdWindow->m_bEnabled) {
		pCmdWindow->Disable();
		pCmdWindow->Enable();
	}
	if (field_78 == 2) SendAttachedEdit(response);
	if (field_78 == 1) SendObjectEdit(response);
}
