// Minimal layouts reconstructed from R5 machine-code accesses.
// Names/semantic leads: samp-reversed; layouts checked against frozen R5.
// Ordinary C++ only: no copied instructions, inline assembly, or _emit.
#pragma pack(push, 1)
class PlayerPool {
public:
    char prefix[0x26];
    void* localPlayer;
    int slots[1004];
    char gap[0xfb0];
    struct NetPlayer { char prefix[0x10]; void* remote; } *players[1004];
    int GetSlotState(unsigned short id);
    void* GetAt(unsigned short id);
    void* GetLocalPlayer();
};
int PlayerPool::GetSlotState(unsigned short id) {
    if (id >= 1004) return 0;
    return slots[id];
}
void* PlayerPool::GetAt(unsigned short id) {
    if (id > 1004 || !players[id]) return 0;
    return players[id]->remote;
}
void* PlayerPool::GetLocalPlayer() {
    return localPlayer;
}

class LocalPlayer {
public:
    char prefix[0x94];
    unsigned short key3, key2, key1;
    int AreKeysChanged(unsigned short a, unsigned short b, unsigned short c);
};
// Compares a freshly sampled key state with the keys cached on the local
// player; returns 1 when any of the three changed.
int LocalPlayer::AreKeysChanged(unsigned short keyA, unsigned short keyB, unsigned short keyC) {
    if (keyA == key1 && keyB == key2 && keyC == key3) return 0;
    return 1;
}

struct _WEAPON_SLOT_TYPE {
    unsigned long dwType;
    char gap[8];
    unsigned long dwAmmo;
    char gap2[12];
};

struct _VEHICLE_TYPE {
    char gap0[0x22];
    unsigned short nModelIndex;     // 0x22
    char gap1[0x43c];               // 0x24 to 0x460
    void* pDriver;                  // 0x460
    char gap2[0xb0];
    unsigned char byteHorn2;        // 0x514
};
struct _ENTITY_TYPE {
    void* vtable;
    char gap0[0x10];
    struct _MATRIX4X4* mat;
    void* pRwObject;
};

struct PED_TASKS_TYPE {
    char gap[0xc];
    unsigned long* pdwSwimWasted;  // 0x0c
    unsigned long* pdwJumpJetPack; // 0x10
    char gap2[0x18];
    unsigned long* pdwIK;          // 0x2c
};

struct PED_TYPE {
    _ENTITY_TYPE entity;
    char gap0[0x450];
    unsigned long dwStateFlags;     // 0x46c
    char gap_task[0xc];             // 0x470 to 0x47c
    PED_TASKS_TYPE* Tasks;          // 0x47c
    unsigned long dwPlayerInfoOffset;// 0x480
    char gap_goggles[0x7c];         // 0x484 to 0x500
    void* pGoggles;                 // 0x500
    char gap1[0x2c];                // 0x504 to 0x530
    unsigned long dwActionTrigger;  // 0x530
    char gap2[0xc];                 // 0x534 to 0x540
    float fHealth;                  // 0x540
    char gap3[0x4];
    float fArmour;                  // 0x548
    char gap4[0xc];
    float fRotation1;               // 0x558
    float fRotation2;               // 0x55c
    char gap4_2[0x8];
    _VEHICLE_TYPE* pContactVehicle; // 0x568
    char gap5[0x18];
    _ENTITY_TYPE* pContactEntity;   // 0x584
    char gap6[0x4];
    _VEHICLE_TYPE* pVehicle;        // 0x58c
    char gap_wep[0x10];             // 0x590 to 0x5a0
    _WEAPON_SLOT_TYPE weaponSlots[13]; // 0x5a0 to 0x70c
    char gap_wep_slot[0xc];         // 0x70c to 0x718
    unsigned char byteCurrentWeaponSlot; // 0x718
    char gap7[0x14];
    unsigned char byteFightingStyle;// 0x72d
};

struct _CAMERA_AIM;
struct _MATRIX4X4;
void __stdcall GameStoreRemotePlayerAim(int iPlayer, struct _CAMERA_AIM* caAim);
void __stdcall SetRadarColor(int nIndex, unsigned long dwColor);
unsigned long __stdcall TranslateColorCodeToRGBA(int iCode);
float DegToRad(float fDegrees);
extern unsigned char* pbyteCurrentPlayer;

class CRemotePlayer {
public:
    char gap[0x1e5];
    unsigned short m_PlayerID;
    void SetPlayerColor(unsigned long color);
    unsigned long GetPlayerColorAsARGB();
};

struct InternalPacket;
namespace DataStructures {
template <class T>
class Queue {
public:
    void Push(const T& input);
};
}

class InternalPacketPool {
public:
    DataStructures::Queue<InternalPacket*> pool;
    void ReleasePointer(InternalPacket *p);
};

PED_TYPE* __stdcall GamePool_Ped_GetAt(int nPedIndex);
extern unsigned char* pbyteCurrentPlayer;
int __cdecl GetTaskTypeFromTask(unsigned long *task);
unsigned long __stdcall TranslateColorCodeToRGBA(int iCode);

class CJetpackTask {
public:
    void Destroy(int flag);
};

struct GTA_CONTROLSET {
    unsigned short wKeys1[24];
    unsigned short wKeys2[24];
    unsigned char bytePadding1[212];
};
GTA_CONTROLSET* __cdecl GameGetInternalKeys();

struct SCRIPT_COMMAND {
    unsigned short OpCode;
    char Params[16];
};
int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);
extern "C" const SCRIPT_COMMAND get_actor_armed_weapon;
extern "C" const SCRIPT_COMMAND disable_marker;
extern "C" const SCRIPT_COMMAND destroy_pickup;
extern "C" void __cdecl free(void*);
extern "C" const SCRIPT_COMMAND set_actor_immunities;
extern "C" const SCRIPT_COMMAND remove_actor_from_car_and_put_at;
extern "C" const SCRIPT_COMMAND actor_task_handsup;
extern "C" const SCRIPT_COMMAND toggle_actor_cellphone;
extern "C" const SCRIPT_COMMAND set_actor_money;
extern "C" const SCRIPT_COMMAND set_fighting_style;
extern unsigned long dwExt4;
GTA_CONTROLSET* __cdecl GameGetPlayerKeys(int iPlayer);
extern "C" size_t __cdecl strlen(const char*);
#pragma intrinsic(strlen)
extern "C" int __cdecl _isnan(double);
const SCRIPT_COMMAND set_actor_armed_weapon = { 0x01b9, "ii" };
extern char DanceStyleLibs[4][16];
extern char DanceIdleLoops[4][16];
void __stdcall GameStoreLocalPlayerWeaponSkills();
void __stdcall GameSetRemotePlayerWeaponSkills(int iPlayer);
void __stdcall GameSetLocalPlayerWeaponSkills();
unsigned long __stdcall GamePool_Vehicle_GetIndex(_VEHICLE_TYPE *pVehicle);

struct _VECTOR;
class CVehicle;
class __single_inheritance CCommandTaskManager {
public:
    void Apply(char* szTaskName, unsigned long dwPed, int p1, int p2, int p3, _VECTOR* p4, int p5, float p6, int p7, int p8, int p9);
};
class __single_inheritance CGtaPedWeapon {
public:
    void SetArmedWeapon(int iWeaponType);
    void ClearWeapon(int unk);
};

class LocalPlayerRPCResetView {
public:
    char unknown[0x2da];
    int field_2DA;
    int field_2DE;
    int field_2E2;
    void SetStatePair(int first, int second);
};

class CPlayerPed {
public:
    char gap[0x40];
    _ENTITY_TYPE* m_pEntity;        // 0x40
    unsigned long m_dwGTAId;        // 0x44
    int m_iCellPhoneEnabled;        // 0x48
    int m_bObjectSlotUsed[10];      // 0x4c
    char gap_ped[0x230];            // 0x74 to 0x2a4
    PED_TYPE *m_pPed;               // 0x2a4
    char gap_dance[8];              // 0x2a8 to 0x2b0
    unsigned char m_bytePlayerNumber;// 0x2b0
    char gap_dance2[0x1c];          // 0x2b1 to 0x2cd
    unsigned long m_dwArrow;        // 0x2cd
    char gap_dance3[1];             // 0x2d1
    int m_iDanceState;              // 0x2d2
    int m_iDanceStyle;              // 0x2d6
    char gap_thermal[0xc];          // 0x2da to 0x2e6
    int m_bHasThermalGoggles;       // 0x2e6
    char gap_piss[8];               // 0x2ea to 0x2f2
    int m_iPissingState;            // 0x2f2

    int GetCurrentVehicleID();
    void HideMarker();
    void SetImmunities(int bBullet, int bFire, int bExplosion, int bDamage, int bUnknown);
    int IsSwimming();
    void StartJetpack();
    void StopJetpack();
    void TeleportTo(float x, float y, float z);
    void ToggleCellphone(int iOn);
    void StartDancing(int iStyle);
    void StopDancing();
    void ApplyCommandTask(char* szTaskName, int p1, int p2, int p3, _VECTOR* p4, int p5, float p6, int p7, int p8, int p9);
    void ApplyAnimation(char* szAnimName, char* szAnimFile, float fT, int opt1, int opt2, int opt3, int opt4, int opt5);

    int IsCellphoneEnabled();
    int GetPissingState();

    unsigned char GetActionTrigger();
    _VEHICLE_TYPE* GetGtaVehicle();
    _ENTITY_TYPE* GetGtaContactEntity();
    _VEHICLE_TYPE* GetGtaContactVehicle();
    int IsDancing();
    unsigned long GetStateFlags();
    int GetFightingStyle();
    void SetHealth(float fHealth);
    void SetArmour(float fArmour);
    void SetStateFlags(unsigned long dwState);
    void SetActionTrigger(unsigned char byteTrigger);
    int IsInVehicle();
    int IsOnGround();
    int HasObjectAttached();
    _WEAPON_SLOT_TYPE* GetCurrentWeaponSlot();
    int HasGoggles();
    int IsAPassenger();
    _WEAPON_SLOT_TYPE* FindWeaponSlot(unsigned long dwWeapon);
    int IsInJetpackMode();    float GetHealth();
    float GetArmour();
    int IsDead();
    void SetCurrentAim(struct _CAMERA_AIM* pAim);
    void ResetPointers();
    void SetInitialState();
    unsigned char GetSpecialKey();
    unsigned char GetCurrentWeapon();
    float GetTargetRotation();
    void SetTargetRotation(float fRotation);
    void ClearAllWeapons();
    void GetMatrix(_MATRIX4X4* pMatrix);
    void SetArmedWeapon(int iWeaponType, bool bUnk);
    void RemoveWeaponWhenEnteringVehicle();
    float GetDistanceFromVehicle(CVehicle* pVehicle);
    void RemoveFromVehicleAndPutAt(float fX, float fY, float fZ);
    void HandsUp();
    int HasHandsUp();
    int IsAdded();
    void HoldItem(int iObject);
    int IsJumping();
    int IsFalling();
    void SetMoney(int iAmount);
    float GetAimZ();
    void SetAimZ(float fAimZ);
    void SetAmmo(unsigned char byteWeapon, unsigned short wAmmo);
    char* GetShopName();
    _VECTOR* GetBonePosition(int iBone, _VECTOR* pVec);
    _VECTOR* GetTransformedBonePosition(int iBone, _VECTOR* pVec);
};
int __cdecl GetTaskTypeFromTask(unsigned long *task);

struct IDirect3DStateBlock9 {
    virtual void Dummy0();
    virtual void Dummy1();
    virtual unsigned long __stdcall Release();
    virtual void Dummy3();
    virtual void Dummy4();
    virtual long __stdcall Apply();
};

class CPlayerTags {
public:
    void* m_pDevice;
    IDirect3DStateBlock9* m_pTexture;
    IDirect3DStateBlock9* m_pVB;
    IDirect3DStateBlock9* m_pOldStates;
    IDirect3DStateBlock9* m_pNewStates;

    ~CPlayerTags();
    void DeleteDeviceObjects();
    void RestoreDeviceObjects();
    void End();
};

struct ID3DXSprite {
    virtual void Dummy0();
    virtual void Dummy1();
    virtual void Dummy2();
    virtual void Dummy3();
    virtual void Dummy4();
    virtual void Dummy5();
    virtual void Dummy6();
    virtual void Dummy7();
    virtual void Dummy8();
    virtual void Dummy9();
    virtual void Dummy10();
    virtual long __stdcall End();
    virtual long __stdcall OnLostDevice();
};

class CNewPlayerTags {
public:
    void* m_pDevice;
    IDirect3DStateBlock9* m_pStates;
    ID3DXSprite* m_pSprite;

    void End();
    void DeleteDeviceObjects();
};

#pragma pack(push, 1)
class R5LabelRemotePlayerView {
public:
    char prefix[0x10a];
    unsigned char state;
    char gap10b[0x1dd - 0x10b];
    void* ped;
    int IsActive();
};
#pragma pack(pop)

int CPlayerPed::IsCellphoneEnabled() {
    return m_iCellPhoneEnabled;
}
int CPlayerPed::GetPissingState() {
    return m_iPissingState;
}

unsigned char CPlayerPed::GetActionTrigger() {
    return (unsigned char)m_pPed->dwActionTrigger;
}

_VEHICLE_TYPE* CPlayerPed::GetGtaVehicle() {
    return m_pPed->pVehicle;
}

_ENTITY_TYPE* CPlayerPed::GetGtaContactEntity() {
    return m_pPed->pContactEntity;
}

_VEHICLE_TYPE* CPlayerPed::GetGtaContactVehicle() {
    return m_pPed->pContactVehicle;
}

int CPlayerPed::IsDancing() {
    return m_iDanceState != 0;
}

unsigned long CPlayerPed::GetStateFlags() {
    if (!m_pPed) return 0;
    return m_pPed->dwStateFlags;
}

int CPlayerPed::GetFightingStyle() {
    if (!m_pPed) return 0;
    return m_pPed->byteFightingStyle;
}

void CPlayerPed::SetHealth(float fHealth) {
    if (!m_pPed) return;
    m_pPed->fHealth = fHealth;
}

void CPlayerPed::SetArmour(float fArmour) {
    if (!m_pPed) return;
    m_pPed->fArmour = fArmour;
}

void CPlayerPed::SetStateFlags(unsigned long dwState) {
    if (!m_pPed) return;
    m_pPed->dwStateFlags = dwState;
}

void CPlayerPed::SetActionTrigger(unsigned char byteTrigger) {
    if (!m_pPed) return;
    m_pPed->dwActionTrigger = byteTrigger;
}

int CPlayerPed::IsInVehicle() {
    if (!m_pPed) return 0;
    return (m_pPed->dwStateFlags >> 8) & 1;
}

int CPlayerPed::IsOnGround() {
    if (m_pPed && (m_pPed->dwStateFlags & 3)) return 1;
    return 0;
}

int CPlayerPed::HasObjectAttached() {
    for (int i = 0; i < 10; ++i) {
        if (m_bObjectSlotUsed[i] == 1) return 1;
    }
    return 0;
}

_WEAPON_SLOT_TYPE* CPlayerPed::GetCurrentWeaponSlot() {
    if (m_pPed) {
        return &m_pPed->weaponSlots[m_pPed->byteCurrentWeaponSlot];
    }
    return 0;
}

int CPlayerPed::HasGoggles() {
    if (!m_pPed) return 0;
    if (m_pPed->pGoggles || m_bHasThermalGoggles) return 1;
    return 0;
}

// A player is a passenger when sitting in a vehicle whose driver is not
// this ped, except in the two models that always count as passenger seats.
int CPlayerPed::IsAPassenger() {
    PED_TYPE* pPed = m_pPed;
    _VEHICLE_TYPE* pVehicle = pPed->pVehicle;
    if (pVehicle && (pPed->dwStateFlags & 0x100)) {
        if (pVehicle->pDriver != pPed || pVehicle->nModelIndex == 0x23a || pVehicle->nModelIndex == 0x239)
            return 1;
    }
    return 0;
}

// Look up the slot holding the given weapon, or null when the ped does not carry it.
_WEAPON_SLOT_TYPE* CPlayerPed::FindWeaponSlot(unsigned long dwWeapon) {
    PED_TYPE* pPed = m_pPed;
    if (pPed) {
        for (unsigned char i = 0; i < 13; ++i) {
            if (pPed->weaponSlots[i].dwType == dwWeapon)
                return &pPed->weaponSlots[i];
        }
    }
    return 0;
}

// True when the ped is alive/valid, not flagged in state 0x100, and its
// primary task is the jetpack task (vtable 0x8705C4).
int CPlayerPed::IsInJetpackMode() {
    if (m_pPed && !(m_pPed->dwStateFlags & 0x100)) {
        void* pTaskManager = *(void**)((char*)m_pPed + 0x47c);
        void* pTask = *(void**)((char*)pTaskManager + 0x10);
        if (pTask) {
            return *(void**)pTask == (void*)0x8705c4;
        }
    }
    return 0;
}

class CObject {
public:
    char gap[0x40];
    _ENTITY_TYPE* m_pEntity;
};

class CObjectPool {
public:
    int lastActive;
    int slots[1000];
    CObject* objects[1000];

    CObjectPool();
    int FindIDFromGtaPtr(_ENTITY_TYPE* pGtaEntity);
    CObject* GetAt(unsigned short id);
};

CObjectPool::CObjectPool() {
    int i = 1000;
    unsigned long* p = (unsigned long*)this;
    do {
        p[1] = 0;
        p[0x3e9] = 0;
        --i;
        ++p;
    } while (i != 0);
    lastActive = 0;
}

// Matched ?GetAt@CObjectPool@@QAEPAVCObject@@G@Z (0x10002de0)
CObject* CObjectPool::GetAt(unsigned short id) {
    if (id > 1000 || !slots[id]) return 0;
    return objects[id];
}

// Finds the slot whose CObject wraps pGtaEntity; 0xffff when it is not pooled.
int CObjectPool::FindIDFromGtaPtr(_ENTITY_TYPE* pGtaEntity) {
    int id = 1;
    if (lastActive >= 1) {
        do {
            CObject* pObj = objects[id];
            if (pObj && pGtaEntity == pObj->m_pEntity)
                return id;
            ++id;
        } while (id <= lastActive);
    }
    return 0xffff;
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

extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl atan2(double, double);
#pragma intrinsic(sqrt, atan2)

class RPCObjectOpsView {
public:
    char gap[0x40];
    _ENTITY_TYPE* m_pEntity; // 0x40
    char gap1[4];            // 0x44
    unsigned char m_bIsAdded;// 0x48
    char gap2[0xc2];         // 0x49 to 0x10b
    _VECTOR m_vecTarget;     // 0x10b to 0x117

    int IsAdded();
    float DistanceRemaining(_MATRIX4X4* pMatrix);
};

int RPCObjectOpsView::IsAdded() {
    if (m_pEntity && *(void**)m_pEntity != (void*)0x863c40 && m_pEntity->pRwObject && m_bIsAdded)
        return 1;
    return 0;
}

// Euclidean distance from the object's current matrix position to its target.
float RPCObjectOpsView::DistanceRemaining(_MATRIX4X4* pMatrix) {
    float fSZ = pMatrix->pos.Z - m_vecTarget.Z;
    float fSY = pMatrix->pos.Y - m_vecTarget.Y;
    float fSX = pMatrix->pos.X - m_vecTarget.X;
    return (float)sqrt((double)(fSX * fSX + fSY * fSY + fSZ * fSZ));
}

class CVehicle {
public:
    void GetMatrix(_MATRIX4X4* pMatrix);
    void LinkToInterior(int interior);
};

class RPCVehiclePoolView {
public:
    char gap0[0x1134];
    CVehicle* m_pVehicles[2000];
    int m_bVehicleSlotState[2000];

    void LinkToInterior(unsigned short id, int interior);
};
class R5VehicleTrailerPoolView {
public:
    char gap0[0x1134];
    CVehicle* m_pVehicles[2000]; // 0x1134
    int m_bSlotState[2000];      // 0x3074

    CVehicle* GetAt(unsigned short id);
    int GetSlotState(unsigned short id);
};

CVehicle* R5VehicleTrailerPoolView::GetAt(unsigned short id) {
    if (id >= 2000 || !m_bSlotState[id]) return 0;
    return m_pVehicles[id];
}

int R5VehicleTrailerPoolView::GetSlotState(unsigned short id) {
    if (id >= 2000) return 0;
    return m_bSlotState[id];
}

class RPCVehicleMotionPoolView {
public:
    int lastActive;              // 0x00
    char gap[0x4fb0];            // 0x04 to 0x4fb4
    _VEHICLE_TYPE* vehicles[2000]; // 0x4fb4

    unsigned short FindIDFromGtaPtr(_VEHICLE_TYPE* pVehicle);
};

unsigned short RPCVehicleMotionPoolView::FindIDFromGtaPtr(_VEHICLE_TYPE* pVehicle) {
    int i = 1;
    if (lastActive >= 1) {
        do {
            if (pVehicle == vehicles[i]) return (unsigned short)i;
            ++i;
        } while (i <= lastActive);
    }
    return 0xffff;
}

class CPickupPoolMore {
public:
    int m_iPickupCount;
    unsigned long m_dwHnd[4096];
    int m_iNetworkIds[4096];

    int FindNetworkId(int networkId);
};

int CPickupPoolMore::FindNetworkId(int networkId) {
    for (int i = 0; i != 4096; i++) {
        if (m_iNetworkIds[i] == networkId) return i;
    }
    return -1;
}

class RPCDeleteObjectPoolView {
public:
    int m_iLargestID;
    int m_slots[1000];

    void UpdateLargestID();
};

// Find the highest occupied slot so later scans can stop there.
void RPCDeleteObjectPoolView::UpdateLargestID() {
    int last = 0;
    for (int i = 0; i < 1000; ++i) {
        if (m_slots[i]) last = i;
    }
    m_iLargestID = last;
}

struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
};
typedef struct tagRECT RECT;

class R5TextDrawSelectionView {
public:
    char gap0[0x9a7];
    unsigned char selectable;    // 0x9a7
    char gap1[0x17];             // 0x9a8
    unsigned char drawn;         // 0x9bf
    char gap2[1];                // 0x9c0
    RECT bounds;                 // 0x9c1
    unsigned char highlighted;   // 0x9d1
    unsigned long highlightColor;// 0x9d2

    RECT* GetSelectableRect();
    inline void SetHighlight(bool value, unsigned long color) {
        highlighted = value;
        highlightColor = color;
    }
};

RECT* R5TextDrawSelectionView::GetSelectableRect() {
    if (selectable && drawn) return &bounds;
    return 0;
}

class R5TextDrawSelectionPoolView {
public:
    int active[2304];
    R5TextDrawSelectionView* items[2304];

    inline R5TextDrawSelectionView* GetAt(unsigned short id) {
        if (id >= 2304) return 0;
        if (!active[id]) return 0;
        return items[id];
    }
};

struct R5TextDrawSelectionPoolsView {
    char gap0[0x1c];
    R5TextDrawSelectionPoolView* textdrawPool;
};

struct R5TextDrawSelectionNetView {
    char gap0[0x3de];
    R5TextDrawSelectionPoolsView* pools;
};

extern "C" void* pNetGame;

class R5TextDrawSelectorView {
public:
    int enabled;
    unsigned long highlightColor;
    unsigned short selectedId;

    void ClearAll();
    void Enable(unsigned long color);
};

// Removes the selection highlight from every text draw in the pool
// (2048 global + 256 player text draws = 2304 slots).
void R5TextDrawSelectorView::ClearAll() {
    if (pNetGame) {
        R5TextDrawSelectionPoolView* pool = ((R5TextDrawSelectionNetView*)pNetGame)->pools->textdrawPool;
        if (pool) {
            int x = 0;
            while (x != 2304) {
                R5TextDrawSelectionView* text = pool->GetAt((unsigned short)x);
                if (text) text->SetHighlight(false, 0);
                x++;
            }
        }
    }
}

// Enables the selector, converts the packed colour to the game's byte order
// (a full 32-bit byte swap) and clears the current selection.
void R5TextDrawSelectorView::Enable(unsigned long color) {
    unsigned long upper = (color >> 16) | (color & 0xff0000);
    unsigned long lower = (color << 16) | (color & 0xff00);
    unsigned long converted = (lower << 8) | (upper >> 8);
    enabled = 1;
    highlightColor = converted;
    selectedId = 0xffff;
}

class CDXUTScrollBar {
public:
    char gap0[0x8e];
    int m_nPosition;

    void Cap();
    void UpdateThumbRect();
};

class R5ChatScrollBarNavigationView : public CDXUTScrollBar {
public:
    void R5SetTrackPos(int position);
};

void R5ChatScrollBarNavigationView::R5SetTrackPos(int position) {
    m_nPosition = position;
    Cap();
    UpdateThumbRect();
}

class R5ChatPagingView {
public:
    void FilterInvalidChars(char* szString);
};

void R5ChatPagingView::FilterInvalidChars(char* szString) {
    while (*szString) {
        if (*szString > 0 && *szString < ' ') {
            *szString = ' ';
        }
        szString++;
    }
}

extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)

struct CTextDrawItem {
    char data[29];
};

class CTextDrawPool {
public:
    CTextDrawItem m_items[2048];
    void* m_pObjects[2048];

    CTextDrawPool();
};

class CMenu {
public:
    void Show();
    void Hide();
};

class CMenuPool {
public:
    CMenu* m_pObject[128];
    int m_bNotEmpty[128];
    unsigned char m_nCurrent;
    char m_bCanceled;

    CMenuPool();
    void ShowMenu(unsigned char byteMenuID);
    void HideMenu(unsigned char byteMenuID);
};

struct CGangZone {
    char gap[0x10];
    unsigned long color;
    unsigned long flashColor;
};

class CGangZonePool {
public:
    CGangZone* m_pGangZone[1024];
    int m_bSlotState[1024];

    CGangZonePool();
    ~CGangZonePool();
    void Flash(unsigned short id, unsigned long color);
    void StopFlash(unsigned short id);
};

class CVehiclePool;
#pragma pack(push, 1)
class CPlayerPool {
public:
    char gap[4];
    unsigned short m_LocalPlayerID;
    unsigned short GetLocalPlayerID() { return m_LocalPlayerID; }
};
#pragma pack(pop)
class CPickupPool;
class CActorPool;
class CLabelPool;

struct Pools {
    CVehiclePool* m_pVehicle;   // 0x00
    CPlayerPool* m_pPlayer;     // 0x04
    CPickupPool* m_pPickup;     // 0x08
    CObjectPool* m_pObject;     // 0x0C
    CActorPool* m_pActor;       // 0x10
    CGangZonePool* m_pGangZone; // 0x14
    CLabelPool* m_pLabel;       // 0x18
    CTextDrawPool* m_pTextDraw; // 0x1C
    CMenuPool* m_pMenu;         // 0x20
};

class CNetGame {
public:
    char gap[0x3de];
    Pools* m_pPools;

    CVehiclePool* GetVehiclePool();
    CPlayerPool* GetPlayerPool();
    CPickupPool* GetPickupPool();
    CObjectPool* GetObjectPool();
    CActorPool* GetActorPool();
    CGangZonePool* GetGangZonePool();
    CLabelPool* GetLabelPool();
    CTextDrawPool* GetTextDrawPool();
    CMenuPool* GetMenuPool();
};

#pragma pack(pop)

class CLocalPlayer {
public:
    void SetPlayerColor(unsigned long color);
    unsigned long GetPlayerColorAsARGB();
};

// Matched ?GetVehiclePool@CNetGame@@QAEPAVCVehiclePool@@XZ (0x10001180)
CVehiclePool* CNetGame::GetVehiclePool() { return m_pPools->m_pVehicle; }

// Matched ?GetPlayerPool@CNetGame@@QAEPAVCPlayerPool@@XZ (0x10001170)
CPlayerPool* CNetGame::GetPlayerPool() { return m_pPools->m_pPlayer; }

// Matched ?GetPickupPool@CNetGame@@QAEPAVCPickupPool@@XZ (0x100084e0)
CPickupPool* CNetGame::GetPickupPool() { return m_pPools->m_pPickup; }

// Matched ?GetObjectPool@CNetGame@@QAEPAVCObjectPool@@XZ (0x10002e10)
CObjectPool* CNetGame::GetObjectPool() { return m_pPools->m_pObject; }

// Matched ?GetActorPool@CNetGame@@QAEPAVCActorPool@@XZ (0x10002e20)
CActorPool* CNetGame::GetActorPool() { return m_pPools->m_pActor; }

// Matched ?GetGangZonePool@CNetGame@@QAEPAVCGangZonePool@@XZ (0x10017d00)
CGangZonePool* CNetGame::GetGangZonePool() { return m_pPools->m_pGangZone; }

void CPlayerPed::SetCurrentAim(_CAMERA_AIM* pAim) {
    GameStoreRemotePlayerAim(m_bytePlayerNumber, pAim);
}

void CRemotePlayer::SetPlayerColor(unsigned long color) {
    SetRadarColor(m_PlayerID, color);
}

void CLocalPlayer::SetPlayerColor(unsigned long color)
{
    SetRadarColor(((CNetGame*)pNetGame)->m_pPools->m_pPlayer->GetLocalPlayerID(), color);
}

unsigned long CLocalPlayer::GetPlayerColorAsARGB()
{
    return (TranslateColorCodeToRGBA(((CNetGame*)pNetGame)->m_pPools->m_pPlayer->GetLocalPlayerID()) >> 8) | 0xFF000000;
}

unsigned long CRemotePlayer::GetPlayerColorAsARGB()
{
    return (TranslateColorCodeToRGBA(m_PlayerID) >> 8) | 0xFF000000;
}

void InternalPacketPool::ReleasePointer(InternalPacket *p) {
    if (p) {
        pool.Push(p);
    }
}

void CPlayerPed::ResetPointers(void)
{
    m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
    m_pEntity = (_ENTITY_TYPE*)m_pPed;
}

void CPlayerPed::SetInitialState(void)
{
}

unsigned char CPlayerPed::GetSpecialKey(void)
{
    GTA_CONTROLSET *pInternalKeys = GameGetInternalKeys();

    if (pInternalKeys->wKeys1[11])
        return 1;
    else if (pInternalKeys->wKeys1[10])
        return 2;
    else if (pInternalKeys->wKeys1[9])
        return 3;

    return 0;
}

unsigned char CPlayerPed::GetCurrentWeapon(void)
{
    if (!m_pPed) return 0;
    if (GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0;

    unsigned long dwRetVal;
    ScriptCommand(&get_actor_armed_weapon, m_dwGTAId, &dwRetVal);
    return (unsigned char)dwRetVal;
}

int CPlayerPed::GetCurrentVehicleID() {
    if (!m_pPed) return 0;
    return GamePool_Vehicle_GetIndex(m_pPed->pVehicle);
}

void CPlayerPed::HideMarker() {
    if (m_dwArrow) ScriptCommand(&disable_marker, m_dwArrow);
    m_dwArrow = 0;
}

void CPlayerPed::SetImmunities(int bBullet, int bFire, int bExplosion, int bDamage, int bUnknown) {
    if (!m_pPed) return;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return;
    ScriptCommand(&set_actor_immunities, m_dwGTAId, bBullet, bFire, bExplosion, bDamage, bUnknown);
}

float CPlayerPed::GetTargetRotation()
{
    if (!m_pPed) return 0.0f;
    if (GamePool_Ped_GetAt(m_dwGTAId) == 0) return 0.0f;

    _MATRIX4X4 mat;
    GetMatrix(&mat);

    float fZAngle = (float)atan2(-mat.up.X, mat.up.Y) * (180.0f / 3.14159265f);
    if (fZAngle < 0.0f)
        fZAngle += 360.0f;
    else if (fZAngle >= 360.0f)
        fZAngle -= 360.0f;
    return fZAngle;
}

void CPlayerPed::SetTargetRotation(float fRotation)
{
    if (!m_pPed) return;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

    m_pPed->fRotation2 = DegToRad(fRotation);
    m_pPed->fRotation1 = DegToRad(fRotation);
}

#pragma optimize("y", off)
void CPlayerPed::ClearAllWeapons()
{
    volatile unsigned long dwPedPtr = (unsigned long)m_pPed;

    *pbyteCurrentPlayer = m_bytePlayerNumber;

    if (dwPedPtr) {
        typedef void (PED_TYPE::*ClearFn)();
        union {
            void* p;
            ClearFn fn;
        } u;
        u.p = (void*)0x5E6320;
        (((PED_TYPE*)dwPedPtr)->*u.fn)();
        *pbyteCurrentPlayer = 0;
        return;
    }

    *pbyteCurrentPlayer = 0;
}

void CPlayerPed::SetArmedWeapon(int iWeaponType, bool bUnk) {
    if (!m_pPed) return;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return;

    *pbyteCurrentPlayer = m_bytePlayerNumber;

    GameStoreLocalPlayerWeaponSkills();
    GameSetRemotePlayerWeaponSkills(m_bytePlayerNumber);

    if ((!m_pPed || !(m_pPed->dwStateFlags & 0x100)) && !bUnk) {
        ScriptCommand(&set_actor_armed_weapon, m_dwGTAId, iWeaponType);
    } else {
        void* dwPedPtr = m_pPed;
        if (dwPedPtr) {
            typedef void (CGtaPedWeapon::*SetArmedFn)(int);
            union { void* p; SetArmedFn fn; } u;
            u.p = (void*)0x5E6280;
            (((CGtaPedWeapon*)dwPedPtr)->*u.fn)(iWeaponType);
        }
    }

    GameSetLocalPlayerWeaponSkills();
    *pbyteCurrentPlayer = 0;
}

#pragma optimize("", off)
void CPlayerPed::RemoveWeaponWhenEnteringVehicle() {
    void* dwPedPtr = m_pPed;
    if (dwPedPtr) {
        typedef void (CGtaPedWeapon::*ClearWeaponFn)(int);
        union { void* p; ClearWeaponFn fn; } u;
        u.p = (void*)0x5E6370;
        (((CGtaPedWeapon*)dwPedPtr)->*u.fn)(0);
    }
}
#pragma optimize("", on)

#pragma optimize("y", on)
float CPlayerPed::GetDistanceFromVehicle(CVehicle* pVehicle) {
    _MATRIX4X4 matFromPlayer;
    _MATRIX4X4 matThis;
    float fSX, fSY, fSZ;

    GetMatrix(&matThis);
    pVehicle->GetMatrix(&matFromPlayer);

    fSX = (matThis.pos.X - matFromPlayer.pos.X) * (matThis.pos.X - matFromPlayer.pos.X);
    fSY = (matThis.pos.Y - matFromPlayer.pos.Y) * (matThis.pos.Y - matFromPlayer.pos.Y);
    fSZ = (matThis.pos.Z - matFromPlayer.pos.Z) * (matThis.pos.Z - matFromPlayer.pos.Z);

    return (float)sqrt((double)(fSX + fSY + fSZ));
}
#pragma optimize("", on)

#pragma optimize("y", on)
void CPlayerPed::RemoveFromVehicleAndPutAt(float fX, float fY, float fZ) {
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return;
    if (m_pPed && (m_pPed->dwStateFlags & 0x100)) {
        ScriptCommand(&remove_actor_from_car_and_put_at, m_dwGTAId, fX, fY, fZ);
    }
}
#pragma optimize("", on)
void CPlayerPed::HandsUp() {
    if (!m_pPed || (m_pPed->dwStateFlags & 0x100)) return;
    if (!IsAdded()) return;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return;
    ScriptCommand(&actor_task_handsup, m_dwGTAId, -2);
}

int CPlayerPed::HasHandsUp() {
    if (!m_pPed || (m_pPed->dwStateFlags & 0x100)) return 0;
    if (!IsAdded()) return 0;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return 0;
    if (m_pPed->Tasks->pdwJumpJetPack == 0) return 0;
    return m_pPed->Tasks->pdwJumpJetPack[0] == 0x85a29c;
}

void CPlayerPed::HoldItem(int iObject)
{
}

int CPlayerPed::IsJumping(void)
{
    if (m_pPed && !(m_pPed->dwStateFlags & 0x100))
    {
        void* pTaskManager = *(void**)((char*)m_pPed + 0x47c);
        unsigned long* pTask = *(unsigned long**)((char*)pTaskManager + 0x10);
        if (pTask)
        {
            return GetTaskTypeFromTask(pTask) == 211;
        }
    }
    return 0;
}

int CPlayerPed::IsFalling(void)
{
    if (m_pPed && !(m_pPed->dwStateFlags & 0x100))
    {
        void* pTaskManager = *(void**)((char*)m_pPed + 0x47c);
        unsigned long* pTask = *(unsigned long**)((char*)pTaskManager + 0x4);
        if (pTask)
        {
            return GetTaskTypeFromTask(pTask) == 208;
        }
    }
    return 0;
}

int CPlayerPed::IsSwimming() {
    if (m_pPed && !(m_pPed->dwStateFlags & 0x100) && m_pPed->Tasks->pdwSwimWasted) {
        return GetTaskTypeFromTask(m_pPed->Tasks->pdwSwimWasted) == 268;
    }
    return 0;
}

void CPlayerPed::StartJetpack() {
    if (!m_pPed) return;
    *pbyteCurrentPlayer = m_bytePlayerNumber;
    TeleportTo(m_pPed->entity.mat->pos.X, m_pPed->entity.mat->pos.Y, m_pPed->entity.mat->pos.Z);
    ((void(*)())0x439600)();
    *pbyteCurrentPlayer = 0;
}

#pragma optimize("y", off)
void CPlayerPed::StopJetpack() {
    if (m_pPed && !(m_pPed->dwStateFlags & 0x100)) {
        PED_TASKS_TYPE* tasks = m_pPed->Tasks;
        if (tasks->pdwJumpJetPack && *tasks->pdwJumpJetPack == 0x8705C4) {
            volatile unsigned long dwJetPackTask = (unsigned long)tasks->pdwJumpJetPack;
            typedef void (CJetpackTask::*DestroyFn)(int);
            union { void* p; DestroyFn fn; } u;
            u.p = (void*)0x6801D0;
            (((CJetpackTask*)dwJetPackTask)->*u.fn)(1);
            m_pPed->Tasks->pdwJumpJetPack = 0;
        }
    }
}
#pragma optimize("y", on)
void CPlayerPed::SetMoney(int iAmount) {
    ScriptCommand(&set_actor_money, m_dwGTAId, 0);
    ScriptCommand(&set_actor_money, m_dwGTAId, iAmount);
}
#pragma optimize("", on)
void CPlayerPed::StartDancing(int iStyle)
{
    if (iStyle < 0 || iStyle > 3) return;

    m_iDanceState = 1;
    m_iDanceStyle = iStyle;

    if (m_bytePlayerNumber == 0) {
        ApplyAnimation(DanceIdleLoops[m_iDanceStyle], DanceStyleLibs[m_iDanceStyle], 16.0f, 1, 0, 0, 0, -1);
    }
}

void CPlayerPed::StopDancing()
{
    m_iDanceState = 0;
    _MATRIX4X4 mat;
    GetMatrix(&mat);
    TeleportTo(mat.pos.X, mat.pos.Y, mat.pos.Z);
}

#pragma optimize("s", on)
#pragma optimize("y", off)
void CPlayerPed::ApplyCommandTask(char *szTaskName, int p1, int p2, int p3,
                                  _VECTOR *p4, int p5, float p6, int p7, int p8, int p9)
{
    volatile unsigned long dwPed = (unsigned long)m_pPed;
    if (!dwPed) return;

    typedef void (CCommandTaskManager::*Fn)(char*, unsigned long, int, int, int, _VECTOR*, int, int, int, int, int);
    Fn fn;
    *(void**)&fn = (void*)0x618970;
    (((CCommandTaskManager*)0xC15448)->*fn)(szTaskName, dwPed, p1, p2, p3, p4, p5, *(int*)&p6, p7, p8, p9);
}
#pragma optimize("", on)

void CPlayerPed::ToggleCellphone(int iOn)
{
    if (!m_pPed) return;
    m_iCellPhoneEnabled = iOn;
    ScriptCommand(&toggle_actor_cellphone, m_dwGTAId, iOn);
}

char* CPlayerPed::GetShopName() {
    if (!m_pPed) return 0;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return 0;
    return *(char**)((char*)m_pPed + 0x78c);
}

void CPlayerPed::SetAmmo(unsigned char byteWeapon, unsigned short wAmmo) {
    if (m_pPed) {
        _WEAPON_SLOT_TYPE* pSlot = FindWeaponSlot(byteWeapon);
        if (pSlot) {
            pSlot->dwAmmo = wAmmo;
        }
    }
}

void CMenuPool::ShowMenu(unsigned char byteMenuID) {
    if (byteMenuID >= 128) return;
    if (!m_bNotEmpty[byteMenuID] || !m_pObject[byteMenuID]) return;

    if (m_nCurrent != 128) {
        m_pObject[m_nCurrent]->Hide();
    }
    m_pObject[byteMenuID]->Show();
    m_nCurrent = byteMenuID;
    m_bCanceled = 0;
}

void CMenuPool::HideMenu(unsigned char byteMenuID) {
    if (byteMenuID >= 128 || m_nCurrent == 128) return;
    if (!m_bNotEmpty[byteMenuID] || !m_pObject[byteMenuID]) return;
    m_pObject[byteMenuID]->Hide();
    m_nCurrent = 128;
}

CGangZonePool::~CGangZonePool() {
    int i = 1024;
    CGangZone** pZone = m_pGangZone;
    do {
        if (*pZone) {
            delete *pZone;
            *pZone = 0;
        }
        pZone++;
        --i;
    } while (i != 0);
}

CPlayerTags::~CPlayerTags() {
    if (m_pTexture) m_pTexture->Release();
    if (m_pVB) m_pVB->Release();
    if (m_pOldStates) m_pOldStates->Release();
    if (m_pNewStates) m_pNewStates->Release();
}

void CPlayerTags::DeleteDeviceObjects() {
    if (m_pVB) {
        m_pVB->Release();
        m_pVB = 0;
    }
    if (m_pOldStates) {
        m_pOldStates->Release();
        m_pOldStates = 0;
    }
    if (m_pNewStates) {
        m_pNewStates->Release();
        m_pNewStates = 0;
    }
}

#pragma optimize("", on)

void CPlayerTags::End() {
    if (!m_pOldStates) {
        RestoreDeviceObjects();
        if (!m_pOldStates)
            return;
    }
    m_pOldStates->Apply();
}

int R5LabelRemotePlayerView::IsActive() {
    if (ped && state) return 1;
    return 0;
}

#pragma optimize("", on)

void LocalPlayerRPCResetView::SetStatePair(int first, int second) {
    field_2DA = first;
    field_2DE = second;
}

void RPCVehiclePoolView::LinkToInterior(unsigned short id, int interior) {
    if (m_bVehicleSlotState[id]) {
        m_pVehicles[id]->LinkToInterior(interior);
    }
}

// Matched ?GetLabelPool@CNetGame@@QAEPAVCLabelPool@@XZ (0x1000e550)
CLabelPool* CNetGame::GetLabelPool() { return m_pPools->m_pLabel; }

// Matched ?GetTextDrawPool@CNetGame@@QAEPAVCTextDrawPool@@XZ (0x10017cf0)
CTextDrawPool* CNetGame::GetTextDrawPool() { return m_pPools->m_pTextDraw; }

// Matched ?GetMenuPool@CNetGame@@QAEPAVCMenuPool@@XZ (0x100084f0)
CMenuPool* CNetGame::GetMenuPool() { return m_pPools->m_pMenu; }

// Matched ??0CGangZonePool@@QAE@XZ (0x10002120)
CGangZonePool::CGangZonePool() {
    int i = 1024;
    CGangZone** pZone = m_pGangZone;
    int* pState = m_bSlotState;
    do {
        *pZone = 0;
        *pState = 0;
        pZone++;
        pState++;
        --i;
    } while (i != 0);
}

// Matched ?Flash@CGangZonePool@@QAEXGK@Z (0x10002200)
void CGangZonePool::Flash(unsigned short id, unsigned long color) {
    if (m_pGangZone[id]) {
        m_pGangZone[id]->flashColor = color;
    }
}

// Matched ??0CTextDrawPool@@QAE@XZ (0x10001190)
// Clears all 2048 text-draw items and their game-object pointers.
CTextDrawPool::CTextDrawPool() {
    int i = 2048;
    CTextDrawItem* pItem = m_items;
    void** pObj = m_pObjects;
    do {
        memset(pItem, 0, sizeof(CTextDrawItem));
        *pObj = 0;
        pObj++;
        pItem++;
        --i;
    } while (i != 0);
}

// Matched ??0CMenuPool@@QAE@XZ (0x10007e60)
CMenuPool::CMenuPool() {
    int i = 128;
    CMenu** pMenu = m_pObject;
    int* pState = m_bNotEmpty;
    do {
        *pState = 0;
        *pMenu = 0;
        pMenu++;
        pState++;
        --i;
    } while (i != 0);
    m_nCurrent = 0x80;
}

// Matched ?StopFlash@CGangZonePool@@QAEXG@Z (0x10002220)
void CGangZonePool::StopFlash(unsigned short id) {
    if (m_pGangZone[id]) {
        m_pGangZone[id]->flashColor = m_pGangZone[id]->color;
    }
}



// Matched ?GetHealth@CPlayerPed@@QAEMXZ (0x100abd50)
float CPlayerPed::GetHealth()
{
	if (!m_pPed)
	{
		return 0.0f;
	}

	return m_pPed->fHealth;
}

// Matched ?GetArmour@CPlayerPed@@QAEMXZ (0x100abd90)
float CPlayerPed::GetArmour()
{
	if (!m_pPed)
	{
		return 0.0f;
	}

	return m_pPed->fArmour;
}

// Matched ?IsDead@CPlayerPed@@QAEHXZ (0x100abe10)
int CPlayerPed::IsDead()
{
	if (m_pPed && m_pPed->fHealth > 0.0f)
	{
		return 0;
	}

	return 1;
}
