#include "../main.h"

BOOL unnamed_100B2AF0();

//----------------------------------------------------

// R5: 0x1001E7A0
CTextDrawPool::CTextDrawPool()
{
	int x = 0;
	while (x != MAX_TEXT_DRAWS) {
		m_pTextDraw[x] = NULL;
		m_bSlotState[x] = FALSE;
		x++;
	}
	unnamed_100B2AF0();
}

//----------------------------------------------------

// R5: 0x1001E8D0
CTextDrawPool::~CTextDrawPool()
{
	int x = 0;
	while (x != MAX_TEXT_DRAWS) {
		if (m_pTextDraw[x]) {
			delete m_pTextDraw[x];
			m_pTextDraw[x] = NULL;
			m_bSlotState[x] = FALSE;
		}
		x++;
	}
}

//----------------------------------------------------

// R5: 0x1001E7F0
void CTextDrawPool::Delete(WORD wText)
{
	if (wText < MAX_TEXT_DRAWS && m_pTextDraw[wText]) {
		delete m_pTextDraw[wText];
		m_pTextDraw[wText] = NULL;
		m_bSlotState[wText] = FALSE;
	}
}

//----------------------------------------------------

// R5: 0x1001E8A0
void CTextDrawPool::ReleasePreviewTextures()
{
	int x = 0;
	while (x != MAX_TEXT_DRAWS) {
		if (m_bSlotState[x] && m_pTextDraw[x]) {
			m_pTextDraw[x]->ReleasePreviewTexture();
		}
		x++;
	}
}

//----------------------------------------------------
