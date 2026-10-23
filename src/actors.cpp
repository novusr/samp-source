// R5 layouts and loop bodies checked against frozen machine code.
// Semantic leads: samp-reversed/saco/net/actorpool.cpp; see provenance.json.
#pragma pack(push, 1)
class ActorPool {
public:
    int lastActive;
    void* actors[1000];
    int slots[1000];
    int gtaIds[1000];
    ActorPool();
    void UpdateCount();
    unsigned short FindIDFromGtaPtr(int gtaId);
    int Delete(unsigned short id); // external R5 dependency, not implemented
    void DeleteAll();
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

void ActorPool::UpdateCount() {
    int last = 0;
    for (int i = 0; i < 1000; ++i) {
        if (slots[i]) last = i;
    }
    lastActive = last;
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

struct _ACTOR_PED_TYPE {
    char gap0[0x46c];
    unsigned long dwStateFlags;    // 0x46c
    char gap1[0xc0];               // 0x470 to 0x530
    unsigned long dwActionTrigger; // 0x530
    char gap2[0x14];               // 0x534 to 0x548
    float fArmour;                 // 0x548
};

class CActorPed {
public:
    char gap[0x48];
    _ACTOR_PED_TYPE* m_pPed;       // 0x48

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

