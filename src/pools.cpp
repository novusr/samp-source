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
int LocalPlayer::AreKeysChanged(unsigned short a, unsigned short b, unsigned short c) {
    if (a == key1 && b == key2 && c == key3) return 0;
    return 1;
}

struct _VEHICLE_TYPE;
struct _ENTITY_TYPE;

struct PED_TYPE {
    char gap0[0x46c];
    unsigned long dwStateFlags; // 0x46c
    char gap1[0xc0];
    unsigned char byteAction;   // 0x530
    char gap2[0xf];
    float fHealth;              // 0x540
    char gap3[0x4];
    float fArmour;              // 0x548
    char gap4[0x1c];
    _VEHICLE_TYPE* pContactVehicle; // 0x568
    char gap5[0x18];
    _ENTITY_TYPE* pContactEntity;   // 0x584
    char gap6[0x4];
    _VEHICLE_TYPE* pVehicle;        // 0x58c
    char gap7[0x19d];
    unsigned char byteFightingStyle;// 0x72d
};

class CPlayerPed {
public:
    char gap[0x48];
    int m_iCellPhoneEnabled;
    char gap_ped[0x258];
    PED_TYPE *m_pPed;
    char gap_dance[0x2a];
    int m_iDanceState;
    char gap_piss[0x1c];
    int m_iPissingState;

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
};

int CPlayerPed::IsCellphoneEnabled() {
    return m_iCellPhoneEnabled;
}
int CPlayerPed::GetPissingState() {
    return m_iPissingState;
}

unsigned char CPlayerPed::GetActionTrigger() {
    return m_pPed->byteAction;
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

#pragma pack(pop)
