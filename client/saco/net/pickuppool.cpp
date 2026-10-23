
#include "../main.h"

extern CGame *pGame;
extern CNetGame *pNetGame;

const SCRIPT_COMMAND is_pickup_picked_up = { 0x0214, "i" };

//----------------------------------------------------

// R5: 0x10013180
void CPickupPool::New(int iModel, float fX, float fY, float fZ, DWORD dwAmmo, WORD fromDeadPlayer)
{
	if (m_iPickupCount >= MAX_PICKUPS) return;
	int iPickup;
	for (iPickup = 0; iPickup < MAX_PICKUPS; iPickup++) {
		if (m_dwHnd[iPickup] == NULL) break;
	}
	if (iPickup == MAX_PICKUPS) return;

	m_Pickups[iPickup].iModel = iModel;
	m_Pickups[iPickup].iType = 4;
	m_Pickups[iPickup].fX = fX;
	m_Pickups[iPickup].fY = fY;
	m_Pickups[iPickup].fZ = fZ;
	m_dwHnd[iPickup] = pGame->CreateWeaponPickup(iModel, dwAmmo, fX, fY, fZ);
	m_iTimer[iPickup] = NULL;
	m_iNetworkIds[iPickup] = -1;
	m_droppedWeapon[iPickup].bDroppedWeapon = true;
	m_droppedWeapon[iPickup].fromPlayer = fromDeadPlayer;
	m_iPickupCount++;
}

//----------------------------------------------------

// Transferred from michael-fa0.2.5 using the verified R5 layout and4096
// slots. Destroy additionally resets the R5 network-ID slot to-1.

// R5: 0x10013320
void CPickupPool::Destroy(int iPickup)
{
	if (m_iPickupCount <= 0 || iPickup < 0 || iPickup >= MAX_PICKUPS) return;
	if (m_dwHnd[iPickup] != NULL)
	{
		ScriptCommand(&destroy_pickup, m_dwHnd[iPickup]);
		m_dwHnd[iPickup] = NULL;
		m_iTimer[iPickup] = NULL;
		m_iNetworkIds[iPickup] = -1;
		m_iPickupCount--;
	}
}

// R5: 0x10013380
void CPickupPool::DestroyDropped(WORD fromPlayer)
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwHnd[i] != NULL && m_droppedWeapon[i].bDroppedWeapon && m_droppedWeapon[i].fromPlayer == fromPlayer)
		{
			ScriptCommand(&destroy_pickup, m_dwHnd[i]);
			m_dwHnd[i] = NULL;
			m_iTimer[i] = NULL;
			m_iPickupCount--;
		}
	}
}

//----------------------------------------------------

// R5: 0x100133E0
int CPickupPool::FindNetworkId(int networkId)
{
	for (int i = 0; i != MAX_PICKUPS; i++) {
		if (m_iNetworkIds[i] == networkId) return i;
	}
	return -1;
}

//----------------------------------------------------

// R5: 0x10013410
CPickupPool::~CPickupPool()
{
	for(int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwHnd[i] != NULL)
		{
			ScriptCommand(&destroy_pickup, m_dwHnd[i]);
		}
	}
}

//----------------------------------------------------

// R5: 0x10013440
void CPickupPool::PickedUp(int networkId)
{
	int iPickup = FindNetworkId(networkId);
	if (iPickup == -1) return;
	if (m_dwHnd[iPickup] != NULL && m_iTimer[iPickup] == 0) {
		if (m_droppedWeapon[iPickup].bDroppedWeapon) return;
		RakNet::BitStream bsPickup;
		bsPickup.Write(iPickup);
		pNetGame->GetRakClient()->RPC(RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE);
		m_iTimer[iPickup] = 15;
	}
}

//----------------------------------------------------

// R5: 0x10013520
void CPickupPool::Process()
{
	for (int i = 0; i < MAX_PICKUPS; i++) {
		if (m_dwHnd[i] != NULL) {
			if (m_droppedWeapon[i].bDroppedWeapon || m_Pickups[i].iType == 14) {
				if (ScriptCommand(&is_pickup_picked_up, m_dwHnd[i])) {
					RakNet::BitStream bsPickup;
					if (m_droppedWeapon[i].bDroppedWeapon) {
						bsPickup.Write(m_droppedWeapon[i].fromPlayer);
						pNetGame->GetRakClient()->RPC(RPC_PickedUpWeapon, &bsPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, FALSE);
					} else {
						bsPickup.Write(i);
						pNetGame->GetRakClient()->RPC(RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, FALSE);
					}
				}
			} else if (m_iTimer[i] > 0) {
				m_iTimer[i]--;
			}
		}
	}
}

typedef char complete_pickup_pool_size[(sizeof(CPickupPool)==0x23004)?1:-1];
