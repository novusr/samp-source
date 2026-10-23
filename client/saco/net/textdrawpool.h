#pragma once

//----------------------------------------------------

class CTextDraw
{
public:
	~CTextDraw();
	void ReleasePreviewTexture();
};

class CTextDrawPool
{
private:
	BOOL m_bSlotState[MAX_TEXT_DRAWS];
	CTextDraw *m_pTextDraw[MAX_TEXT_DRAWS];
public:
	CTextDrawPool();
	~CTextDrawPool();
	void Delete(WORD wText);
	void ReleasePreviewTextures();
};

//----------------------------------------------------
