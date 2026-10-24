#include "main.h"

extern CGame *pGame;
extern CCmdWindow *pCmdWindow;

struct R5InputRestoreCountdownView { BYTE unknown[0x65]; int countdown; };

// R5: 0x1006DA00
CUnkClass11::CUnkClass11()
{
	field_0 = 0;
	field_4 = -1;
}

// R5: 0x1006DB80
void CUnkClass11::Toggle(BOOL enable)
{
	if (field_0 && !enable) {
		pGame->ToggleKeyInputsDisabled(enable, TRUE);
		if (pCmdWindow->m_bEnabled) {
			pCmdWindow->Disable();
			pCmdWindow->Enable();
		}
	}
	if (!field_0 && enable) {
		pGame->ToggleKeyInputsDisabled(0, TRUE);
		((R5InputRestoreCountdownView*)pGame)->countdown = 0;
		pGame->ProcessInputDisabling();
	}
	field_0 = enable;
}
