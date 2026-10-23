
#include "../main.h"

//----------------------------------------------------

// R5: 0x100141B0
CNetPlayer::CNetPlayer(PCHAR szPlayerName, BOOL bIsNPC)
{
	m_bIsNPC = bIsNPC;
	field_C = 0;
	field_4 = 0;
	m_PlayerName = szPlayerName;
	m_pRemotePlayer = new CRemotePlayer();
}

//----------------------------------------------------

// R5: 0x10013F60
CNetPlayer::~CNetPlayer()
{
	SAFE_DELETE(m_pRemotePlayer);
}

//----------------------------------------------------
