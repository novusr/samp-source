
#pragma once

class CScoreBoard
{
private:
	BOOL enabled;
	int reserved4;
	float left;
	float top;
	float scale;
	float width;
	float height;
	float headerHeight;
	int reserved20;
	float column1;
	float column2;
	float column3;
	IDirect3DDevice9 *device;
	CDXUTDialog *dialog;
	CDXUTListBox *listbox;
	int reserved3C;
	int reserved40;

public:
	CScoreBoard(IDirect3DDevice9 *pD3DDevice);

	void FUNC_1006E930();
	void ResetDialogControls(CDXUTDialog *pDialog);
	void UpdatePlayers();

	void GetRect(RECT *rect);
	void Hide(bool restoreInputs);
	void ClickSelected();
	BOOL IsNPC(WORD playerId);
	int GetCount(BOOL includeNPC = FALSE);
	int GetScore(WORD playerId);
	int GetPing(WORD playerId);
};
