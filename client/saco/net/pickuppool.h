
#pragma once

#define MAX_PICKUPS 4096

//----------------------------------------------------

struct R5_DROPPED_WEAPON { bool bDroppedWeapon; WORD fromPlayer; };
typedef char dropped_weapon_size[(sizeof(R5_DROPPED_WEAPON)==3)?1:-1];

struct R5PickupRecord { int iModel; int iType; float fX; float fY; float fZ; };
typedef char pickup_record_size[(sizeof(R5PickupRecord)==20)?1:-1];

class CPickupPool
{
private:

	int m_iPickupCount;
	DWORD	m_dwHnd[MAX_PICKUPS];
	int m_iNetworkIds[MAX_PICKUPS];
	int m_iTimer[MAX_PICKUPS];

	R5_DROPPED_WEAPON m_droppedWeapon[MAX_PICKUPS];

	R5PickupRecord m_Pickups[MAX_PICKUPS];

public:

	CPickupPool() {
		memset(m_Pickups, 0, sizeof(m_Pickups));
		m_iPickupCount = 0;
		for (int i = 0; i < MAX_PICKUPS; i++)
		{
			m_dwHnd[i] = NULL;
			m_iTimer[i] = 0;
			m_iNetworkIds[i] = -1;
		}
	}

	~CPickupPool();
	void Destroy(int iPickup);
	void DestroyDropped(WORD fromPlayer);
	int FindNetworkId(int networkId);
	void PickedUp(int networkId);
	void Process();
	void New(int iModel, float fX, float fY, float fZ, DWORD dwAmmo, WORD fromDeadPlayer);

};

//----------------------------------------------------
