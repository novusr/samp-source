
#include "main.h"

extern CNetGame *pNetGame;
extern CGame *pGame;
extern CCmdWindow *pCmdWindow;
extern const int r5RpcClickTextdraw;

class R5TextDrawSelectionView {
	unsigned char unknown0[0x9A7];
	BYTE selectable;
	unsigned char unknown9a8[0x17];
	BYTE drawn;
	BYTE keyCodes;
	RECT bounds;
	BYTE highlighted;
	DWORD highlightColor;
public:
	RECT *GetSelectableRect() { if (selectable && drawn) return &bounds; return NULL; }
	void SetHighlight(bool value, DWORD color) { highlighted = value; highlightColor = color; }
};

class R5TextDrawSelectionPoolView {
	BOOL active[2304];
	R5TextDrawSelectionView *items[2304];
public:
	R5TextDrawSelectionView *GetAt(WORD id) { if (id >= 2304) return NULL; if (!active[id]) return NULL; return items[id]; }
};

struct R5TextDrawSelectionPoolsView { unsigned char unknown0[0x1c]; R5TextDrawSelectionPoolView *textdrawPool; };
struct R5TextDrawSelectionNetView { unsigned char unknown0[0x3de]; R5TextDrawSelectionPoolsView *pools; };

// R5: 0x100712B0
void CUnkClass12::ClearAll()
{
	if (pNetGame) {
		R5TextDrawSelectionPoolView *pool = ((R5TextDrawSelectionNetView*)pNetGame)->pools->textdrawPool;
		if (pool) {
			int x = 0;
			while (x != 2304) {
				R5TextDrawSelectionView *text = pool->GetAt((WORD)x);
				if (text) text->SetHighlight(false, 0);
				x++;
			}
		}
	}
}

// R5: 0x10071310
void CUnkClass12::UpdateHover()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(*(HWND*)0xC97C1C, &point);
	if (pNetGame) {
		R5TextDrawSelectionPoolView *pool = ((R5TextDrawSelectionNetView*)pNetGame)->pools->textdrawPool;
		if (pool) {
			int x = 0;
			field_8 = 0xFFFF;
			while (x != 2304) {
				R5TextDrawSelectionView *text = pool->GetAt((WORD)x);
				if (text) {
					text->SetHighlight(false, 0);
					RECT *rect = pool->GetAt((WORD)x)->GetSelectableRect();
					if (rect && PtInRect(rect, point)) {
						field_8 = (WORD)x;
						pool->GetAt((WORD)x)->SetHighlight(true, field_4);
					}
				}
				x++;
			}
		}
	}
}

// R5: 0x10071410
void CUnkClass12::Update()
{
	if (field_0) {
		pGame->ToggleKeyInputsDisabled(2, FALSE);
		pGame->DisplayHud(FALSE);
		UpdateHover();
	}
}

// R5: 0x10071440
void CUnkClass12::Enable(DWORD color)
{
	DWORD upper = (color >> 16) | (color & 0xFF0000);
	DWORD lower = (color << 16) | (color & 0xFF00);
	DWORD converted = (lower << 8) | (upper >> 8);
	field_0 = TRUE;
	field_4 = converted;
	field_8 = 0xFFFF;
}

// R5: 0x10071480
void CUnkClass12::SendClick()
{
	RakNet::BitStream stream;
	stream.Write<WORD>(field_8);
	pNetGame->GetRakClient()->RPC((char*)&r5RpcClickTextdraw, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
}

// R5: 0x10071520
void CUnkClass12::Disable()
{
	field_0 = FALSE;
	pGame->ToggleKeyInputsDisabled(0, FALSE);
	if (pCmdWindow->m_bEnabled) {
		pCmdWindow->Disable();
		pCmdWindow->Enable();
	}
	field_8 = 0xFFFF;
	SendClick();
	ClearAll();
}

// R5: 0x10071570
BOOL CUnkClass12::MsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (field_0 && message == WM_LBUTTONUP) {
		if (field_8 != 0xFFFF) SendClick();
		return TRUE;
	}
	return FALSE;
}
