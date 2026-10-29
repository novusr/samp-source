// GTA absolute addresses are part of R5's original behavior.
// These functions must not be invoked in an ordinary host process.
extern "C" void __cdecl Unprotect(void* address, unsigned long bytes);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void*);

class SimpleMutex {
public:
    void* criticalSection[6];
    void Unlock(void);
    void Lock(void);
};

class LightweightDatabaseServer {
public:
    static int __cdecl DatabaseTableComp(char *const &key1, char *const &key2);
};

#pragma pack(push, 1)
struct StrAndBool {
    char *str;
    bool b;
};
#pragma pack(pop)
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

class Replica;
class ReplicaManager {
public:
    struct RegisteredReplica {
        Replica *replica;
    };
    struct CommandStruct {
        Replica *replica;
    };
    struct RemoteObject {
        Replica *replica;
    };
    static int __cdecl RegisteredReplicaComp(Replica *const &key, const RegisteredReplica &data);
    static int __cdecl CommandStructComp(Replica *const &key, const CommandStruct &data);
    static int __cdecl RemoteObjectComp(Replica *const &key, const RemoteObject &data);
};

class CCryptoContext {
public:
    unsigned long m_dwProvider;
    unsigned long GetProvider();
};

unsigned long CCryptoContext::GetProvider() {
    return m_dwProvider;
}

extern "C" {
void* __cdecl memset(void*, int, unsigned int);
void* __cdecl memcpy(void*, const void*, unsigned int);
double __cdecl sqrt(double);
int __cdecl vsprintf(char*, const char*, ...);
char* __cdecl strcpy(char*, const char*);
int __cdecl _stat(const char*, void*);
}
#pragma intrinsic(memset, memcpy, sqrt, strcpy)

struct _stat32 {
    char pad[36];
};

void __cdecl ResetLocalPad(int unk1, int unk2);
class CPad {
public:
    void Reset(int unk1, int unk2);
};
int __cdecl IsFileOrDirectoryExists(char *szPath);
unsigned long __cdecl GetColorFromEmbedCode(char *szString);
void __cdecl RemoveColorEmbedsFromString(char *szString);

float DegToRad(float fDegrees);

class RakPeerInterface;
class PacketLogger {
public:
    virtual void OnAttach(RakPeerInterface* peer);
    RakPeerInterface* rakPeer;
};

typedef long time_t;
struct tm;
struct FILE;
extern "C" {
time_t __cdecl time(time_t*);
struct tm* __cdecl localtime(const time_t*);
unsigned int __cdecl strftime(char*, unsigned int, const char*, const struct tm*);
FILE* __cdecl fopen(const char*, const char*);
int __cdecl fprintf(FILE*, const char*, ...);
int __cdecl fclose(FILE*);
}

#pragma pack(push, 1)
class CChatWindow {
public:
    int field_0;
    char _gap4[4];
    int field_8;
    char field_C;
    int field_D;
    char field_11[261];
    char pad[0x14];
    unsigned long m_dwChatDebugColor;
    void AddEntry(int type, char* text, char* prefix, unsigned long textColor, unsigned long prefixColor);
    void AddDebugMessage(char* szFormat, ...);
    void Log(int type, char* text, char* prefix);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MENU_INT {
    int bMenu;
    int bRow[12];
    int bPadding[3];
};

class CMenu {
public:
    unsigned char m_byteMenuID;
    char m_charTitle[33];
    char m_charItems[12][2][33];
    char m_charHeader[2][33];
    float m_fXPos;
    float m_fYPos;
    float m_fCol1Width;
    float m_fCol2Width;
    unsigned char m_byteColumns;
    MENU_INT m_MenuInteraction;
    unsigned char m_byteColCount[2];
    unsigned long m_dwPanel;

    CMenu(float fX, float fY, unsigned char byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction);
    char* GetMenuTitle();
    char* GetMenuItem(unsigned char byteColumn, unsigned char byteRow);
    char* GetMenuHeader(unsigned char byteColumn);
};
#pragma pack(pop)

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
    float GetHealth();
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

float CVehicle::GetHealth() {
    if (m_pVehicle) return m_pVehicle->fHealth;
    return 0.0f;
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

// True for the train rolling-stock models: freight/brown streak (0x21A/0x219),
// passenger (0x23A/0x239) and the tram (0x1C1).
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

struct SCRIPT_COMMAND {
    unsigned short OpCode;
    char Params[16];
};
int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);
extern "C" const SCRIPT_COMMAND load_requested_models;
extern "C" const SCRIPT_COMMAND request_model;
extern "C" const SCRIPT_COMMAND request_animation;
extern "C" const SCRIPT_COMMAND is_animation_loaded;
extern "C" const SCRIPT_COMMAND is_model_available;
extern "C" const SCRIPT_COMMAND put_train_at;
extern "C" const SCRIPT_COMMAND get_ground_z;
extern "C" const SCRIPT_COMMAND destroy_racing_checkpoint;
extern "C" const SCRIPT_COMMAND refresh_streaming_at;
extern "C" const SCRIPT_COMMAND set_current_time;
#define TRAIN_PASSENGER_LOCO 538
#define TRAIN_FREIGHT_LOCO 537
#define TRAIN_TRAM 449
void __stdcall WorldAddEntity(unsigned long *pEntity);
void __stdcall WorldRemoveEntity(unsigned long *pEntity);

struct GAME_SCRIPT_THREAD {
    char pad[0xe0];
};
extern unsigned long dwActorPed;
class CPedIntelligence {
public:
    void Flush(int a);
};
extern GAME_SCRIPT_THREAD* gst;
extern int bScmLocalDebug;
extern unsigned char ScriptBuf[255];
extern void* ProcessOneCommand;
void* __cdecl operator new(unsigned int);

#pragma pack(push, 1)
class CGame {
public:
    char pad0[0x29];
    int m_bRaceCheckpointsEnabled; // 0x29
    int m_dwRaceCheckpointMarker;  // 0x2D
    unsigned long m_dwRaceCheckpointHandle; // 0x31
    char pad1[0x5D - 0x35];
    unsigned long field_5D;
    int field_61;
    int field_65;
    int field_69;
    void SetWorldTime(int iHour, int iMinute);
    void ToggleThePassingOfTime(unsigned char byteOnOff);
    void SetFrameLimit(unsigned long dwLimit);
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
    void LoadRequestedModels();
    void LoadScene(char* szScene);
    void LoadCullZone(char* szLine);
    void RequestModel(int iModelID, int iLoadingStream = 2);
    int IsAnimationLoaded(char* szAnimFile);
    void RequestAnimation(char* szAnimFile);
    int IsModelLoaded(int iModelID);
    void sub_100A0090(int a1, int a2);
    float FindGroundZForCoord(float x, float y, float z);
    void SetMaxStats();
    void RefreshStreamingAt(float x, float y);
    void DisableRaceCheckpoint();
};
#pragma pack(pop)

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

class CAudio {
public:
    void StartRadio(int nStation);
    void StopRadio();
    float GetRadioVolume();
    void StopOutdoorAmbienceTrack();
    void SetOutdoorAmbienceTrack(int nTrack);
};

void CAudio::StartRadio(int nStation) {
	__asm {
		push 0
		push nStation
		mov ecx, 0xB6BC90
		mov edx, 0x507DC0
		call edx
	}
}

void CAudio::StopRadio() {
	__asm {
		push 0
		push 0
		mov ecx, 0xB6BC90
		mov edx, 0x506F70
		call edx
	}
}

float CAudio::GetRadioVolume() {
	return *(float*)0xB5FCC8;
}

void CAudio::StopOutdoorAmbienceTrack() {
	__asm {
		mov ecx, 0x8AC15C
		mov edx, 0x4D6D60
		call edx
	}
}

void CAudio::SetOutdoorAmbienceTrack(int nTrack) {
	__asm {
		push nTrack
		mov ecx, 0x8AC15C
		mov edx, 0x4D6D50
		call edx
	}
}

struct _VECTOR {
    float X, Y, Z;
};
bool __cdecl FUNC_100B4B50(_VECTOR *vecPos);

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

class CAudioClass {
public:
    void Play(int event, int dummy);
};

struct IEntityGTA {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void DeleteRwObject();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void Teleport(float x, float y, float z, int unk);
};

class CUniBuffer {
    static long __stdcall Dummy_ScriptStringCPtoX(void *, int, int, int *);
    static long __stdcall Dummy_ScriptStringXtoCP(void *, int, int *, int *);
};

class CEntity {
public:
    virtual ~CEntity();
    virtual void Add();
    virtual void Remove();
    char gap[0x3c];
    ENTITY_TYPE* m_pEntity;  // 0x40
    unsigned long m_dwGTAId; // 0x44

    void TeleportTo(float x, float y, float z);
    void PlayAudio(int event);
    void DeleteRwObject();

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
    void GetBoundRect(struct _FRECT *pRect);
    float GetDistanceFromCentreOfMassToBaseOfModel();
    void SetClumpAlpha(int iAlpha);
    float GetDistanceFromPoint(float X, float Y, float Z);
    float GetDistanceFromCamera();
    int HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY);
    void ProcessControl();
    void SetMatrixAndUpdate(_MATRIX4X4 matrix);
    void AdvancePosition();
    int IsStationary();


};

class CActorPed : public CEntity {
public:
    virtual ~CActorPed();
};

// Copies the entity transformation matrix into the output parameter
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

// Copy orientation and position matrix to entity's transformation matrix.
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

// Clamps the scroll position so at least one page stays visible and it never
// runs past either end of the scroll range.
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

// Updates the scrollbar thumb rectangle based on position, range, and page size.
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

// Builds a console command window: clears the command list, recall history and
// input buffers, leaving the recall cursor at "no entry".
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

// Writes the game UI's dialog size into pRect (all zero when there is no UI).
void CCmdWindow::GetDialogSize(RECT* pRect) {
    memset(pRect, 0, sizeof(RECT));
    if (m_pGameUI) {
        pRect->right = m_pGameUI->m_width;
        pRect->bottom = m_pGameUI->m_height;
    }
}

// Push the last command into the recall history, dropping the oldest entry.
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

// Register a named command handler, provided the command table has room
// and the name fits in its slot.
void CCmdWindow::AddCmdProc(char* szCmdName, CMDPROC cmdHandler) {
    if (m_iCmdCount < MAX_CMDS && (strlen(szCmdName) < MAX_CMD_STRLEN)) {
        m_pCmds[m_iCmdCount] = cmdHandler;
        strcpy(m_szCmdNames[m_iCmdCount], szCmdName);
        m_iCmdCount++;
    }
}

// Step forward through the recall history; past the newest entry the edit
// control is restored to the text that was being typed.
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

// Step back to the previous command in the recall history, saving the
// text currently typed in the edit control before leaving it.
void CCmdWindow::RecallUp() {
    if (m_iCurrentRecallAt >= (m_iTotalRecalls - 1)) return;

    if (m_iCurrentRecallAt == -1) {
        strncpy(m_szCurBuffer, m_pEditControl->GetTextA(), MAX_CMD_INPUT);
        m_szCurBuffer[MAX_CMD_INPUT] = '\0';
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

// Matched void CEntity::GetBoundRect (0x1009ef20)
void CEntity::GetBoundRect(struct _FRECT *pRect)
{
	void *pEntity = m_pEntity;
	if (pEntity)
	{
		__asm
		{
			push pRect
			mov ecx, pEntity
			mov edx, 0x534120
			call edx
		}
	}
}

// Matched float CEntity::GetDistanceFromCentreOfMassToBaseOfModel (0x1009eeb0)
float CEntity::GetDistanceFromCentreOfMassToBaseOfModel()
{
	void *pEntity = m_pEntity;
	float fDist = 0.0f;
	if (!pEntity) return 0.0f;
	__asm
	{
		mov ecx, pEntity
		mov edx, 0x536BE0
		call edx
		mov fDist, eax
	}
	return fDist;
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

// Matched ?GamePool_FindPlayerPed@@YGPAU_PED_TYPE@@XZ (0x100b3c60)
_PED_TYPE* __stdcall GamePool_FindPlayerPed()
{
	return *(_PED_TYPE**)0xB7CD98;
}

// Matched ?GamePool_GetBuilding@@YAPAU_BUILDING_TYPE@@XZ (0x100b3c70)
struct _BUILDING_TYPE;
_BUILDING_TYPE* GamePool_GetBuilding()
{
	_BUILDING_TYPE* pBuilding;
	__asm
	{
		mov eax, 0xB74498
		mov edx, [eax]
		mov eax, [edx]
		mov pBuilding, eax
	}
	return pBuilding;
}

// Matched ?GamePool_GetDummy@@YAPAU_DUMMY_TYPE@@XZ (0x100b3c90)
struct _DUMMY_TYPE;
_DUMMY_TYPE* GamePool_GetDummy()
{
	_DUMMY_TYPE* pDummy;
	__asm
	{
		mov eax, 0xB744A0
		mov edx, [eax]
		mov eax, [edx]
		mov pDummy, eax
	}
	return pDummy;
}

// Matched ?GamePool_GetObject@@YAPAU_OBJECT_TYPE@@XZ (0x100b3cb0)
struct _OBJECT_TYPE;
_OBJECT_TYPE* GamePool_GetObject()
{
	_OBJECT_TYPE* pObject;
	__asm
	{
		mov eax, 0xB7449C
		mov edx, [eax]
		mov eax, [edx]
		mov pObject, eax
	}
	return pObject;
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


// Vehicle subtypes returned by GetVehicleSubtypeFromVehiclePtr.
enum
{
    VEHICLE_SUBTYPE_CAR = 1,
    VEHICLE_SUBTYPE_BIKE = 2,
    VEHICLE_SUBTYPE_HELI = 3,
    VEHICLE_SUBTYPE_BOAT = 4,
    VEHICLE_SUBTYPE_PLANE = 5,
    VEHICLE_SUBTYPE_PUSHBIKE = 6,
    VEHICLE_SUBTYPE_TRAIN = 7
};

// Matched ?GetVehicleSubtypeFromVehiclePtr@@YAIPAU_VEHICLE_TYPE@@@Z (0x100b61e0)
struct _VEHICLE_TYPE;

// Maps a vehicle's vtable (its runtime class) to its VEHICLE_SUBTYPE_*.
unsigned int GetVehicleSubtypeFromVehiclePtr(_VEHICLE_TYPE *pVehicle)
{
    if (!pVehicle)
        return 0;

    unsigned int vtable = *(unsigned int *)pVehicle;

    if (vtable == 0x871120)
        return VEHICLE_SUBTYPE_CAR;
    if (vtable == 0x8721A0)
        return VEHICLE_SUBTYPE_BOAT;
    if (vtable == 0x871360)
        return VEHICLE_SUBTYPE_BIKE;
    if (vtable == 0x871948)
        return VEHICLE_SUBTYPE_PLANE;
    if (vtable == 0x871680)
        return VEHICLE_SUBTYPE_HELI;
    if (vtable == 0x871528)
        return VEHICLE_SUBTYPE_PUSHBIKE;
    if (vtable == 0x872370)
        return VEHICLE_SUBTYPE_TRAIN;

    return 0;
}


// Constructor for CMenu: initializes items, headers, title buffers and column configuration.
CMenu::CMenu(float fX, float fY, unsigned char byteColumns, float fCol1Width, float fCol2Width, MENU_INT *MenuInteraction)
{
	for (int i = 0; i < 12; i++)
	{
		memset(m_charItems[i][0], 0, sizeof(m_charItems[i][0]));
		memset(m_charItems[i][1], 0, sizeof(m_charItems[i][1]));
	}
	memset(m_charHeader[0], 0, sizeof(m_charHeader[0]));
	memset(m_charHeader[1], 0, sizeof(m_charHeader[1]));
	memset(m_charTitle, 0, sizeof(m_charTitle));

	m_fXPos = fX;
	m_fYPos = fY;
	m_fCol1Width = fCol1Width;
	m_fCol2Width = fCol2Width;

	if (byteColumns == 2) m_byteColumns = 2;
	else m_byteColumns = 1;
	memcpy(&m_MenuInteraction, MenuInteraction, sizeof(MENU_INT));

	m_dwPanel = 0;
}

#pragma pack(push, 1)
class CDXUTTimer {
public:
    bool m_bUsingQPF;
    bool m_bTimerStopped;

    bool IsStopped();
};

struct D3DXMATRIX {
    float m[4][4];
};

struct D3DXQUATERNION {
    float x, y, z, w;
};

struct D3DXVECTOR2 {
    float x, y;
};

struct POINT {
    long x, y;
};

inline D3DXQUATERNION* D3DXQuaternionIdentity(D3DXQUATERNION *pOut) {
    pOut->x = pOut->y = pOut->z = 0.0f;
    pOut->w = 1.0f;
    return pOut;
}

inline D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX *pOut) {
    pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
    pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
    pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
    pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;
    pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
    return pOut;
}

class CD3DArcBall {
public:
    D3DXMATRIX m_mRotation;
    D3DXMATRIX m_mTranslation;
    D3DXMATRIX m_mTranslationDelta;
    POINT m_Offset;
    int m_nWidth;
    int m_nHeight;
    D3DXVECTOR2 m_vCenter;
    float m_fRadius;
    float m_fRadiusTranslation;
    D3DXQUATERNION m_qDown;
    D3DXQUATERNION m_qNow;
    bool m_bDrag;

    void Reset();
    void OnEnd();
};

class TEABlockEncryptor {
public:
    char gap0[0x249];
    bool m_bKeySet;                // 0x249
    unsigned char m_key[4];        // 0x24a
    char gap1[0xc];                // 0x24e to 0x25a
    unsigned int m_sumInit;        // 0x25a
    unsigned int m_delta;          // 0x25e

    bool IsKeySet() const;
    void SetKey(const unsigned char* const pKey);
    void UnsetKey();
protected:
    void EncryptBlock(unsigned int &v0, unsigned int &v1);
    void DecryptBlock(unsigned int &v0, unsigned int &v1);
};
#pragma pack(pop)

// Matched ?IsStopped@CDXUTTimer@@QAE_NXZ (0x10093800)
bool CDXUTTimer::IsStopped() {
    return m_bTimerStopped;
}

// Reset CD3DArcBall orientation quaternions, matrices, drag state, and radii to defaults.
void CD3DArcBall::Reset() {
    D3DXQuaternionIdentity(&m_qDown);
    D3DXQuaternionIdentity(&m_qNow);
    D3DXMatrixIdentity(&m_mRotation);
    D3DXMatrixIdentity(&m_mTranslation);
    D3DXMatrixIdentity(&m_mTranslationDelta);
    m_bDrag = false;
    m_fRadiusTranslation = 1.0f;
    m_fRadius = 1.0f;
}

void PacketLogger::OnAttach(RakPeerInterface *peer) {
    rakPeer = peer;
}

int ReplicaManager::RegisteredReplicaComp(Replica *const &key, const ReplicaManager::RegisteredReplica &data) {
    if (key < data.replica)
        return -1;
    if (key == data.replica)
        return 0;
    return 1;
}

int ReplicaManager::CommandStructComp(Replica *const &key, const ReplicaManager::CommandStruct &data) {
    if (key < data.replica)
        return -1;
    if (key == data.replica)
        return 0;
    return 1;
}

int ReplicaManager::RemoteObjectComp(Replica *const &key, const ReplicaManager::RemoteObject &data)
{
    if (key < data.replica)
        return -1;
    if (key == data.replica)
        return 0;
    return 1;
}

void SimpleMutex::Unlock(void)
{
    LeaveCriticalSection(this);
}

void SimpleMutex::Lock(void) {
    EnterCriticalSection(this);
}

long __stdcall CUniBuffer::Dummy_ScriptStringCPtoX(void *p1, int p2, int p3, int *p4)
{
    return 0x80004001;
}

long __stdcall CUniBuffer::Dummy_ScriptStringXtoCP(void *p1, int p2, int *p3, int *p4)
{
    return 0x80004001;
}

extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int, unsigned long, const char *, int, unsigned short *, int);
extern "C" void * __cdecl memset(void *, int, unsigned int);
extern "C" unsigned int __cdecl strlen(const char *);
#pragma intrinsic(memset, strlen)
void __cdecl ConvertAnsiToWide(char const *source, unsigned short *destination, int capacity)
{
    memset(destination, 0, capacity * sizeof(unsigned short));
    int required = MultiByteToWideChar(0, 0, source, strlen(source), 0, 0);
    if (required < capacity)
        MultiByteToWideChar(0, 0, source, strlen(source), destination, required);
}

void __cdecl R5FontConvertAnsiToWide(char *source, unsigned short *destination, int capacity);

extern "C" int __cdecl strcmp(const char *, const char *);
#pragma intrinsic(strcmp)
int __cdecl StrAndBoolComp(char *const &key, const StrAndBool &data)
{
	return strcmp(key, (const char*)data.str);
}

int __cdecl LightweightDatabaseServer::DatabaseTableComp(char *const &key1, char *const &key2)
{
	return strcmp(key1, key2);
}

void CGame::LoadRequestedModels() {
    ScriptCommand(&load_requested_models);
}

#pragma optimize("s", on)
#pragma optimize("y", off)
void CGame::LoadCullZone(char *szLine)
{
    typedef void (__cdecl *Func)(char*);
    ((Func)0x5B4B40)(szLine);
}
#pragma optimize("", on)

#pragma optimize("sy", off)
#pragma optimize("s", on)
void __stdcall WorldAddEntity(unsigned long *pEntity) {
    ((void (__cdecl *)(unsigned long*))0x563220)(pEntity);
}
#pragma optimize("", on)
extern "C" void __cdecl func_563280(unsigned long*);
#pragma optimize("s", on)
void __stdcall WorldRemoveEntity(unsigned long *pEntity) {
    ((void (__cdecl *)(unsigned long*))0x563280)(pEntity);
}
#pragma optimize("s", on)
#pragma optimize("y", off)
void CGame::LoadScene(char* szScene)
{
    typedef void (__cdecl *Func)(char*);
    ((Func)0x5b8700)(szScene);
}
#pragma optimize("", on)
void CGame::RequestModel(int iModelID, int iLoadingStream)
{
	ScriptCommand(&request_model, iModelID);
}

int CGame::IsAnimationLoaded(char *szAnimFile)
{
	return ScriptCommand(&is_animation_loaded, szAnimFile);
}

void CGame::RequestAnimation(char *szAnimFile)
{
	ScriptCommand(&request_animation, szAnimFile);
}

struct CPedIntelligence;
int FlushPedIntelligence(void)
{
    CPedIntelligence* p = *(CPedIntelligence**)(dwActorPed + 0x47c);
    union {
        void (CPedIntelligence::*fn)(int);
        void* addr;
    } u;
    u.addr = (void*)0x601640;
    (p->*u.fn)(1);
    return 1;
}

void InitScripting(void)
{
	gst = new GAME_SCRIPT_THREAD;
	memset(gst, 0, sizeof(GAME_SCRIPT_THREAD));
}

int CGame::IsModelLoaded(int iModelID)
{
	if (iModelID > 20000 || iModelID < 0) return 1;
	return ScriptCommand(&is_model_available, iModelID);
}

int ExecuteScriptBuf(void)
{
	bScmLocalDebug = 1;
	*(unsigned long*)((char*)gst + 0x14) = (unsigned long)(void*)ScriptBuf;
	typedef void (__fastcall *Proc)(GAME_SCRIPT_THREAD*);
	((Proc)ProcessOneCommand)(gst);
	int ret = *(int*)((char*)gst + 0xc5);
	bScmLocalDebug = 0;
	return ret;
}

#pragma optimize("y", off)
void CEntity::SetClumpAlpha(int iAlpha)
{
	if (!m_pEntity || !m_pEntity->pRwObject || *(void**)m_pEntity == (void*)0x863C40)
		return;

	volatile unsigned long dwEntity = (unsigned long)m_pEntity;
	typedef void (__cdecl *Func)(void*, int);
	((Func)0x732B00)(((ENTITY_TYPE*)dwEntity)->pRwObject, (int)iAlpha);
}
#pragma optimize("", on)
// Straight-line distance from this entity's world position to the point
// (X, Y, Z).
float CEntity::GetDistanceFromPoint(float X, float Y, float Z)
{
	_MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	GetMatrix(&matThis);
	fSX = (matThis.pos.X - X) * (matThis.pos.X - X);
	fSY = (matThis.pos.Y - Y) * (matThis.pos.Y - Y);
	fSZ = (matThis.pos.Z - Z) * (matThis.pos.Z - Z);

	return (float)sqrt(fSX + fSY + fSZ);
}

// Straight-line distance from this entity to the player camera position
// (0xB6F9CC/0xB6F9D0/0xB6F9D4); 100000 while the entity is unstreamed or
// a dummy (0x863C40 vtable).
float CEntity::GetDistanceFromCamera()
{
	if (!m_pEntity || *(void**)m_pEntity == (void*)0x863C40) return 100000.0f;

	_MATRIX4X4	matThis;
	float		fSX,fSY,fSZ;

	GetMatrix(&matThis);
	fSX = (matThis.pos.X - *(float*)0xB6F9CC) * (matThis.pos.X - *(float*)0xB6F9CC);
	fSY = (matThis.pos.Y - *(float*)0xB6F9D0) * (matThis.pos.Y - *(float*)0xB6F9D0);
	fSZ = (matThis.pos.Z - *(float*)0xB6F9D4) * (matThis.pos.Z - *(float*)0xB6F9D4);

	return (float)sqrt(fSX + fSY + fSZ);
}

CEntity::~CEntity() {
}

void CChatWindow::Log(int type, char *text, char *prefix) {
    time_t now = time(0);
    if (!field_D) return;
    FILE *file = fopen(field_11, "a");
    if (!file) return;
    char stamp[65];
    strftime(stamp, 64, "[%H:%M:%S]", localtime(&now));
    switch (type) {
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

void CChatWindow::AddDebugMessage(char *szFormat, ...) {
    __declspec(align(8)) char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    char* args = (char*)&szFormat + sizeof(szFormat);
    vsprintf(buffer, szFormat, args);
    for (char *p = buffer; *p; ++p) {
        if (*p > 0 && *p < 32) *p = ' ';
    }
    AddEntry(8, buffer, 0, m_dwChatDebugColor, 0);
}

int CEntity::HasExceededWorldBoundries(float fPX, float fZX, float fPY, float fNY) {
    _MATRIX4X4 matWorld;
    if (!m_pEntity) return 0;
    GetMatrix(&matWorld);
    if (matWorld.pos.X > fPX) {
        return 1;
    } else if (matWorld.pos.X < fZX) {
        return 1;
    } else if (matWorld.pos.Y > fPY) {
        return 1;
    } else if (matWorld.pos.Y < fNY) {
        return 1;
    }
    return 0;
}

float DegToRad(float fDegrees) {
    if (fDegrees > 360.0f || fDegrees < 0.0f) return 0.0f;
    if (fDegrees > 180.0f) {
        return (float)(-(3.14159265f - (((fDegrees - 180.0f) * 3.14159265f) / 180.0f)));
    } else {
        return (float)((fDegrees * 3.14159265f) / 180.0f);
    }
}

#pragma optimize("y", off)
void CEntity::TeleportTo(float x, float y, float z)
{
    volatile unsigned long dwThisEntity = (unsigned long)m_pEntity;
    if (dwThisEntity && *(unsigned long*)m_pEntity != 0x863C40) {
        if (m_pEntity->nModelIndex != TRAIN_PASSENGER_LOCO &&
            m_pEntity->nModelIndex != TRAIN_FREIGHT_LOCO &&
            m_pEntity->nModelIndex != TRAIN_TRAM) {
            ((IEntityGTA*)dwThisEntity)->Teleport(x, y, z, 0);
        } else {
            ScriptCommand(&put_train_at, m_dwGTAId, x, y, z);
        }
    }
}
#pragma optimize("", on)

#pragma optimize("y", off)
void CEntity::PlayAudio(int event)
{
    volatile unsigned long entity = (unsigned long)m_pEntity;
    if (!entity) return;
    typedef void (CAudioClass::*AudioFunc)(int, int);
    union { void* p; AudioFunc fn; } u;
    u.p = (void*)0x4F6420;
    (((CAudioClass*)(entity + 0x138))->*u.fn)(event, 0);
}
#pragma optimize("", on)

#pragma optimize("y", off)
void CEntity::DeleteRwObject()
{
    if (!m_pEntity || !m_pEntity->pRwObject) return;
    IEntityGTA* volatile pEntity = (IEntityGTA*)m_pEntity;
    pEntity->DeleteRwObject();
}
#pragma optimize("", on)

void CEntity::ProcessControl(void)
{
}

void CEntity::AdvancePosition(void)
{
    if (!m_pEntity) return;
    float step = *(float*)0xB7CB5C;
    _MATRIX4X4 matrix;
    GetMatrix(&matrix);
    matrix.pos.X += step * m_pEntity->vecMoveSpeed.X;
    matrix.pos.Y += step * m_pEntity->vecMoveSpeed.Y;
    matrix.pos.Z += step * m_pEntity->vecMoveSpeed.Z;
    SetMatrixAndUpdate(matrix);
}

int CEntity::IsStationary(void)
{
    if (!m_pEntity || *(void**)m_pEntity == (void*)0x863C40 || !m_pEntity->pSector)
        return 0;

    if (m_pEntity->vecMoveSpeed.X == 0.0f &&
        m_pEntity->vecMoveSpeed.Y == 0.0f &&
        m_pEntity->vecMoveSpeed.Z == 0.0f)
    {
        return 1;
    }
    return 0;
}

int __cdecl GetInvalidModelInfoCount(void)
{
    int count = 0;
    for (unsigned long p = 0xA9C068; p != 0xAAE948; p += 4) {
        if (!*(void**)p) count++;
    }
    return count;
}

float __cdecl NormalizeAngle(float fAngle)
{
	if(fAngle > 180.0f) fAngle -= 360.0f;
	if(fAngle < -180.0f) fAngle += 360.0f;
	return fAngle;
}

#pragma optimize("s", on)
#pragma optimize("y", off)
void __cdecl ResetLocalPad(int unk1, int unk2) {
    void* fn = (void*)0x53fb70;
    CPad* pad = ((CPad* (__cdecl *)(int))fn)(0);
    union {
        void (CPad::*f)(int, int);
        void* addr;
    } u;
    u.addr = (void*)0x541a70;
    (pad->*u.f)(unk1, unk2);
}
#pragma optimize("", on)
int __cdecl IsFileOrDirectoryExists(char *szPath) {
    struct _stat32 buf;
    return _stat(szPath, &buf) == 0;
}

void __cdecl RemoveColorEmbedsFromString(char *szString) {
    while (*szString) {
        if (GetColorFromEmbedCode(szString) != 0xFFFFFFFF) {
            strcpy(szString, szString + 8);
            continue;
        }
        szString++;
    }
    *szString = 0;
}

bool FUNC_100B4B50(_VECTOR *vecPos)
{
	if (vecPos->X < 20000.0f && vecPos->X > -20000.0f &&
		vecPos->Y < 20000.0f && vecPos->Y > -20000.0f &&
		vecPos->Z < 100000.0f && vecPos->Z > -10000.0f)
	{
		return true;
	}
	return false;
}

void CGame::sub_100A0090(int a1, int a2)
{
	if (a1 && a2 && a1 < 1000 / a2)
		Sleep(1000 / a2 - a1 - 1);
}

float CGame::FindGroundZForCoord(float x, float y, float z)
{
	float fGroundZ;
	ScriptCommand(&get_ground_z, x, y, z, &fGroundZ);
	return fGroundZ;
}

void CGame::SetWorldTime(int iHour, int iMinute)
{
    *(unsigned char*)0xB70152 = (unsigned char)iMinute;
    *(unsigned char*)0xB70153 = (unsigned char)iHour;
    ScriptCommand(&set_current_time, iHour, iMinute);
}

void CGame::ToggleThePassingOfTime(unsigned char byteOnOff)
{
    Unprotect((void*)0x52CF10, 1);
    if (byteOnOff) {
        *(unsigned char*)0x52CF10 = 0x56;
        field_69 = 1;
    } else {
        *(unsigned char*)0x52CF10 = 0xC3;
        field_69 = 0;
    }
}

void CGame::SetFrameLimit(unsigned long dwLimit)
{
    field_5D = dwLimit;
    Unprotect((void*)0xC1704C, 4);
    *(unsigned long*)0xC1704C = 200;
}

// Matched ?OnEnd@CD3DArcBall@@QAEXXZ (0x10093bf0)
void CD3DArcBall::OnEnd() {
    m_bDrag = false;
}

// Matched ?IsKeySet@TEABlockEncryptor@@QBE_NXZ (0x10056990)
bool TEABlockEncryptor::IsKeySet() const {
    return m_bKeySet;
}

// Matched ?SetKey@TEABlockEncryptor@@QAEXQBE@Z (0x100569a0)
void TEABlockEncryptor::SetKey(const unsigned char* const pKey) {
    m_bKeySet = true;
}

// Matched ?UnsetKey@TEABlockEncryptor@@QAEXXZ (0x100569b0)
void TEABlockEncryptor::UnsetKey() {
    m_bKeySet = false;
}

// Encrypt 64 bits (v0 and v1) using Tiny Encryption Algorithm (TEA) with 32 rounds.
void TEABlockEncryptor::EncryptBlock(unsigned int &v0, unsigned int &v1) {
    unsigned int sum = 0;
    int rounds = 8;
    do {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);
        sum += m_delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);

        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);
        sum += m_delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);

        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);
        sum += m_delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);

        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);
        sum += m_delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);

        --rounds;
    } while (rounds != 0);
}

// Decrypt 64 bits (v0 and v1) using Tiny Encryption Algorithm (TEA) with 32 rounds.
void TEABlockEncryptor::DecryptBlock(unsigned int &v0, unsigned int &v1) {
    unsigned int sum = m_sumInit;
    int rounds = 8;
    do {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);
        sum -= m_delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);

        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);
        sum -= m_delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);

        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);
        sum -= m_delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);

        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + m_key[(sum >> 11) & 3]);
        sum -= m_delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + m_key[sum & 3]);

        --rounds;
    } while (rounds != 0);
}
