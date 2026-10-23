
#include "../main.h"
#include "game/util.h"

//----------------------------------------------------

// R5: 0x1001EA10
CVehiclePool::CVehiclePool()
{
	// loop through and initialize all vehicle properties to 0
	for(VEHICLEID VehicleID = 0; VehicleID < MAX_VEHICLES; VehicleID++) {
		m_bVehicleSlotState[VehicleID] = 0;
		m_pVehicles[VehicleID] = 0;
		m_pGTAVehicles[VehicleID] = 0;
		field_9DD4[VehicleID] = GetTickCount();
	}
	memset(field_FA4, 0, sizeof(field_FA4));
	field_0 = 0;
	field_17894 = 1;
}

//----------------------------------------------------

// R5: 0x1001EBC0
int CVehiclePool::FindGtaIDFromID(int id)
{
	if (id < MAX_VEHICLES && m_pGTAVehicles[id]) {
		return GamePool_Vehicle_GetIndex(m_pGTAVehicles[id]);
	}
	return 0xFFFF;
}

//----------------------------------------------------
