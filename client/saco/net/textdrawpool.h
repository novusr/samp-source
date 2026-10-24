#pragma once

//----------------------------------------------------

#pragma pack(push, 1)

struct R5TextDrawTransmit
{
	BYTE flags;
	float letterWidth, letterHeight;
	DWORD letterColor;
	float lineWidth, lineHeight;
	DWORD boxColor;
	BYTE shadow, outline;
	DWORD backgroundColor;
	BYTE style, selectable;
	float x, y;
	WORD previewModel;
	float rotationX, rotationY, rotationZ, zoom;
	WORD color1, color2;
};

struct R5TextDrawConstructionData
{
	float letterWidth, letterHeight;
	DWORD letterColor;
	BYTE unknown12, centered, box;
	float lineWidth, lineHeight;
	DWORD boxColor;
	BYTE proportional;
	DWORD backgroundColor;
	BYTE shadow, outline, alignLeft, alignRight;
	DWORD style;
	float x, y;
	BYTE reservedGxt[8];
	DWORD parameter1, parameter2;
	int textureSlot;
	BYTE selectable;
	WORD previewModel;
	float rotationX, rotationY, rotationZ, zoom;
	WORD color1, color2;
};

class CTextDraw
{
public:
	char text[801];
	char expandedText[1602];
	R5TextDrawConstructionData data;
	BYTE reserved9be, drawn, keyCodes;
	RECT bounds;
	BYTE selected;
	DWORD selectedColor;

	CTextDraw(R5TextDrawTransmit *transmit, char *value);
	~CTextDraw();
	void ReleasePreviewTexture();
	void SetText(char *value);
	void LoadTexture();
};

#pragma pack(pop)

class CTextDrawPool
{
private:
	BOOL m_bSlotState[MAX_TEXT_DRAWS];
	CTextDraw *m_pTextDraw[MAX_TEXT_DRAWS];
public:
	CTextDrawPool();
	~CTextDrawPool();
	CTextDraw *New(WORD id, R5TextDrawTransmit *transmit, char *value);
	void Delete(WORD wText);
	void ReleasePreviewTextures();
};

//----------------------------------------------------
