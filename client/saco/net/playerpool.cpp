
#include "../main.h"

//----------------------------------------------------

// R5: 0x10013FD0
CPlayerPool::CPlayerPool()
{
	// loop through and initialize all net players to null and slot states to false
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) {
		if (playerId <= MAX_PLAYERS) m_pPlayers[playerId] = NULL;
		field_2A[playerId] = 0;
	}

	m_pLocalPlayer = new CLocalPlayer();

	field_0 = 0;
	field_22 = 0;
	m_LocalPlayerID = 0;
	field_2F3A = 0;
}

//----------------------------------------------------

// R5: 0x100010B0
BOOL CPlayerPool::GetSlotState(PLAYERID playerId)
{
	if (playerId >= MAX_PLAYERS) return FALSE;
	return field_2A[playerId];
}

//----------------------------------------------------

// R5: 0x100010F0
CRemotePlayer* CPlayerPool::GetAt(PLAYERID playerId)
{
	if (playerId > MAX_PLAYERS) { return NULL; }
	CNetPlayer *pNetPlayer = m_pPlayers[playerId];
	if (pNetPlayer) return pNetPlayer->m_pRemotePlayer;
	return NULL;
}

//----------------------------------------------------

// R5: 0x10001A40
CLocalPlayer* CPlayerPool::GetLocalPlayer()
{
	return m_pLocalPlayer;
}

//----------------------------------------------------

// R5: 0x10013750
void CPlayerPool::UpdateLargestId()
{
	int largest = 0;
	for (int id = 0; id < MAX_PLAYERS; ++id) {
		if (field_2A[id]) largest = id;
	}
	field_2F3A = largest;
}

//----------------------------------------------------

// R5: 0x10014250
BOOL CPlayerPool::New(PLAYERID id, PCHAR name, BOOL isNPC)
{
	CNetPlayer *player = new CNetPlayer(name, isNPC);
	if (id >= MAX_PLAYERS) return FALSE;
	m_pPlayers[id] = player;
	if (player) {
		if (player->m_pRemotePlayer) player->m_pRemotePlayer->SetPlayerID(id);
		if (isNPC && GetAt(id)) GetAt(id)->SetNPC(TRUE);
		field_2A[id] = 1;
		UpdateLargestId();
		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------

// R5: 0x100175C0
PCHAR CPlayerPool::GetPlayerName(PLAYERID playerId)
{
	if (playerId == m_LocalPlayerID) return (PCHAR)field_6.c_str();
	if (playerId > MAX_PLAYERS) return NULL;
	CNetPlayer *player = m_pPlayers[playerId];
	if (!player) return NULL;
	return (PCHAR)player->m_PlayerName.c_str();
}

//----------------------------------------------------

