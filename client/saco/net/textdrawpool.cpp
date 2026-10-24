#include "../main.h"
#include "../game/textures.h"
#include "../game/util.h"
#include <stddef.h>
#include <stdlib.h>

typedef char r5_transmit63[(sizeof(R5TextDrawTransmit) == 63) ? 1 : -1];
typedef char r5_construction_data91[(sizeof(R5TextDrawConstructionData) == 91) ? 1 : -1];
typedef char r5_textdraw_allocation9d6[(sizeof(CTextDraw) == 0x9d6) ? 1 : -1];
typedef char r5_textdraw_data963[(offsetof(CTextDraw, data) == 0x963) ? 1 : -1];
typedef char r5_textdraw_bounds9c1[(offsetof(CTextDraw, bounds) == 0x9c1) ? 1 : -1];

struct R5CustomModelEntryView {
	unsigned char unknown0[7];
	BYTE type;
	unsigned char unknown8[8];
	int modelId;
	unsigned char unknown14[0x36];
	DWORD textureId;
	unsigned char unknown4e[10];
	bool ready;
};

class R5CustomModelManagerView {
	R5CustomModelEntryView **items;
	unsigned int count;
public:
	bool HasTextureModel(int id);
	bool IsModelReady(int id);
	DWORD GetTextureId(int id);
};

struct R5CustomModelLoadEntryView {
	BYTE present;
	BYTE downloaded;
	unsigned char unknown2[5];
	BYTE type;
	unsigned char unknown8[4];
	int baseModel;
	int modelId;
	unsigned char unknown14[0x32];
	DWORD dffChecksum;
	DWORD txdChecksum;
	unsigned char unknown4e[10];
	bool ready;
	bool retry;
};

class R5CustomModelLoadView {
	R5CustomModelLoadEntryView **items;
	unsigned int count;
	unsigned char unknown8;
	char primaryPath[261];
	char secondaryPath[261];
public:
	BOOL LoadModelFiles(int baseModel, int model, DWORD dffChecksum, DWORD txdChecksum);
	BOOL EnsureModelReady(int model);
};

struct RPCCustomModelDownloadWorldView;
extern RPCCustomModelDownloadWorldView *rpc_custom_model_download_manager;

DWORD r5TextdrawTextures[200];
BOOL r5TextdrawTextureUsed[200];

//----------------------------------------------------

// R5: 0x100B2AF0
BOOL InitializeTextdrawTextures()
{
	memset(r5TextdrawTextureUsed, 0, sizeof(BOOL) * 200);
	memset(r5TextdrawTextures, 0, sizeof(DWORD) * 200);
	return TRUE;
}

//----------------------------------------------------

// R5: 0x100B2B20
int AllocateTextdrawTexture()
{
	int x = 0;
	while (x != 200) {
		if (!r5TextdrawTextureUsed[x]) {
			r5TextdrawTextureUsed[x] = TRUE;
			return x;
		}
		x++;
	}
	return -1;
}

//----------------------------------------------------

// R5: 0x100B2B50
void ReleaseTextdrawTexture(int slot)
{
	if (slot >= 0 && slot < 200) {
		r5TextdrawTextureUsed[slot] = FALSE;
		if (r5TextdrawTextures[slot]) DestroyTexture(r5TextdrawTextures[slot]);
		r5TextdrawTextures[slot] = 0;
	}
}

//----------------------------------------------------

// R5: 0x100B2B90
BOOL EnsureTextdrawDictionary(char *dictionary, char *file, bool sampDirectory)
{
	char directory[65];
	char filename[129];
	if (sampDirectory) strcpy(directory, "SAMP\\");
	else strcpy(directory, "models\\txd");
	int slot = FindTextureSlot(dictionary);
	if (slot == -1) {
		slot = AddTextureSlot(dictionary);
		sprintf(filename, "%s\\%s.txd", directory, file);
		if (IsFileOrDirectoryExists(filename)) {
			LoadTexture(slot, filename);
			AddTextureRef(slot);
		} else {
			return FALSE;
		}
	}
	return TRUE;
}

//----------------------------------------------------

// R5: 0x100B2C60
DWORD ReadTextdrawDictionaryTexture(char *dictionary, char *texture)
{
	int slot = FindTextureSlot(dictionary);
	if (slot == -1) return 0;
	PushCurrentTexture();
	SetCurrentTexture(slot);
	DWORD result = ReloadTexture(texture);
	PopCurrentTexture();
	return result;
}

//----------------------------------------------------

// R5: 0x100B2CD0
void CTextDraw::LoadTexture()
{
	__declspec(align(8)) char dictionary[65];
	__declspec(align(8)) char texture[65];
	__declspec(align(8)) char dictionaryName[65];
	memset(dictionary, 0, sizeof(dictionary));
	memset(texture, 0, sizeof(texture));
	memset(dictionaryName, 0, sizeof(dictionaryName));
	char *colon = strchr(text, ':');
	if (!colon || strlen(text) >= 64 || strchr(text, '\\') || strchr(text, '/')) return;
	strncpy(dictionary, text, colon - text);
	strcpy(texture, colon + 1);
	if (data.textureSlot == -1) return;
	if (!strcmp(dictionary, "hud")) {
		strcpy(dictionaryName, "hud");
	} else if (!strcmp(dictionary, "samaps")) {
		strcpy(dictionaryName, "samaps");
		if (!EnsureTextdrawDictionary(dictionaryName, "samaps", true)) return;
	} else if (!strcmp(dictionary, "vehicleprev")) {
		strcpy(dictionaryName, "vehicleprev");
		if (!EnsureTextdrawDictionary(dictionaryName, "vehicles128", true)) return;
	} else if (rpc_custom_model_download_manager && strlen(dictionary) < 10 && !strncmp(dictionary, "mdl", 3)) {
		int model = atoi(dictionary + 3);
		if (!((R5CustomModelManagerView*)rpc_custom_model_download_manager)->HasTextureModel(model)) return;
		if (!((R5CustomModelManagerView*)rpc_custom_model_download_manager)->IsModelReady(model)) {
			if (!((R5CustomModelLoadView*)rpc_custom_model_download_manager)->EnsureModelReady(model)) return;
		}
		DWORD textureId = ((R5CustomModelManagerView*)rpc_custom_model_download_manager)->GetTextureId(model);
		if (!textureId) return;
		sprintf(dictionaryName, "%X", textureId);
	} else {
		strncpy(dictionaryName, dictionary, 10);
		if (!EnsureTextdrawDictionary(dictionaryName, dictionary, false)) return;
	}
	r5TextdrawTextures[data.textureSlot] = ReadTextdrawDictionaryTexture(dictionaryName, texture);
}

//----------------------------------------------------

// R5: 0x100B2F50
CTextDraw::~CTextDraw()
{
	ReleaseTextdrawTexture(data.textureSlot);
}

//----------------------------------------------------

// R5: 0x100B2F60
void CTextDraw::SetText(char *value)
{
	memset(text, 0, 800);
	if (strlen(value) >= 800) return;
	strncpy(text, value, 800);
	text[800] = 0;
	keyCodes = 0;
	bool inside = false;
	int tokenLength = 0;
	char *cursor = text;
	bool valid = true;
	while (*cursor) {
		if (*cursor == '~') {
			if (!inside) {
				inside = true;
				if (cursor[1] == 'k' || cursor[1] == 'K') keyCodes = 1;
			} else {
				tokenLength = 0;
				inside = false;
			}
		}
		if (inside) {
			tokenLength++;
			if (tokenLength > 24) valid = false;
		}
		cursor++;
	}
	if (!valid || inside) {
		strcpy(text, "Error: unmatched tilde");
		keyCodes = 0;
	}
	if (keyCodes && strlen(text) > 255) {
		strcpy(text, "Error: can't use k-codes in long string");
		keyCodes = 0;
	}
	if (data.style == 4 && data.textureSlot != -1) {
		ReleaseTextdrawTexture(data.textureSlot);
		data.textureSlot = AllocateTextdrawTexture();
		LoadTexture();
	}
}

//----------------------------------------------------

// R5: 0x100B36B0
void CTextDraw::ReleasePreviewTexture()
{
	if (data.style == 5) {
		if (data.textureSlot != -1) {
			ReleaseTextdrawTexture(data.textureSlot);
			data.textureSlot = -1;
		}
	}
}

//----------------------------------------------------

// R5: 0x100B36E0
CTextDraw::CTextDraw(R5TextDrawTransmit *transmit, char *value)
{
	memset(&data, 0, sizeof(data));
	data.letterWidth = transmit->letterWidth;
	data.letterHeight = transmit->letterHeight;
	data.letterColor = transmit->letterColor;
	data.unknown12 = 0;
	data.centered = (transmit->flags >> 3) & 1;
	data.box = transmit->flags & 1;
	data.lineWidth = transmit->lineWidth;
	data.lineHeight = transmit->lineHeight;
	data.boxColor = transmit->boxColor;
	data.proportional = (transmit->flags >> 4) & 1;
	data.backgroundColor = transmit->backgroundColor;
	data.shadow = transmit->shadow;
	data.outline = transmit->outline;
	data.alignLeft = (transmit->flags >> 1) & 1;
	data.alignRight = (transmit->flags >> 2) & 1;
	data.style = transmit->style;
	data.x = transmit->x;
	data.y = transmit->y;
	data.parameter1 = 0xFFFFFFFF;
	data.parameter2 = 0xFFFFFFFF;
	data.selectable = transmit->selectable;
	data.previewModel = transmit->previewModel;
	data.rotationX = transmit->rotationX;
	data.rotationY = transmit->rotationY;
	data.rotationZ = transmit->rotationZ;
	data.zoom = transmit->zoom;
	data.color1 = transmit->color1;
	data.color2 = transmit->color2;
	keyCodes = 0;
	data.textureSlot = -1;
	SetText(value);
	if (data.style == 4) {
		data.textureSlot = AllocateTextdrawTexture();
		LoadTexture();
	}
	drawn = 0;
	bounds.left = 0;
	bounds.right = 0;
	bounds.top = 0;
	bounds.bottom = 0;
	selected = 0;
	selectedColor = 0;
}

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
	InitializeTextdrawTextures();
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
	if (m_pTextDraw[wText]) {
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
		if (m_bSlotState[x]) {
			m_pTextDraw[x]->ReleasePreviewTexture();
		}
		x++;
	}
}

//----------------------------------------------------

// R5: 0x1001E910
CTextDraw *CTextDrawPool::New(WORD id, R5TextDrawTransmit *transmit, char *value)
{
	if (m_pTextDraw[id]) Delete(id);
	CTextDraw *result = new CTextDraw(transmit, value);
	if (result) {
		m_pTextDraw[id] = result;
		m_bSlotState[id] = TRUE;
		return result;
	}
	return NULL;
}

//----------------------------------------------------
