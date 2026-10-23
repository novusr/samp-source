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

struct _WEAPON_SLOT_TYPE {
    unsigned long dwType;
    unsigned long dwAmmo;
    unsigned long dwAmmoInClip;
    char gap[0x10];
};

struct _VEHICLE_TYPE {
    char gap0[0x22];
    unsigned short nModelIndex;     // 0x22
    char gap1[0x43c];               // 0x24 to 0x460
    void* pDriver;                  // 0x460
};
struct _ENTITY_TYPE;

struct PED_TYPE {
    char gap0[0x46c];
    unsigned long dwStateFlags;     // 0x46c
    char gap_goggles[0x90];         // 0x470 to 0x500
    void* pGoggles;                 // 0x500
    char gap1[0x2c];                // 0x504 to 0x530
    unsigned long dwActionTrigger;  // 0x530
    char gap2[0xc];                 // 0x534 to 0x540
    float fHealth;                  // 0x540
    char gap3[0x4];
    float fArmour;                  // 0x548
    char gap4[0x1c];
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

class CPlayerPed {
public:
    char gap[0x48];
    int m_iCellPhoneEnabled;        // 0x48
    int m_bObjectSlotUsed[10];      // 0x4c
    char gap_ped[0x230];            // 0x74 to 0x2a4
    PED_TYPE *m_pPed;               // 0x2a4
    char gap_dance[0x2a];           // 0x2a8 to 0x2d2
    int m_iDanceState;              // 0x2d2
    char gap_thermal[0x10];         // 0x2d6 to 0x2e6
    int m_bHasThermalGoggles;       // 0x2e6
    char gap_piss[8];               // 0x2ea to 0x2f2
    int m_iPissingState;            // 0x2f2

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
};

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

int CPlayerPed::IsAPassenger() {
    PED_TYPE* pPed = m_pPed;
    _VEHICLE_TYPE* pVehicle = pPed->pVehicle;
    if (pVehicle && (pPed->dwStateFlags & 0x100)) {
        if (pVehicle->pDriver != pPed || pVehicle->nModelIndex == 0x23a || pVehicle->nModelIndex == 0x239)
            return 1;
    }
    return 0;
}

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

#pragma pack(pop)

