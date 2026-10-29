// R5 layouts and loop bodies checked against frozen machine code.
// Semantic leads: samp-reversed/saco/net/actorpool.cpp; see provenance.json.
#pragma pack(push, 1)
class CActorPed;

class ActorPool {
public:
    int lastActive;
    CActorPed* actors[1000];
    int slots[1000];
    int gtaIds[1000];
    ActorPool();
    void UpdateCount();
    unsigned short FindIDFromGtaPtr(int gtaId);
    int Delete(unsigned short id); // external R5 dependency, not implemented
    void DeleteAll();
};

class CActorPool : public ActorPool {
public:
    int Delete(unsigned short id);
};
#pragma pack(pop)

ActorPool::ActorPool() {
    int i = 1000;
    unsigned long* p = (unsigned long*)this;
    do {
        p[0x3e9] = 0;
        p[1] = 0;
        p[0x7d1] = 0;
        --i;
        ++p;
    } while (i != 0);
    lastActive = 0;
}

// Recomputes lastActive as the highest occupied actor slot (0 when none).
void ActorPool::UpdateCount() {
    int lastUsed = 0;
    for (int id = 0; id < 1000; ++id) {
        if (slots[id]) lastUsed = id;
    }
    lastActive = lastUsed;
}

unsigned short ActorPool::FindIDFromGtaPtr(int gtaId) {
    int i = 0;
    if (lastActive >= 0) {
        do {
            if (gtaId == gtaIds[i]) return (unsigned short)i;
            ++i;
        } while (i <= lastActive);
    }
    return 0xffff;
}

void ActorPool::DeleteAll() {
    int i = 0;
    do {
        Delete((unsigned short)i);
        ++i;
    } while ((unsigned short)i < 1000);
}

struct _PED_TYPE;
_PED_TYPE* __stdcall GamePool_Ped_GetAt(int nPedIndex);
float DegToRad(float fDegrees);

struct SCRIPT_COMMAND {
    unsigned short OpCode;
    char Params[16];
};
int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);
const SCRIPT_COMMAND set_actor_immunities = { 0x02ab, "iiiiii" };
const SCRIPT_COMMAND kill_actor = { 0x0321, "i" };

class CGtaEntity {
public:
    virtual void Destroy(int b);
};

struct _ACTOR_PED_TYPE {
    char gap0[0x46c];
    unsigned long dwStateFlags;    // 0x46c
    char gap1[0xc0];               // 0x470 to 0x530
    unsigned long dwActionTrigger; // 0x530
    char gap2[0xc];                // 0x534 to 0x540
    float fHealth;                 // 0x540
    char gap3[4];                  // 0x544
    float fArmour;                 // 0x548
    char gap4[0x10];               // 0x54c to 0x55c
    float fRotation2;              // 0x55c
};

class CEntity {
public:
    virtual ~CEntity() {}
    char gap[0x3c];
    void* m_pEntity;              // 0x40
    unsigned long m_dwGTAId;      // 0x44
};

class CActorPed : public CEntity {
public:
    _ACTOR_PED_TYPE* m_pPed;      // 0x48
    unsigned long m_dwMarkerID;   // 0x4c
    unsigned long m_dwArrow;      // 0x50
    unsigned char m_byteCreateMarker; // 0x54
    unsigned char m_byteImmune;   // 0x55

    virtual ~CActorPed();
    void Destroy();
    void SetTargetRotation(float fRotation);
    void SetHealth(float fHealth);
    void ToggleImmunity(unsigned char byteEnable);
    unsigned char GetActionTrigger();
    unsigned long GetStateFlags();
    void SetArmour(float fArmour);
    void SetStateFlags(unsigned long dwState);
    void SetActionTrigger(unsigned char byteTrigger);
    int IsInVehicle();
};

unsigned char CActorPed::GetActionTrigger() {
    return (unsigned char)m_pPed->dwActionTrigger;
}

unsigned long CActorPed::GetStateFlags() {
    if (!m_pPed) return 0;
    return m_pPed->dwStateFlags;
}

void CActorPed::SetArmour(float fArmour) {
    if (!m_pPed) return;
    m_pPed->fArmour = fArmour;
}

void CActorPed::SetStateFlags(unsigned long dwState) {
    if (!m_pPed) return;
    m_pPed->dwStateFlags = dwState;
}

void CActorPed::SetActionTrigger(unsigned char byteTrigger) {
    if (!m_pPed) return;
    m_pPed->dwActionTrigger = byteTrigger;
}

int CActorPed::IsInVehicle() {
    if (!m_pPed) return 0;
    return (m_pPed->dwStateFlags >> 8) & 1;
}

void CActorPed::SetTargetRotation(float fRotation) {
    if (!m_pPed) return;
    if (!GamePool_Ped_GetAt(m_dwGTAId)) return;
    m_pPed->fRotation2 = DegToRad(fRotation);
}

void CActorPed::SetHealth(float fHealth) {
    if (!m_pPed) return;
    m_pPed->fHealth = fHealth;
    if (m_pPed->fHealth <= 0.0f) {
        ScriptCommand(&kill_actor, m_dwGTAId);
    }
}

CActorPed::~CActorPed() {
    Destroy();
}

// Turns the actor's scripted immunity on or off; the flag is cached on the
// wrapper and the six immunity sub-flags are pushed to the game script.
void CActorPed::ToggleImmunity(unsigned char byteEnable) {
    if (!m_pPed) return;
    m_byteImmune = byteEnable;
    if (byteEnable)
        ScriptCommand(&set_actor_immunities, m_dwGTAId, 1, 1, 1, 1, 1);
    else
        ScriptCommand(&set_actor_immunities, m_dwGTAId, 0, 0, 0, 1, 0);
}

#pragma optimize("y", off)
void CActorPed::Destroy() {
    volatile unsigned int dwPed = (unsigned int)m_pPed;
    if (!m_pPed || !GamePool_Ped_GetAt(m_dwGTAId) || *(unsigned long*)m_pPed == 0x863C40) {
        m_pPed = 0;
        m_pEntity = 0;
        m_dwGTAId = 0;
        return;
    }
    ((CGtaEntity*)(void*)dwPed)->Destroy(1);
    m_pEntity = 0;
    m_pPed = 0;
}
#pragma optimize("", on)
// Frees the actor in slot `id`: clears its slot flag, deletes the CActorPed,
// clears both its pointer and its GTA id, then recomputes the last active slot.
int CActorPool::Delete(unsigned short id) {
    if (id < 1000 && slots[id] && actors[id]) {
        slots[id] = 0;
        delete actors[id];
        actors[id] = 0;
        gtaIds[id] = 0;
        UpdateCount();
        return 1;
    }
    return 0;
}
