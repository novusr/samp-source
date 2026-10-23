
#include "../main.h"

extern CGame *pGame;

// R5: 0x100127A0
void CObjectPool::UpdateLargestID()
{
	int last = 0;
	for (int i = 0; i < MAX_OBJECTS; i++) {
		if (m_bObjectSlotState[i]) last = i;
	}
	m_iLastObjectId = last;
}

// R5: 0x10012800
CObjectPool::CObjectPool()
{
	for(WORD wObjectID = 0; wObjectID < MAX_OBJECTS; wObjectID++)
	{
		m_bObjectSlotState[wObjectID] = 0;
		m_pObjects[wObjectID] = 0;
	}
	m_iLastObjectId = 0;
};

// R5: 0x10012850
BOOL CObjectPool::Delete(WORD id)
{
	if (id >= MAX_OBJECTS || !m_bObjectSlotState[id] || !m_pObjects[id]) return FALSE;
	CCamera *camera = pGame->GetCamera();
	if (camera && camera->field_0 == (CEntity *)m_pObjects[id]) camera->FUNC_1009D660(NULL);
	m_bObjectSlotState[id] = FALSE;
	delete m_pObjects[id];
	m_pObjects[id] = NULL;
	UpdateLargestID();
	return TRUE;
}

// Transferred from michael-fa0.2.5; R5 uses its inclusive last-object ID
// and invalid ID65535. CObject remains incomplete; only its verified Entity
// base field at0x40 is accessed. No CObject implementation is counted.

// R5: 0x100129D0
CObject* CObjectPool::GetObjFromGtaPtr(ENTITY_TYPE * pGtaObject)
{
	int x=1;

	while(x <= m_iLastObjectId) {
		if (m_pObjects[x])
		{
			if(pGtaObject == reinterpret_cast<CEntity *>(m_pObjects[x])->m_pEntity) return m_pObjects[x];
		}
		x++;
	}

	return NULL;
}

// R5: 0x10012A10
int CObjectPool::FindIDFromGtaPtr(ENTITY_TYPE * pGtaObject)
{
	int x=1;

	while(x <= m_iLastObjectId) {
		if (m_pObjects[x])
		{
			if(pGtaObject == reinterpret_cast<CEntity *>(m_pObjects[x])->m_pEntity) return x;
		}
		x++;
	}

	return 0xFFFF;
}

typedef char complete_object_pool_size[(sizeof(CObjectPool)==0x1F44)?1:-1];
