
#include "../main.h"

// R5: 0x10001190
CLabelPool::CLabelPool()
{
	for(WORD wLabelID = 0; wLabelID < MAX_LABELS; wLabelID++)
	{
		memset(&m_Labels[wLabelID], 0, sizeof(LABEL));
		m_bLabelSlotState[wLabelID] = FALSE;
	}
}

// R5: 0x100015E0
CLabelPool::~CLabelPool()
{
	for(WORD wLabelID = 0; wLabelID < MAX_LABELS; wLabelID++)
	{
		if(m_bLabelSlotState[wLabelID])
		{
			Delete(wLabelID);
		}
	}
}

// R5: 0x100012E0
BOOL CLabelPool::Delete(WORD wLabelID)
{
	if(wLabelID >= MAX_LABELS) return FALSE;
	if(!m_bLabelSlotState[wLabelID]) return FALSE;

	if(m_Labels[wLabelID].szText)
		free(m_Labels[wLabelID].szText);
	memset(&m_Labels[wLabelID], 0, sizeof(LABEL));
	m_bLabelSlotState[wLabelID] = FALSE;

	return TRUE;
}
