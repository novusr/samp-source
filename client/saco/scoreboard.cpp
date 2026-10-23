
#include "main.h"

extern CGame *pGame;
extern CNetGame *pNetGame;
extern const int r5RpcClickPlayer;

#pragma pack(push, 1)
struct PlayerPoolView
{
	BYTE prefix[0x2A];
	BOOL slots[MAX_PLAYERS];
	BYTE middleGap[4016];
	CNetPlayer *players[MAX_PLAYERS];

	BOOL GetSlotState(PLAYERID id)
	{
		if (id >= MAX_PLAYERS) return FALSE;
		return slots[id];
	}
};
#pragma pack(pop)

// R5: 0x1006EA30
CScoreBoard::CScoreBoard(IDirect3DDevice9 *pD3DDevice)
{
	device = pD3DDevice;
	dialog = NULL;
	listbox = NULL;
	reserved3C = 0;
	reserved40 = 0;
	reserved4 = 0;

	if (pGame && pGame->GetScreenWidth() > 800)
	{
		width = 800.0f;
		height = 600.0f;
	}
	else
	{
		width = 640.0f;
		height = 480.0f;
	}
	headerHeight = 60.0f;
	reserved20 = 0;
	column1 = 0.085f;
	column2 = 0.265625f;
	column3 = 0.437500f;

	FUNC_1006E930();

	enabled = 0;
}

// R5: 0x1006E930
void CScoreBoard::FUNC_1006E930()
{
	RECT rect;

	GetClientRect(pGame->GetMainWindowHwnd(), &rect);

	scale = 1.0f;
	left = rect.right * 0.5f - width * 0.5f;
	top = rect.bottom * 0.5f - height * 0.5f;
}

void CScoreBoard::ResetDialogControls(CDXUTDialog *pDialog)
{
	dialog = pDialog;

	if (pDialog) {
		pDialog->SetSize(width, height);

		listbox = new CDXUTListBox(dialog);
		dialog->AddControl(listbox);
		listbox->SetLocation(0, headerHeight);
		listbox->SetSize(width, height - headerHeight);
		listbox->OnInit();
		listbox->GetElement(0)->TextureColor.Init(D3DCOLOR_ARGB( 200, 255, 255, 255 ));
		listbox->field_4D = 3;
		listbox->field_51 = column1 * width;
		listbox->field_59 = column2 * width;
		listbox->field_55 = column3 * width;
		listbox->SetEnabled(false);
		listbox->SetVisible(false);

		FUNC_1006E930();
	}
}

// R5: 0x1006E990 (CScoreBoard::GetRect)
void CScoreBoard::GetRect(RECT *rect)
{
	rect->left = (int)left;
	rect->right = rect->left + (int)width;
	rect->top = (int)top;
	rect->bottom = rect->top + (int)height;
}

// R5: 0x1006E9E0 (CScoreBoard::Hide)
void CScoreBoard::Hide(bool restoreInputs)
{
	if (enabled && dialog)
	{
		dialog->SetVisible(false);
		listbox->SetEnabled(false);
		listbox->SetVisible(false);
		if (restoreInputs) pGame->ToggleKeyInputsDisabled(0, FALSE);
		enabled = FALSE;
	}
}

// R5: 0x1006EC10 (CScoreBoard::ClickSelected)
void CScoreBoard::ClickSelected()
{
	if (enabled && dialog)
	{
		CDXUTListBox *selectedList = listbox;
		DXUTListBoxItem *item = selectedList->GetItem(selectedList->GetSelectedIndex());
		if (item)
		{
			// Only the actual inline text beginning at item offset0 is used.
			// No claim about the rest of R5's larger item layout.
			int playerId = atoi((char*)item);
			RakNet::BitStream stream;
			stream.Write<WORD>((WORD)playerId);
			stream.Write<BYTE>(0);
			pNetGame->GetRakClient()->RPC((char*)&r5RpcClickPlayer, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
		}
		Hide(true);
	}
}

// R5: 0x1000E470 (CScoreBoard::IsNPC)
BOOL CScoreBoard::IsNPC(WORD playerId)
{
	if (!pNetGame || !pNetGame->GetPlayerPool()) return FALSE;
	if (playerId > MAX_PLAYERS) return FALSE;
	PlayerPoolView *pool = (PlayerPoolView*)pNetGame->GetPlayerPool();
	CNetPlayer *player = pool->players[playerId];
	if (!player) return FALSE;
	return player->m_bIsNPC;
}

// R5: 0x100139F0 (CScoreBoard::GetCount)
int CScoreBoard::GetCount(BOOL includeNPC)
{
	if (!pNetGame || !pNetGame->GetPlayerPool()) return 0;
	PlayerPoolView *pool = (PlayerPoolView*)pNetGame->GetPlayerPool();
	int count = 0;
	if (includeNPC)
	{
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (pool->GetSlotState((PLAYERID)i) == TRUE) ++count;
		}
	}
	else
	{
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (pool->GetSlotState((PLAYERID)i) == TRUE && !pool->players[(PLAYERID)i]->m_bIsNPC) ++count;
		}
	}
	return count;
}

// R5: 0x1006E850 (CScoreBoard::GetScore)
int CScoreBoard::GetScore(WORD playerId)
{
	if (!pNetGame || !pNetGame->GetPlayerPool()) return 0;
	if (playerId > MAX_PLAYERS) return 0;
	PlayerPoolView *pool = (PlayerPoolView*)pNetGame->GetPlayerPool();
	CNetPlayer *player = pool->players[playerId];
	if (!player) return 0;
	return player->field_4;
}

// R5: 0x1006E880 (CScoreBoard::GetPing)
int CScoreBoard::GetPing(WORD playerId)
{
	if (!pNetGame || !pNetGame->GetPlayerPool()) return 0;
	if (playerId > MAX_PLAYERS) return 0;
	PlayerPoolView *pool = (PlayerPoolView*)pNetGame->GetPlayerPool();
	CNetPlayer *player = pool->players[playerId];
	if (!player) return 0;
	return player->field_C;
}
