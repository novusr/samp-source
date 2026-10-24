#pragma once

class CUnkClass12
{
private:
	BOOL field_0;
	DWORD field_4;
	WORD field_8;

public:
	CUnkClass12() {
		field_0 = 0;
		field_8 = -1;
		field_4 = -1;
	}

	void ClearAll();
	void UpdateHover();
	void Update();
	void Enable(DWORD color);
	void SendClick();
	void Disable();
	BOOL MsgProc(UINT message, WPARAM wParam, LPARAM lParam);

};
