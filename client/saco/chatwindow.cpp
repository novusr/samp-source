#include <time.h>

#include "main.h"

extern CConfig *pConfig;
extern CGame *pGame;
extern CCmdWindow *pCmdWindow;
extern void RemoveColorEmbedsFromString(char *text);

class R5ChatGameMenuView { public: BOOL IsMenuActive(); };
class R5ChatInputQueryView { public: IDirect3DDevice9 *device; CDXUTDialog *dialog; CDXUTIMEEditBox *edit; bool IsImeInteractionActive(); };

class R5ChatScrollBarNavigationView : public CDXUTScrollBar
{
public:
	void R5SetTrackPos(int position);
};

// R5: 0x100670E0
void R5ChatScrollBarNavigationView::R5SetTrackPos(int position)
{
	m_nPosition = position;
	Cap();
	UpdateThumbRect();
}

//----------------------------------------------------

CChatWindow::CChatWindow(IDirect3DDevice9 *pD3DDevice, CFontRender *pFontRender, CHAR *szChatLogFile)
{
	int x=0;

	m_pD3DDevice		= pD3DDevice;
	m_pFontRender		= pFontRender;
	field_8 = 2;

	// Create a sprite to use when drawing text
	D3DXCreateSprite(pD3DDevice,&field_63A6);
	D3DXCreateSprite(pD3DDevice,&field_63AA);

	// Init the chat window lines to 0
	while(x!=MAX_MESSAGES) {
		memset(&m_ChatWindowEntries[x],0,sizeof(CHAT_WINDOW_ENTRY));
		x++;
	}

	m_dwChatTextColor = D3DCOLOR_ARGB(255,255,255,255);
	m_dwChatInfoColor = D3DCOLOR_ARGB(255,136,170,98);
	m_dwChatDebugColor = D3DCOLOR_ARGB(255,169,196,228);

	field_0 = 10;
	field_C = 0;

	if(szChatLogFile && strlen(szChatLogFile))
	{
		memset(&field_11[0],0,sizeof(field_11));
		strncpy(field_11,szChatLogFile,MAX_PATH);

		FILE *f = fopen(field_11, "w");
		if(f)
		{
			field_D = 1;
			fclose(f);
		}
	}

	field_63BE = NULL;
	field_63BA = NULL;
	field_63B6 = NULL;
	field_63D2 = GetTickCount();
	field_63DE = 1;
	m_pScrollBar = NULL;
	m_pGameUI = NULL;
	field_11A = 0;

	CreateFonts();
}

//----------------------------------------------------

// R5: 0x10067120
void CChatWindow::MeasureDimensions()
{
	RECT rect;
	m_pFontRender->field_0->DrawTextA(NULL, "Y", -1, &rect, DT_CALCRECT | DT_SINGLELINE, 0xFF000000);
	field_63E2 = rect.bottom - rect.top;
	m_pFontRender->field_0->DrawTextA(NULL, "[19:58:34]", -1, &rect, DT_CALCRECT | DT_SINGLELINE, 0xFF000000);
	field_63E6 = rect.right - rect.left;
}

//----------------------------------------------------

// R5: 0x100671A0
void CChatWindow::ReleaseSurfaces()
{
	SAFE_RELEASE(field_63BE);
	SAFE_RELEASE(field_63BA);
	SAFE_RELEASE(field_63B6);
}

//----------------------------------------------------

// R5: 0x100681D0
void CChatWindow::CreateFonts()
{
	field_63B2 = 1;
	SAFE_RELEASE(field_63BE);
	SAFE_RELEASE(field_63BA);
	SAFE_RELEASE(field_63B6);
	m_pD3DDevice->GetDisplayMode(0, &field_63C2);
	if (pConfig && pConfig->GetIntVariable("directmode"))
	{
		AddDebugMessage("ChatWindow: Using direct drawing mode.");
		MeasureDimensions();
		field_63B2 = 0;
		return;
	}
	HRESULT result;
	if (field_63C2.Width <= 1024)
		result = D3DXCreateTexture(m_pD3DDevice, 1024, 512, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &field_63BA);
	else
		result = D3DXCreateTexture(m_pD3DDevice, 2048, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &field_63BA);
	if (FAILED(result))
	{
		AddDebugMessage("ChatWindow: Can't create a render surface texture. Will use direct mode.");
		field_63B2 = 0;
		return;
	}
	field_63BA->GetSurfaceLevel(0, &field_63BE);
	D3DSURFACE_DESC description;
	field_63BE->GetDesc(&description);
	result = D3DXCreateRenderToSurface(m_pD3DDevice, description.Width, description.Height, description.Format, TRUE, D3DFMT_D16, &field_63B6);
	if (FAILED(result))
	{
		AddDebugMessage("ChatWindow: Can't create a render to surface. Will use direct mode.");
		MeasureDimensions();
		field_63B2 = 0;
		return;
	}
	MeasureDimensions();
	field_63D6 = 0;
	field_63DA = 1;
}

//----------------------------------------------------
// MATCH
void CChatWindow::ResetDialogControls(CDXUTDialog *pGameUI)
{
	m_pGameUI = pGameUI;

	if(pGameUI) {
		m_pScrollBar = new CDXUTScrollBar(pGameUI);
		pGameUI->AddControl(m_pScrollBar);
		m_pScrollBar->SetVisible(true);
		m_pScrollBar->SetEnabled(true);

		FUNC_10067200();
	}
}

//----------------------------------------------------
//----------------------------------------------------

// R5: 0x100680F0
void CChatWindow::AddDebugMessage(CHAR * szFormat, ...)
{
    // Complete C++ reconstruction of the R5 formatting and sanitizing path.
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    va_list args;
    va_start(args, szFormat);
    vsprintf(buffer, szFormat, args);
    va_end(args);
    for(char *p = buffer; *p; ++p) {
        if(*p > 0 && *p < 32) *p = ' ';
    }
    AddEntry(8, buffer, NULL, m_dwChatDebugColor, 0);
}

// R5: 0x10068070
void CChatWindow::AddInfoMessage(CHAR * szFormat, ...)
{
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    va_list args;
    va_start(args, szFormat);
    vsprintf(buffer, szFormat, args);
    va_end(args);
    for(char *p = buffer; *p; ++p) {
        if(*p > 0 && *p < 32) *p = ' ';
    }
    AddEntry(4, buffer, NULL, m_dwChatInfoColor, 0);
}

// R5: 0x10067200
void CChatWindow::FUNC_10067200()
{
	if(m_pScrollBar) {
		m_pScrollBar->SetLocation(10,40);
		m_pScrollBar->SetSize(20,((field_63E2+1)*field_0)-60);
		m_pScrollBar->SetTrackRange(1,MAX_MESSAGES);
		m_pScrollBar->SetPageSize(field_0);
		m_pScrollBar->SetTrackPos(MAX_MESSAGES-field_0);
	}
}

// R5: 0x100672A0
void CChatWindow::SetPageSize(int nValue)
{
	if(nValue >= 10 && nValue <= 100) {
		field_0 = nValue;
		FUNC_10067200();
		field_63DA = 1;
	}
}

//----------------------------------------------------

// R5: 0x100672D0
void CChatWindow::PageUp()
{
	if(!field_8 || !m_pScrollBar) return;
	if(((R5ChatGameMenuView*)pGame)->IsMenuActive()) return;
	if(((R5ChatInputQueryView*)pCmdWindow)->IsImeInteractionActive()) return;
	int position = m_pScrollBar->GetTrackPos() - field_0;
	if(position < 1) position = 1;
	((R5ChatScrollBarNavigationView*)m_pScrollBar)->R5SetTrackPos(position);
}

//----------------------------------------------------

// R5: 0x10067330
void CChatWindow::PageDown()
{
	if(!field_8 || !m_pScrollBar) return;
	if(((R5ChatGameMenuView*)pGame)->IsMenuActive()) return;
	if(((R5ChatInputQueryView*)pCmdWindow)->IsImeInteractionActive()) return;
	int position = m_pScrollBar->GetTrackPos();
	if(position == 1) position = field_0;
	else position += field_0;
	if(position > 100) position = 100;
	((R5ChatScrollBarNavigationView*)m_pScrollBar)->R5SetTrackPos(position);
}


inline void CChatWindow::PushBack()
{
    memcpy(m_ChatWindowEntries, m_ChatWindowEntries + 1, sizeof(m_ChatWindowEntries));
}

// R5 copies all 100 entries from the next slot, including its original over-read.
// The prefix guard deliberately follows that copy.
void CChatWindow::AddEntry(int type, CHAR *text, CHAR *prefix, DWORD textColor, DWORD prefixColor)
{
    PushBack();
    if(prefix && strlen(prefix) > 24) return;
    m_ChatWindowEntries[99].type = type;
    m_ChatWindowEntries[99].textColor = textColor;
    m_ChatWindowEntries[99].prefixColor = prefixColor;
    m_ChatWindowEntries[99].timestamp = (DWORD)time(NULL);
    if(prefix) {
        strcpy(m_ChatWindowEntries[99].prefix, prefix);
        strcat(m_ChatWindowEntries[99].prefix, ":");
    } else m_ChatWindowEntries[99].prefix[0] = 0;
    Log(type, text, prefix);
    if(m_ChatWindowEntries[99].type == 2 && strlen(text) > 72) {
        int split = 72;
        while(text[split] != ' ' && split) --split;
        if(72 - split > 12) {
            strncpy(m_ChatWindowEntries[99].text, text, 72);
            m_ChatWindowEntries[99].text[72] = 0;
            PushBack();
            m_ChatWindowEntries[99].type = type;
            m_ChatWindowEntries[99].textColor = textColor;
            m_ChatWindowEntries[99].prefixColor = prefixColor;
            m_ChatWindowEntries[99].prefix[0] = 0;
            strcpy(m_ChatWindowEntries[99].text, text + 72);
        } else {
            strncpy(m_ChatWindowEntries[99].text, text, split);
            m_ChatWindowEntries[99].text[split] = 0;
            PushBack();
            m_ChatWindowEntries[99].type = type;
            m_ChatWindowEntries[99].textColor = textColor;
            m_ChatWindowEntries[99].prefixColor = prefixColor;
            m_ChatWindowEntries[99].prefix[0] = 0;
            strcpy(m_ChatWindowEntries[99].text, text + split + 1);
        }
    } else {
        strncpy(m_ChatWindowEntries[99].text, text, 144);
        m_ChatWindowEntries[99].text[144] = 0;
    }
    field_63DA = 1;
}

// R5: 0x100677D0
void CChatWindow::Log(int type, CHAR *text, CHAR *prefix)
{
    time_t now = time(NULL);
    if(!field_D) return;
    FILE *file = fopen(field_11, "a");
    if(!file) return;
    char stamp[65];
    strftime(stamp, 64, "[%H:%M:%S]", localtime(&now));
    switch(type) {
        case 2:
            fprintf(file, "%s <%s> %s\r\n", stamp, prefix, text);
            break;
        case 4:
        case 8:
            fprintf(file, "%s %s\r\n", stamp, text);
            break;
    }
    fclose(file);
}

// R5: 0x10067390
void CChatWindow::ResetPage()
{
    if(m_pScrollBar) m_pScrollBar->SetTrackPos(90);
}

//----------------------------------------------------

// R5: 0x100673C0
void CChatWindow::ScrollWheel(int delta)
{
	if(field_8 && m_pScrollBar) {
		UINT lines;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &lines, 0);
		m_pScrollBar->Scroll(-(delta * (int)lines));
	}
}

//----------------------------------------------------

// R5: 0x10067420
void CChatWindow::FilterInvalidChars(char *text)
{
	while(*text) {
		if(*text > 0 && *text < ' ') {
			*text = ' ';
		}
		text++;
	}
}

//----------------------------------------------------

// R5: 0x10067470
void CChatWindow::RenderText(char *text, RECT rect, DWORD color)
{
	ID3DXFont *shadowFont = m_pFontRender->field_8;
	ID3DXFontHook *textFont = m_pFontRender->field_0;
	int textLength = strlen(text);

	if (field_8 == 2)
	{
		char stripped[512] = {0};
		strncpy(stripped, text, 511);
		RemoveColorEmbedsFromString(stripped);
		int shadowLength = strlen(stripped);
		const DWORD NativeScreenWidth = 0xC17044;

		if (*(int*)NativeScreenWidth > 1280)
		{
			rect.top -= 2; rect.bottom -= 2;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.top += 1; rect.bottom += 1;
			rect.left -= 1; rect.right -= 1;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left += 1; rect.right += 1;
			rect.left += 1; rect.right += 1;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left -= 1; rect.right -= 1;
			rect.top += 1; rect.bottom += 1;
			rect.top += 2; rect.bottom += 2;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.top -= 1; rect.bottom -= 1;
			rect.left -= 1; rect.right -= 1;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left += 1; rect.right += 1;
			rect.left += 1; rect.right += 1;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left -= 1; rect.right -= 1;
			rect.top -= 1; rect.bottom -= 1;
			rect.left -= 2; rect.right -= 2;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left += 2; rect.right += 2;
			rect.left += 2; rect.right += 2;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left -= 2; rect.right -= 2;
		}
		else
		{
			rect.top -= 1;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.top += 2;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.top -= 1; rect.left -= 1;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left += 2;
			shadowFont->DrawTextA(field_63A6, stripped, shadowLength, &rect, DT_NOCLIP | DT_SINGLELINE, 0xFF000000);
			rect.left -= 1;
		}
	}

	textFont->DrawTextA(field_63A6, text, textLength, &rect, DT_NOCLIP | DT_SINGLELINE, color | 0xFF000000);
}
