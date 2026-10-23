
#include "../main.h"
#include "game/util.h"

//----------------------------------------------------

// R5: 0x10001120
CVehicle *CVehiclePool::GetAt(VEHICLEID id)
{
	if (id >= MAX_VEHICLES || !m_bVehicleSlotState[id]) return NULL;
	return m_pVehicles[id];
}

//----------------------------------------------------

// R5: 0x10001150
BOOL CVehiclePool::GetSlotState(VEHICLEID id)
{
	if (id >= MAX_VEHICLES) return FALSE;
	return m_bVehicleSlotState[id];
}

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

// R5: 0x1001EB90
VEHICLEID CVehiclePool::FindIDFromGtaPtr(VEHICLE_TYPE *pGtaVehicle)
{
	for(int i = 1; i <= field_0; i++) {
		if(pGtaVehicle == m_pGTAVehicles[i]) return (VEHICLEID)i;
	}
	return 0xFFFF;
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

