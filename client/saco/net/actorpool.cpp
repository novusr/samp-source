
#include "../main.h"

//----------------------------------------------------

// R5: 0x100016C0
CActorPool::CActorPool()
{
	for(ACTORID ActorID = 0; ActorID < MAX_ACTORS; ActorID++) {
		m_bSlotState[ActorID] = 0;
		m_pActors[ActorID] = 0;
		m_iGtaActorID[ActorID] = 0;
	}
	field_0 = 0;
}

//----------------------------------------------------

// Ordinary C++ reconstruction. Original full bodies are pinned in checkpoint32.
// R5: 0x10001660
void CActorPool::UpdateCount() {
 int last = 0;
 for(int i=0;i<MAX_ACTORS;++i) if(m_bSlotState[i]) last=i;
 field_0=last;
}
// R5: 0x100016F0
BOOL CActorPool::Delete(ACTORID id) {
 if(id<MAX_ACTORS && m_bSlotState[id] && m_pActors[id]) {
  m_bSlotState[id]=FALSE;
  delete m_pActors[id];
  m_pActors[id]=NULL;
  m_iGtaActorID[id]=0;
  UpdateCount();
  return TRUE;
 }
 return FALSE;
}
// R5: 0x100018B0
ACTORID CActorPool::FindIDFromGtaPtr(int gtaPtr) {
 int i=0;
 if(field_0>=0) {
  do {if(gtaPtr==m_iGtaActorID[i]) return (ACTORID)i;++i;} while(i<=field_0);
 }
 return (ACTORID)0xFFFF;
}
// R5: 0x100018E0
void CActorPool::DeleteAll() {
 int i=0;
 do {Delete((ACTORID)i);++i;} while((ACTORID)i<MAX_ACTORS);
}

extern CChatWindow *pChatWindow;

// R5: 0x10001900
BOOL CActorPool::New(ACTOR_SPAWN_INFO *info)
{
    if(m_pActors[info->id]) {
        pChatWindow->AddDebugMessage("Warning: actor %u was not deleted", info->id);
        Delete(info->id);
    }
    m_pActors[info->id] = new CActorPed(info->skin, info->x, info->y, info->z, info->rotation);
    if(m_pActors[info->id]) {
        m_iGtaActorID[info->id] = (int)m_pActors[info->id]->m_pPed;
        m_bSlotState[info->id] = TRUE;
        m_pActors[info->id]->SetHealth(info->health);
        if(info->immune) m_pActors[info->id]->ToggleImmunity(1);
        else m_pActors[info->id]->ToggleImmunity(0);
        field_2EE4[info->id] = TRUE;
        field_3E84[info->id] = 0;
        UpdateCount();
        return TRUE;
    }
    return FALSE;
}
