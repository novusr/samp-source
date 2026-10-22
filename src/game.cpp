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
    char gap0[0x45c];
    unsigned short wAlarmState; // 0x45c
    char gap1[0x62];
    float fHealth;              // 0x4c0
    char gap2[0x3a8];
    unsigned long dwHydraThrusters; // 0x86c
    char gap3[0xdc];
    float fTankRotX;            // 0x94c
    float fTankRotY;            // 0x950
};

class CVehicle {
public:
    char gap[0x4c];
    VEHICLE_TYPE* m_pVehicle;

    float GetTankRotX();
    float GetTankRotY();
    unsigned long GetHydraThrusters();
    void SetHydraThrusters(unsigned long dwDirection);
    void SetHealth(float fHealth);
    void SetAlarmState(unsigned short wState);
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

#pragma pack(pop)
