// GTA absolute addresses are part of R5's original behavior.
// These functions must not be invoked in an ordinary host process.
extern "C" void __cdecl Unprotect(void* address, unsigned long bytes);
#pragma pack(push, 1)
class Game {
public:
    char prefix[0x69];
    int weatherLocked;
    void SetWeather(int weather);
    void SetGravity(float gravity);
};
#pragma pack(pop)

void Game::SetWeather(int weather) {
    if (weatherLocked) {
        *(int*)0xc81318 = weather;
    } else {
        *(int*)0xc81318 = weather;
        *(int*)0xc8131c = weather;
        *(int*)0xc81320 = weather;
    }
}
void Game::SetGravity(float gravity) {
    Unprotect((void*)0x863984, 4);
    *(float*)0x863984 = gravity;
}

class CCryptoContext {
public:
    unsigned long m_dwProvider;
    unsigned long GetProvider();
};

unsigned long CCryptoContext::GetProvider() {
    return m_dwProvider;
}

#pragma pack(push, 1)
class CMenu {
public:
    unsigned char m_byteMenuID;
    char m_charTitle[33];
    char m_charItems[12][2][33];
    char m_charHeader[2][33];

    char* GetMenuTitle();
    char* GetMenuItem(unsigned char byteColumn, unsigned char byteRow);
    char* GetMenuHeader(unsigned char byteColumn);
};

char* CMenu::GetMenuTitle() {
    return m_charTitle;
}

char* CMenu::GetMenuItem(unsigned char byteColumn, unsigned char byteRow) {
    return m_charItems[byteRow][byteColumn];
}

char* CMenu::GetMenuHeader(unsigned char byteColumn) {
    return m_charHeader[byteColumn];
}

struct VEHICLE_TYPE {
    char gap0[0x22];
    unsigned short nModelIndex;     // 0x22
    char gap_flags[0x404];          // 0x24 to 0x428
    unsigned char byteFlags;        // 0x428
    char gap_siren[4];              // 0x429 to 0x42d
    unsigned char byteSiren;        // 0x42d
    char gap_alarm[0x2e];           // 0x42e to 0x45c
    unsigned short wAlarmState;     // 0x45c
    char gap1[0x62];
    float fHealth;                  // 0x4c0
    char gap2[0x3a8];
    unsigned long dwHydraThrusters; // 0x86c
    char gap3[0xdc];
    float fTankRotX;                // 0x94c
    float fTankRotY;                // 0x950
};

class CVehicle {
public:
    char gap[0x48];
    CVehicle* m_pTrailer;           // 0x48
    VEHICLE_TYPE* m_pVehicle;       // 0x4c

    float GetTankRotX();
    float GetTankRotY();
    unsigned long GetHydraThrusters();
    void SetHydraThrusters(unsigned long dwDirection);
    void SetHealth(float fHealth);
    void SetAlarmState(unsigned short wState);
    void SetTrailer(CVehicle* pTrailer);
    void SetTankRot(float X, float Y);
    int IsATrainPart();
    void SetEngineState(int bState);
    void SetSirenOn(unsigned char byteState);
    int IsSirenOn();
};

float CVehicle::GetTankRotX() {
    return m_pVehicle->fTankRotX;
}

float CVehicle::GetTankRotY() {
    return m_pVehicle->fTankRotY;
}

unsigned long CVehicle::GetHydraThrusters() {
    if (m_pVehicle) return m_pVehicle->dwHydraThrusters;
    return 0;
}

void CVehicle::SetHydraThrusters(unsigned long dwDirection) {
    if (!m_pVehicle) return;
    m_pVehicle->dwHydraThrusters = dwDirection;
}

void CVehicle::SetHealth(float fHealth) {
    if (!m_pVehicle) return;
    m_pVehicle->fHealth = fHealth;
}

void CVehicle::SetAlarmState(unsigned short wState) {
    m_pVehicle->wAlarmState = wState;
}

void CVehicle::SetTrailer(CVehicle* pTrailer) {
    m_pTrailer = pTrailer;
}

void CVehicle::SetTankRot(float X, float Y) {
    m_pVehicle->fTankRotX = X;
    m_pVehicle->fTankRotY = Y;
}

int CVehicle::IsATrainPart() {
    if (m_pVehicle) {
        int nModel = m_pVehicle->nModelIndex;
        if (nModel == 0x21a || nModel == 0x23a || nModel == 0x219 || nModel == 0x239 || nModel == 0x1c1)
            return 1;
    }
    return 0;
}

void CVehicle::SetEngineState(int bState) {
    if (!m_pVehicle) return;
    if (!bState) {
        m_pVehicle->byteFlags &= 0xdf;
    } else {
        m_pVehicle->byteFlags |= 0x20;
    }
}

void CVehicle::SetSirenOn(unsigned char byteState) {
    m_pVehicle->byteSiren = (m_pVehicle->byteSiren & 0x7f) | (byteState << 7);
}

int CVehicle::IsSirenOn() {
    return (m_pVehicle->byteSiren & 0x80) == 0x80;
}

class CGame {
public:
    int GetWorldWeather();
    unsigned char IsHudEnabled();
    int GetLocalMoney();
    int GetMemoryUsedForStreaming();
    int GetMemoryAvailableForStreaming();
    unsigned long GetTimeInMilliseconds();
    int GetRwObjectsCount();
    void EnableFrameLimiter();
    int IsGameLoaded();
    int IsFrameLimiterEnabled();
    void ToggleRadar(int bToggle);
    void DisplayHud(int bDisp);
    int GetScreenWidth();
    int GetScreenHeight();
    void SetWantedLevel(unsigned char byteLevel);
    void SetGameTextCount(unsigned short wCount);
    int IsUsingController();
    int GetLoadedVehicleModelCount();
    void GetWorldTime(unsigned char* pHour, unsigned char* pMinute);
};

int CGame::GetWorldWeather() {
    return *(int*)0xc81318;
}

unsigned char CGame::IsHudEnabled() {
    return *(unsigned char*)0xba6769;
}

int CGame::GetLocalMoney() {
    return *(int*)0xb7ce50;
}

int CGame::GetMemoryUsedForStreaming() {
    return *(int*)0x8e4cb4;
}

int CGame::GetMemoryAvailableForStreaming() {
    return *(int*)0x8a5a80;
}

unsigned long CGame::GetTimeInMilliseconds() {
    return *(unsigned long*)0xb7cb84;
}

int CGame::GetRwObjectsCount() {
    return *(int*)0xb71804;
}

void CGame::EnableFrameLimiter() {
    *(unsigned char*)0xba6794 = 1;
}

int CGame::IsGameLoaded() {
    return *(unsigned char*)0xba6831 == 0;
}

int CGame::IsFrameLimiterEnabled() {
    return *(unsigned char*)0xba6794 != 0;
}

void CGame::ToggleRadar(int bToggle) {
    *(unsigned char*)0xbaa3fb = (bToggle == 0);
}

void CGame::DisplayHud(int bDisp) {
    if (bDisp) {
        *(unsigned char*)0xba6769 = 1;
        *(unsigned char*)0xbaa3fb = 0;
    } else {
        *(unsigned char*)0xba6769 = 0;
        *(unsigned char*)0xbaa3fb = 1;
    }
}

int CGame::GetScreenWidth() {
    return *(int*)0xc17044;
}

int CGame::GetScreenHeight() {
    return *(int*)0xc17048;
}

void CGame::SetWantedLevel(unsigned char byteLevel) {
    *(unsigned char*)0x58db60 = byteLevel;
}

void CGame::SetGameTextCount(unsigned short wCount) {
    *(unsigned short*)0xa44b68 = wCount;
}

int CGame::IsUsingController() {
    return *(unsigned char*)0xba6818 != 0;
}

int CGame::GetLoadedVehicleModelCount() {
    int count = 0;
    for (unsigned long p = 0x8e6c10; p != 0x8e7c8c; p += 0x14) {
        if (*(int*)p) count++;
    }
    return count;
}

void CGame::GetWorldTime(unsigned char* pHour, unsigned char* pMinute) {
    *pMinute = *(unsigned char*)0xb70152;
    *pHour = *(unsigned char*)0xb70153;
}

struct _VECTOR {
    float X, Y, Z;
};

struct _MATRIX4X4 {
    _VECTOR right;
    unsigned long flagsRight;
    _VECTOR up;
    unsigned long flagsUp;
    _VECTOR at;
    unsigned long flagsAt;
    _VECTOR pos;
    unsigned long flagsPos;
};

struct ENTITY_TYPE {
    void* vtable;            // 0x00
    char gap0[0x10];         // 0x04 to 0x14
    _MATRIX4X4* mat;         // 0x14
    void* pRwObject;         // 0x18
    unsigned long nFlags;    // 0x1c
    char gap1[0x2];          // 0x20 to 0x22
    unsigned short nModelIndex; // 0x22
    char gap2[0x1c];         // 0x24 to 0x40
    unsigned long dwUnkFlags;// 0x40
    _VECTOR vecMoveSpeed;    // 0x44
    _VECTOR vecTurnSpeed;    // 0x50
    char gap3[0x58];         // 0x5c to 0xb4
    void* pSector;           // 0xb4
};

class CEntity {
public:
    char gap[0x40];
    ENTITY_TYPE* m_pEntity;  // 0x40

    void GetMatrix(_MATRIX4X4* pMatrix);
    void SetMatrix(_MATRIX4X4 matrix);
    void SetMoveSpeedVector(_VECTOR vec);
    int IsAdded();
    int IsCollisionCheckingEnabled();
    void SetCollisionChecking(int bEnable);
    void SetGravityProcessing(int bEnable);
    void DisableStreaming();
    void EnableTunnelTransition();
    void SetApplySpeed(int bState);
    void MakeNonCollidable();
    unsigned long* GetRwObject();
    unsigned char IsNativeTarget();
    unsigned int GetModelIndex();
    void SetTurnSpeedVector(_VECTOR vec);
    void GetMoveSpeedVector(_VECTOR* pVec);
    void GetTurnSpeedVector(_VECTOR* pVec);
    unsigned long GetWorldBoundRadius();    void GetBoundCentre(_VECTOR *pVector);    void ApplyTurnSpeed();


};

void CEntity::GetMatrix(_MATRIX4X4* pMatrix) {
    if (!m_pEntity || !m_pEntity->mat) return;

    pMatrix->right.X = m_pEntity->mat->right.X;
    pMatrix->right.Y = m_pEntity->mat->right.Y;
    pMatrix->right.Z = m_pEntity->mat->right.Z;

    pMatrix->up.X = m_pEntity->mat->up.X;
    pMatrix->up.Y = m_pEntity->mat->up.Y;
    pMatrix->up.Z = m_pEntity->mat->up.Z;

    pMatrix->at.X = m_pEntity->mat->at.X;
    pMatrix->at.Y = m_pEntity->mat->at.Y;
    pMatrix->at.Z = m_pEntity->mat->at.Z;

    pMatrix->pos.X = m_pEntity->mat->pos.X;
    pMatrix->pos.Y = m_pEntity->mat->pos.Y;
    pMatrix->pos.Z = m_pEntity->mat->pos.Z;
}

void CEntity::SetMatrix(_MATRIX4X4 Matrix) {
    if (!m_pEntity || !m_pEntity->mat) return;

    m_pEntity->mat->right.X = Matrix.right.X;
    m_pEntity->mat->right.Y = Matrix.right.Y;
    m_pEntity->mat->right.Z = Matrix.right.Z;

    m_pEntity->mat->up.X = Matrix.up.X;
    m_pEntity->mat->up.Y = Matrix.up.Y;
    m_pEntity->mat->up.Z = Matrix.up.Z;

    m_pEntity->mat->at.X = Matrix.at.X;
    m_pEntity->mat->at.Y = Matrix.at.Y;
    m_pEntity->mat->at.Z = Matrix.at.Z;

    m_pEntity->mat->pos.X = Matrix.pos.X;
    m_pEntity->mat->pos.Y = Matrix.pos.Y;
    m_pEntity->mat->pos.Z = Matrix.pos.Z;
}

void CEntity::SetMoveSpeedVector(_VECTOR vec) {
    m_pEntity->vecMoveSpeed.X = vec.X;
    m_pEntity->vecMoveSpeed.Y = vec.Y;
    m_pEntity->vecMoveSpeed.Z = vec.Z;
}

int CEntity::IsAdded() {
    if (m_pEntity && *(void**)m_pEntity != (void*)0x863c40 && m_pEntity->pSector)
        return 1;
    return 0;
}

int CEntity::IsCollisionCheckingEnabled() {
    if (!m_pEntity || *(void**)m_pEntity == (void*)0x863c40)
        return 1;
    return *(unsigned char*)((char*)m_pEntity + 0x1c) & 1;
}

void CEntity::SetCollisionChecking(int bEnable) {
    if (!m_pEntity || *(void**)m_pEntity == (void*)0x863c40) return;
    if (bEnable)
        m_pEntity->nFlags |= 1;
    else
        m_pEntity->nFlags &= ~1;
}

void CEntity::SetGravityProcessing(int bEnable) {
    if (!m_pEntity || *(void**)m_pEntity == (void*)0x863c40) return;
    if (bEnable)
        m_pEntity->nFlags &= 0x7ffffffd;
    else
        m_pEntity->nFlags |= 0x80000002;
}

void CEntity::DisableStreaming() {
    if (!m_pEntity || *(void**)m_pEntity == (void*)0x863c40) return;
    m_pEntity->nFlags |= 0x80400;
}

void CEntity::EnableTunnelTransition() {
    if (!m_pEntity || *(void**)m_pEntity == (void*)0x863c40) return;
    m_pEntity->nFlags |= 0x80000000;
}

void CEntity::SetApplySpeed(int bState) {
    if (!m_pEntity || *(void**)m_pEntity == (void*)0x863c40) return;
    if (!bState) {
        m_pEntity->dwUnkFlags |= 0x2000;
    } else {
        m_pEntity->dwUnkFlags &= ~0x2000;
    }
}

void CEntity::MakeNonCollidable() {
    if (m_pEntity)
        m_pEntity->dwUnkFlags &= 0xfffffff7;
}

unsigned long* CEntity::GetRwObject() {
    if (!m_pEntity) return (unsigned long*)m_pEntity;
    return (unsigned long*)m_pEntity->pRwObject;
}

unsigned char CEntity::IsNativeTarget() {
    if (m_pEntity && m_pEntity == *(ENTITY_TYPE**)0xb7cd68)
        return 1;
    return 0;
}

unsigned int CEntity::GetModelIndex() {
    return m_pEntity->nModelIndex;
}

void CEntity::SetTurnSpeedVector(_VECTOR vec) {
    m_pEntity->vecTurnSpeed.X = vec.X;
    m_pEntity->vecTurnSpeed.Y = vec.Y;
    m_pEntity->vecTurnSpeed.Z = vec.Z;
}

void CEntity::GetMoveSpeedVector(_VECTOR* pVec) {
    pVec->X = m_pEntity->vecMoveSpeed.X;
    pVec->Y = m_pEntity->vecMoveSpeed.Y;
    pVec->Z = m_pEntity->vecMoveSpeed.Z;
}

void CEntity::GetTurnSpeedVector(_VECTOR* pVec) {
    pVec->X = m_pEntity->vecTurnSpeed.X;
    pVec->Y = m_pEntity->vecTurnSpeed.Y;
    pVec->Z = m_pEntity->vecTurnSpeed.Z;
}

unsigned long CEntity::GetWorldBoundRadius() {
    if (m_pEntity && m_pEntity->pRwObject && *(void**)m_pEntity != (void*)0x863c40) {
        return *(unsigned long*)((char*)m_pEntity->pRwObject + 0x38);
    }
    return 0;
}

#pragma pack(pop)

#pragma pack(push, 1)
struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
};
typedef struct tagRECT RECT;

class CDXUTScrollBar {
public:
    char gap0[0x4d];
    bool m_bShowThumb;      // 0x4d
    char gap1[0x20];        // 0x4e to 0x6e
    RECT m_rcTrack;         // 0x6e
    RECT m_rcThumb;         // 0x7e
    int m_nPosition;        // 0x8e
    int m_nPageSize;        // 0x92
    int m_nStart;           // 0x96
    int m_nEnd;             // 0x9a

protected:
    void Cap();
    void UpdateThumbRect();
    friend void CDXUTScrollBar_Anchor();
};

void CDXUTScrollBar::Cap() {
    if (m_nPosition < m_nStart || m_nEnd - m_nStart <= m_nPageSize) {
        m_nPosition = m_nStart;
    } else if (m_nPosition + m_nPageSize > m_nEnd) {
        m_nPosition = m_nEnd - m_nPageSize;
    }
}

#define SCROLLBAR_MINTHUMBSIZE 8
#define __max(a,b) (((a) > (b)) ? (a) : (b))
static inline int RectHeight(RECT& rc) { return rc.bottom - rc.top; }

void CDXUTScrollBar::UpdateThumbRect() {
    if (m_nEnd - m_nStart > m_nPageSize) {
        int nThumbHeight = __max(RectHeight(m_rcTrack) * m_nPageSize / (m_nEnd - m_nStart), SCROLLBAR_MINTHUMBSIZE);
        int nMaxPosition = m_nEnd - m_nStart - m_nPageSize;
        m_rcThumb.top = m_rcTrack.top + (m_nPosition - m_nStart) * (RectHeight(m_rcTrack) - nThumbHeight) / nMaxPosition;
        m_rcThumb.bottom = m_rcThumb.top + nThumbHeight;
        m_bShowThumb = true;
    } else {
        m_rcThumb.bottom = m_rcThumb.top;
        m_bShowThumb = false;
    }
}

void CDXUTScrollBar_Anchor() {
    void (CDXUTScrollBar::*p1)() = &CDXUTScrollBar::Cap;
    void (CDXUTScrollBar::*p2)() = &CDXUTScrollBar::UpdateThumbRect;
}

class R5ChatGameMenuView {
public:
    int IsMenuActive();
};

int R5ChatGameMenuView::IsMenuActive() {
    return *(void**)0xba67a4 != 0;
}

#define MAX_CMD_INPUT 128
#define MAX_CMDS 144
#define MAX_CMD_STRLEN 32

typedef void (__cdecl *CMDPROC)(char*);

struct CDXUTDialog {
    char gap[0x11e];
    int m_width;    // 0x11e
    int m_height;   // 0x122
};

struct CDXUTEditBox {
    char gap[0x8];
    void SetText(const char* szText, bool bSelected = false);
    char* GetTextA();
};

extern "C" {
void* __cdecl memset(void*, int, unsigned int);
char* __cdecl strcpy(char*, const char*);
char* __cdecl strncpy(char*, const char*, unsigned int);
unsigned int __cdecl strlen(const char*);
int __cdecl _stricmp(const char*, const char*);
}
#pragma intrinsic(memset, strcpy, strncpy, strlen)

struct IDirect3DDevice9;

class CCmdWindow {
public:
    IDirect3DDevice9* m_pD3DDevice;         // 0x0
    CDXUTDialog* m_pGameUI;                 // 0x4
    CDXUTEditBox* m_pEditControl;           // 0x8
    CMDPROC m_pCmds[MAX_CMDS];              // 0xc
    char m_szCmdNames[MAX_CMDS][MAX_CMD_STRLEN + 1]; // 0x24c
    int m_iCmdCount;                        // 0x14dc
    int m_bEnabled;                         // 0x14e0
    char m_szInputBuffer[MAX_CMD_INPUT + 1]; // 0x14e4
    char m_szRecallBuffer[10][MAX_CMD_INPUT + 1]; // 0x1565
    char m_szCurBuffer[MAX_CMD_INPUT + 1];  // 0x1a6f
    int m_iCurrentRecallAt;                 // 0x1af0
    int m_iTotalRecalls;                    // 0x1af4
    CMDPROC m_pDefaultCmd;                  // 0x1af8

    CCmdWindow(IDirect3DDevice9* pD3DDevice);
    void GetDialogSize(RECT* pRect);
    void AddToRecallBuffer(char* szCmdInput);
    void RecallUp();
    void RecallDown();
    void AddDefaultCmdProc(CMDPROC cmdDefault);
    void AddCmdProc(char* szCmdName, CMDPROC cmdHandler);
};

CCmdWindow::CCmdWindow(IDirect3DDevice9* pD3DDevice) {
    m_bEnabled = 0;
    m_pD3DDevice = pD3DDevice;
    m_iCmdCount = 0;
    m_iTotalRecalls = 0;
    m_pEditControl = 0;
    m_iCurrentRecallAt = -1;

    memset(&m_szRecallBuffer[0], 0, 1290);
    memset(&m_szInputBuffer[0], 0, 129);
    memset(&m_szCurBuffer[0], 0, 129);
}

void CCmdWindow::GetDialogSize(RECT* pRect) {
    memset(pRect, 0, sizeof(RECT));
    if (m_pGameUI) {
        pRect->right = m_pGameUI->m_width;
        pRect->bottom = m_pGameUI->m_height;
    }
}

void CCmdWindow::AddToRecallBuffer(char* szCmdInput) {
    int x = 10 - 1;
    while (x) {
        strcpy(m_szRecallBuffer[x], m_szRecallBuffer[x - 1]);
        x--;
    }
    strcpy(m_szRecallBuffer[0], szCmdInput);
    if (m_iTotalRecalls < 10) {
        m_iTotalRecalls++;
    }
}

void CCmdWindow::AddDefaultCmdProc(CMDPROC cmdDefault) {
    m_pDefaultCmd = cmdDefault;
}

void CCmdWindow::AddCmdProc(char* szCmdName, CMDPROC cmdHandler) {
    if (m_iCmdCount < MAX_CMDS && (strlen(szCmdName) < MAX_CMD_STRLEN)) {
        m_pCmds[m_iCmdCount] = cmdHandler;
        strcpy(m_szCmdNames[m_iCmdCount], szCmdName);
        m_iCmdCount++;
    }
}

void CCmdWindow::RecallDown() {
    m_iCurrentRecallAt--;
    if (m_iCurrentRecallAt >= 0) {
        m_pEditControl->SetText(m_szRecallBuffer[m_iCurrentRecallAt]);
    } else {
        if (m_iCurrentRecallAt == -1) {
            m_pEditControl->SetText(m_szCurBuffer);
        }
        m_iCurrentRecallAt = -1;
    }
}

void CCmdWindow::RecallUp() {
    if (m_iCurrentRecallAt >= (m_iTotalRecalls - 1)) return;

    if (m_iCurrentRecallAt == -1) {
        strncpy(m_szCurBuffer, m_pEditControl->GetTextA(), 128);
        m_szCurBuffer[128] = '\0';
    }

    m_iCurrentRecallAt++;
    m_pEditControl->SetText(m_szRecallBuffer[m_iCurrentRecallAt]);
}

#pragma pack(pop)



// Matched void CEntity::GetBoundCentre (0x1009eef0)
void CEntity::GetBoundCentre(_VECTOR *pVector)
{
	void *pEntity = m_pEntity;
	if (pEntity)
	{
		__asm
		{
			push pVector
			mov ecx, pEntity
			mov edx, 0x534250
			call edx
		}
	}
}


// Matched ?GamePool_Ped_GetAt@@YGPAU_PED_TYPE@@H@Z (0x100b3b70)
struct _PED_TYPE;

_PED_TYPE* __stdcall GamePool_Ped_GetAt(int nPedIndex)
{
	_PED_TYPE* pPed;
	__asm
	{
		mov ebx, 0xB74490
		mov ecx, [ebx]
		push nPedIndex
		mov ebx, 0x404910
		call ebx
		mov pPed, eax
	}
	return pPed;
}


// Matched void CEntity::ApplyTurnSpeed (0x1009ee90)
void CEntity::ApplyTurnSpeed()
{
	unsigned long dwEnt = (unsigned long)m_pEntity;
	if (!dwEnt) return;

	_asm mov ecx, dwEnt
	_asm mov eax, 0x542E20
	_asm call eax
}


// Matched ?GetVehicleSubtypeFromVehiclePtr@@YAIPAU_VEHICLE_TYPE@@@Z (0x100b61e0)
struct _VEHICLE_TYPE;

unsigned int GetVehicleSubtypeFromVehiclePtr(_VEHICLE_TYPE *pVehicle)
{
    if (!pVehicle)
        return 0;

    unsigned int vtable = *(unsigned int *)pVehicle;

    if (vtable == 0x871120)
        return 1;
    if (vtable == 0x8721A0)
        return 4;
    if (vtable == 0x871360)
        return 2;
    if (vtable == 0x871948)
        return 5;
    if (vtable == 0x871680)
        return 3;
    if (vtable == 0x871528)
        return 6;
    if (vtable == 0x872370)
        return 7;

    return 0;
}


// Matched ?Font_SetJustify@@YAXH@Z (0x1009feb0)
__declspec(naked) void Font_SetJustify(int on)
{
    __asm {
        mov eax, 0x719610
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x719490
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x7195b0
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x7194f0
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x7195c0
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x7195e0
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x7194d0
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x7194e0
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x719570
        jmp eax
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        int 3
        mov eax, 0x69de90
        ret
    }
}
