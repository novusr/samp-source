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

#pragma pack(pop)

