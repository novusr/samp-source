
#pragma once

//----------------------------------------------------

class CVehiclePool
{
public:
	int field_0;

	char _gap4[4000];

	char field_FA4[400];
	CVehicle *m_pVehicles[MAX_VEHICLES];
	BOOL m_bVehicleSlotState[MAX_VEHICLES];
	VEHICLE_TYPE *m_pGTAVehicles[MAX_VEHICLES];

	char _gap6EF4[12000];

	DWORD field_9DD4[MAX_VEHICLES];

	char _gapBD14[48000];

	int field_17894;

	CVehiclePool();

	CVehicle *GetAt(VEHICLEID id);
	BOOL GetSlotState(VEHICLEID id);
	int FindGtaIDFromID(int id);
	VEHICLEID FindIDFromGtaPtr(VEHICLE_TYPE *pGtaVehicle);
};

//----------------------------------------------------
