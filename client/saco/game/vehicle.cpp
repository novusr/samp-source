
#include "../main.h"
#include "util.h"

extern CGame *pGame;
extern CChatWindow *pChatWindow;
extern CNetGame *pNetGame;

BOOL r5AllowVehicleCreation = FALSE;
DWORD r5LastCreatedVehicleId = 0;
VEHICLE_TYPE *r5LastCreatedVehicle = 0;
BOOL r5TrailerDetachInProgress = FALSE;

const SCRIPT_COMMAND r5CreateVehicle = { 0x00a5, "ifffv" };
const SCRIPT_COMMAND r5CreateTrain = { 0x06e7, "ifffiv" };
const SCRIPT_COMMAND r5DestroyVehicle = { 0x01c3, "i" };
const SCRIPT_COMMAND r5DestroyTrain = { 0x07bd, "i" };
const SCRIPT_COMMAND r5VehicleGasTankExplosion = { 0x09b3, "ii" };
const SCRIPT_COMMAND r5VehicleHydraulics = { 0x098d, "ii" };
const SCRIPT_COMMAND r5VehicleTiresVulnerable = { 0x068b, "ii" };
const SCRIPT_COMMAND r5PutTrailerOnCab = { 0x0893, "ii" };
const SCRIPT_COMMAND r5DetachTrailerFromCab = { 0x07ac, "ii" };
const SCRIPT_COMMAND rpc_set_car_z_angle = { 0x0175, "if" };

// R5: 0x100B42B0
void __stdcall GamePrepareTrain(VEHICLE_TYPE *vehicle)
{
	if(!vehicle) return;
	PED_TYPE *driver = vehicle->pDriver;
	if(driver && driver->dwPedType != 0 && driver->dwPedType != 1) {
		DWORD nativeDriver = (DWORD)driver;
		_asm mov ecx, nativeDriver
		_asm mov ebx, [ecx]
		_asm push 1
		_asm call [ebx]
		vehicle->pDriver = 0;
	}
}

//-----------------------------------------------------------
// CONSTRUCTOR

// R5: 0x100B83D0
CVehicle::CVehicle(int iType, float fPosX, float fPosY,
				   float fPosZ, float fRotation, BOOL bKeepModelLoaded,
				   int a8)
{
	DWORD vehicleId = 0;
	m_pVehicle = 0;
	m_dwGTAId = 0;
	m_pTrailer = 0;
	if(iType != 538 && iType != 537 && iType != 570 && iType != 569 && iType != 449) {
		if(!pGame->IsModelLoaded(iType)) {
			pGame->RequestModel(iType, 2);
			pGame->LoadRequestedModels();
			while(!pGame->IsModelLoaded(iType)) Sleep(0);
		}
		if(a8) {
			r5AllowVehicleCreation = TRUE;
			*(DWORD*)((BYTE*)this + 0x8b) = TRUE;
		} else {
			*(DWORD*)((BYTE*)this + 0x8b) = FALSE;
		}
		ScriptCommand(&r5CreateVehicle, iType, fPosX, fPosY, fPosZ, &vehicleId);
		ScriptCommand(&rpc_set_car_z_angle, vehicleId, fRotation);
		ScriptCommand(&r5VehicleGasTankExplosion, vehicleId, 0);
		ScriptCommand(&r5VehicleHydraulics, vehicleId, 0);
		ScriptCommand(&r5VehicleTiresVulnerable, vehicleId, 0);
		m_pVehicle = GamePool_Vehicle_GetAt(vehicleId);
		m_pEntity = (ENTITY_TYPE*)m_pVehicle;
		m_dwGTAId = vehicleId;
		if(!m_pVehicle) {
			pChatWindow->AddDebugMessage("Warning: couldn't create vehicle type: %u", iType);
			return;
		}
		r5LastCreatedVehicleId = vehicleId;
		m_pVehicle->dwDoorsLocked = 0;
		*(DWORD*)((BYTE*)m_pVehicle + 0x4b0) = 0;
		*(DWORD*)((BYTE*)this + 0x5c) = FALSE;
		MATRIX4X4 matrix;
		GetMatrix(&matrix);
		matrix.pos.Z = (GetVehicleSubtype() != 2 && GetVehicleSubtype() != 6) ? (double)(fPosZ + 0.25f) : (double)fPosZ;
		matrix.pos.X = fPosX;
		matrix.pos.Y = fPosY;
		SetMatrix(matrix);
		*(DWORD*)((BYTE*)this + 0x87) = bKeepModelLoaded;
	} else if(iType == 538 || iType == 537 || iType == 449) {
		int model = iType;
		if(model == 538) model = 5;
		else if(model == 537) model = 3;
		else if(model == 449) model = 9;
		DWORD direction = 0;
		if(fRotation > 180.0f) direction = 1;
		pGame->RequestModel(538, 2);
		pGame->RequestModel(570, 2);
		pGame->RequestModel(537, 2);
		pGame->RequestModel(569, 2);
		pGame->RequestModel(449, 2);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(538)) Sleep(0);
		while(!pGame->IsModelLoaded(570)) Sleep(0);
		while(!pGame->IsModelLoaded(537)) Sleep(0);
		while(!pGame->IsModelLoaded(569)) Sleep(0);
		while(!pGame->IsModelLoaded(449)) Sleep(0);
		ScriptCommand(&r5CreateTrain, model, fPosX, fPosY, fPosZ, direction, &vehicleId);
		m_pVehicle = GamePool_Vehicle_GetAt(vehicleId);
		m_pEntity = (ENTITY_TYPE*)m_pVehicle;
		m_dwGTAId = vehicleId;
		r5LastCreatedVehicleId = vehicleId;
		r5LastCreatedVehicle = m_pVehicle;
		GamePrepareTrain(m_pVehicle);
	} else if(iType == 570 || iType == 569) {
		if(!r5LastCreatedVehicle) {
			m_pEntity = 0;
			m_pVehicle = 0;
			r5LastCreatedVehicle = 0;
			return;
		}
		m_pVehicle = *(VEHICLE_TYPE**)((BYTE*)r5LastCreatedVehicle + 0x5d4);
		if(!m_pVehicle) {
			pChatWindow->AddDebugMessage("Warning: bad train carriages");
			m_pEntity = 0;
			m_pVehicle = 0;
			r5LastCreatedVehicle = 0;
			return;
		}
		vehicleId = GamePool_Vehicle_GetIndex(m_pVehicle);
		m_pEntity = (ENTITY_TYPE*)m_pVehicle;
		m_dwGTAId = vehicleId;
		r5LastCreatedVehicleId = vehicleId;
		r5LastCreatedVehicle = m_pVehicle;
	}
	*(DWORD*)((BYTE*)this + 0x58) = FALSE;
	*(BYTE*)((BYTE*)this + 0x64) = 0;
	*(DWORD*)((BYTE*)this + 0x65) = FALSE;
	*(DWORD*)((BYTE*)this + 0xb4) = 0;
	*(DWORD*)((BYTE*)this + 0x6d) = FALSE;
	*(DWORD*)((BYTE*)this + 0x69) = GetTickCount();
	*(DWORD*)((BYTE*)this + 0x50) = 0xffffffff;
	*(DWORD*)((BYTE*)this + 0x54) = 0xffffffff;
	*(DWORD*)((BYTE*)this + 0x60) = FALSE;
	m_bHasNewColor = FALSE;
	field_71 = 0;
	*(DWORD*)((BYTE*)this + 0x75) = 0;
	*(DWORD*)((BYTE*)this + 0x8f) = 0;
	memset((BYTE*)this + 0x93, 0, 33);
}

//-----------------------------------------------------------

// R5: 0x100B7090
void CVehicle::LinkToInterior(int iInterior)
{
	if(GamePool_Vehicle_GetAt(m_dwGTAId)) {
		ScriptCommand(&link_vehicle_to_interior, m_dwGTAId, iInterior);
	}
}

//-----------------------------------------------------------
// If the game has internally destroyed the vehicle
// during this frame, the vehicle pointer should become 0

// R5: 0x100B70C0
void CVehicle::ResetPointers()
{
	if(!m_pVehicle) return;

	m_pVehicle = GamePool_Vehicle_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE *)m_pVehicle;
}

//-----------------------------------------------------------

// R5: 0x100B70E0
BOOL CVehicle::HasADriver()
{
	if(!m_pVehicle) return FALSE;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return FALSE;

	if(m_pVehicle) {
		if(m_pVehicle->pDriver && IN_VEHICLE(m_pVehicle->pDriver) && m_pVehicle->pDriver->dwPedType == 0)
			return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

// R5: 0x100B7130
BOOL CVehicle::IsOccupied()
{
	if(m_pVehicle) {
		if(m_pVehicle->pDriver) return TRUE;
		if(m_pVehicle->pPassengers[0]) return TRUE;
		if(m_pVehicle->pPassengers[1]) return TRUE;
		if(m_pVehicle->pPassengers[2]) return TRUE;
		if(m_pVehicle->pPassengers[3]) return TRUE;
		if(m_pVehicle->pPassengers[4]) return TRUE;
		if(m_pVehicle->pPassengers[5]) return TRUE;
		if(m_pVehicle->pPassengers[6]) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

// R5: 0x100B7230
void CVehicle::SetLockedState(int iLocked)
{
	if(!m_pVehicle) return;

	if(iLocked) {
		ScriptCommand(&lock_car,m_dwGTAId,1);
	} else {
		ScriptCommand(&lock_car,m_dwGTAId,0);
	}
}

//-----------------------------------------------------------

// R5: 0x100B7270
void CVehicle::SetEngineState(BOOL bState)
{
	if(!m_pVehicle) return;

	if(!bState) {
		m_pVehicle->byteFlags &= 0xDF;
	} else {
		m_pVehicle->byteFlags |= 0x20;
	}
}

//-----------------------------------------------------------

// R5: 0x100B72A0
float CVehicle::GetHealth()
{
	if(m_pVehicle) return m_pVehicle->fHealth;
	else return 0.0f;
}

//-----------------------------------------------------------

// R5: 0x100B72C0
void CVehicle::SetHealth(float fHealth)
{
	if(m_pVehicle) {
		m_pVehicle->fHealth = fHealth;
	}
}

//-----------------------------------------------------------

// R5: 0x100B72E0
void CVehicle::SetColor(BYTE byteColor1, BYTE byteColor2)
{
	if(m_pVehicle && GamePool_Vehicle_GetAt(m_dwGTAId))  {
		m_pVehicle->byteColor1 = byteColor1;
		m_pVehicle->byteColor2 = byteColor2;
	}
	m_byteColor2 = byteColor2;
	m_byteColor1 = byteColor1;
	m_bHasNewColor = TRUE;
}

//-----------------------------------------------------------

// R5: 0x100B7330
void CVehicle::UpdateColor()
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	if(m_bHasNewColor) {
		if(!field_71) {
			if(m_byteColor1 != m_pVehicle->byteColor1 || m_byteColor2 != m_pVehicle->byteColor2) {
				m_pVehicle->byteColor1 = m_byteColor1;
				m_pVehicle->byteColor2 = m_byteColor2;
			}
		}
	}
}

//-----------------------------------------------------------

// R5: 0x100B7390
UINT CVehicle::GetVehicleSubtype()
{
	if(m_pVehicle) {
		return GetVehicleSubtypeFromVehiclePtr(m_pVehicle);
	}
	return 0;
}

//-----------------------------------------------------------

// R5: 0x100B73B0
BOOL CVehicle::HasSunk()
{
	if(!m_pVehicle) return FALSE;

	return ScriptCommand(&has_car_sunk,m_dwGTAId);
}

//-----------------------------------------------------------

// R5: 0x100B73D0
BOOL CVehicle::IsWrecked()
{
	if(!m_pVehicle) return FALSE;

	return ScriptCommand(&is_car_wrecked,m_dwGTAId);
}

//-----------------------------------------------------------

// R5: 0x100B73F0
BOOL CVehicle::IsDriverLocalPlayer()
{
	if(m_pVehicle) {
		if((PED_TYPE *)m_pVehicle->pDriver == GamePool_FindPlayerPed()) {
			return TRUE;
		}
	}
	return FALSE;
}

//-----------------------------------------------------------

// R5: 0x100B7460
BOOL CVehicle::IsATrainPart()
{
	int nModel;
	if(m_pVehicle) {
		nModel = m_pVehicle->entity.nModelIndex;
		if(nModel == TRAIN_PASSENGER_LOCO) return TRUE;
		if(nModel == TRAIN_PASSENGER) return TRUE;
		if(nModel == TRAIN_FREIGHT_LOCO) return TRUE;
		if(nModel == TRAIN_FREIGHT) return TRUE;
		if(nModel == TRAIN_TRAM) return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

// R5: 0x100B74A0
BOOL CVehicle::HasTurret()
{
	int nModel = GetModelIndex();
	return (nModel == 432 ||	// Tank
			nModel == 564 ||	// RC Tank
			nModel == 407 ||	// Firetruck
			nModel == 601		// Swatvan
			);
}

//-----------------------------------------------------------
// This can currently only be used for setting the alternate
// siren. The way it's coded internally doesn't seem to allow
// us to modify the horn alone.

void CVehicle::SetHornState(BYTE byteState)
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;

	if( (GetVehicleSubtype() != VEHICLE_SUBTYPE_BOAT ) &&
		(GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE) &&
		(GetVehicleSubtype() != VEHICLE_SUBTYPE_HELI ) )
	{
		m_pVehicle->byteHorn = byteState;
		m_pVehicle->byteHorn2 = byteState;
	}
}

//-----------------------------------------------------------

// R5: 0x100B7540
void CVehicle::SetSirenOn(BYTE byteState)
{
	m_pVehicle->bSirenOn = byteState;
}

//-----------------------------------------------------------

// R5: 0x100B7560
BOOL CVehicle::IsSirenOn()
{
	return (m_pVehicle->bSirenOn == 1);
}

//-----------------------------------------------------------

// R5: 0x100B7580
void CVehicle::SetAlarmState(WORD wState)
{
	m_pVehicle->wAlarmState = wState;
}

//-----------------------------------------------------------

void CVehicle::SetLandingGearState(eLandingGearState state)
{
	if(GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE) return;

	DWORD dwVehiclePtr = (DWORD)m_pVehicle;
	float fPlaneLandingGear = 0.0f;

	_asm mov eax, dwVehiclePtr
	_asm mov edx, [eax+0x9CC]
	_asm mov fPlaneLandingGear, edx

	if (state == LGS_DOWN && fPlaneLandingGear == 0.0f)
	{
		_asm mov ecx, dwVehiclePtr
		_asm mov edx, 0x6CAC20
		_asm call edx
	}
	else if(state == LGS_UP && fPlaneLandingGear == 1.0f)
	{
		_asm mov ecx, dwVehiclePtr
		_asm mov edx, 0x6CAC70
		_asm call edx		
	}
}

//-----------------------------------------------------------

eLandingGearState CVehicle::GetLandingGearState()
{
	if(GetVehicleSubtype() != VEHICLE_SUBTYPE_PLANE) return LGS_UP;

	DWORD dwVehiclePtr = (DWORD)m_pVehicle;
	float fPlaneLandingGear = 0.0f;

	_asm mov eax, dwVehiclePtr
	_asm mov edx, [eax+0x9CC]
	_asm mov fPlaneLandingGear, edx

	if(fPlaneLandingGear == 0.0f) return LGS_UP;
	return LGS_DOWN;
}

//-----------------------------------------------------------

UINT CVehicle::GetPassengersMax()
{
	return 0;
}

//-----------------------------------------------------------

// R5: 0x100B76A0
void CVehicle::SetHydraThrusters(DWORD dwDirection)
{
	if(m_pVehicle) m_pVehicle->dwHydraThrusters = dwDirection; // 0x00 - 0x80 // byte
}

//-----------------------------------------------------------

// R5: 0x100B76C0
DWORD CVehicle::GetHydraThrusters()
{
	if(m_pVehicle) return m_pVehicle->dwHydraThrusters;
	return 0UL;
}

//-----------------------------------------------------------

//-----------------------------------------------------------


// Transferred from michael-fa-samp dc9eb80, client/game/vehicle.cpp.
// R5: 0x100B78A0
void CVehicle::SetTankRot(float X, float Y)
{
	m_pVehicle->fTankRotX = X;
	m_pVehicle->fTankRotY = Y;
}

// R5: 0x100B78C0
float CVehicle::GetTankRotX()
{
	return m_pVehicle->fTankRotX;
}

// R5: 0x100B78D0
float CVehicle::GetTankRotY()
{
	return m_pVehicle->fTankRotY;
}

//-----------------------------------------------------------

// R5: 0x100B7C10
void CVehicle::AttachTrailer()
{
	if(m_pTrailer) ScriptCommand(&r5PutTrailerOnCab, m_pTrailer->m_dwGTAId, m_dwGTAId);
}

// R5: 0x100B7C30
void CVehicle::DetachTrailer()
{
	r5TrailerDetachInProgress = TRUE;
	if(m_pTrailer && m_pTrailer->m_dwGTAId && GamePool_Vehicle_GetAt(m_pTrailer->m_dwGTAId) && m_pTrailer->m_pVehicle)
		ScriptCommand(&r5DetachTrailerFromCab, m_pTrailer->m_dwGTAId, m_dwGTAId);
	r5TrailerDetachInProgress = FALSE;
}

// R5: 0x100B7C80
void CVehicle::SetTrailer(CVehicle *pTrailer)
{
	m_pTrailer = pTrailer;
}

// R5: 0x100B7C90
CVehicle *CVehicle::GetTrailer()
{
	if(!m_pVehicle) return NULL;
	VEHICLE_TYPE *trailer = (VEHICLE_TYPE*)m_pVehicle->dwTrailer;
	if(trailer && pNetGame) {
		CVehiclePool *pool = pNetGame->GetVehiclePool();
		if(pool) {
			WORD id = pool->FindIDFromGtaPtr(trailer);
			if(id != 0xffff && pool->GetSlotState(id)) return pool->GetAt(id);
		}
	}
	return NULL;
}

// R5: 0x100B7EE0
void CVehicle::RemoveEveryoneFromVehicle()
{
	if(!m_pVehicle) return;
	if(!GamePool_Vehicle_GetAt(m_dwGTAId)) return;
	float x = m_pVehicle->entity.mat->pos.X;
	float y = m_pVehicle->entity.mat->pos.Y;
	float z = m_pVehicle->entity.mat->pos.Z;
	int actor = 0;
	if(m_pVehicle->pDriver) {
		actor = GamePool_Ped_GetIndex(m_pVehicle->pDriver);
		ScriptCommand(&remove_actor_from_car_and_put_at, actor, x, y, z + 2.0f);
	}
	for(int i = 0; i < 7; i++) {
		if(m_pVehicle->pPassengers[i]) {
			actor = GamePool_Ped_GetIndex(m_pVehicle->pPassengers[i]);
			ScriptCommand(&remove_actor_from_car_and_put_at, actor, x, y, z + 2.0f);
		}
	}
}

// R5: 0x100B88F0
CVehicle::~CVehicle()
{
	m_pVehicle = GamePool_Vehicle_GetAt(m_dwGTAId);
	if(m_pVehicle) {
		if(*(DWORD*)((BYTE*)this + 0xb4)) {
			ScriptCommand(&disable_marker, *(DWORD*)((BYTE*)this + 0xb4));
			*(DWORD*)((BYTE*)this + 0xb4) = 0;
		}
		RemoveEveryoneFromVehicle();
		if(m_pTrailer) {
			DetachTrailer();
			m_pTrailer = 0;
		}
		IUnknown *&texture = *(IUnknown**)((BYTE*)this + 0x8f);
		if(texture) {
			texture->Release();
			texture = 0;
		}
		if(m_pVehicle->entity.nModelIndex == 538 || m_pVehicle->entity.nModelIndex == 537) {
			ScriptCommand(&r5DestroyTrain, m_dwGTAId);
		} else {
			int model = m_pVehicle->entity.nModelIndex;
			ScriptCommand(&r5DestroyVehicle, m_dwGTAId);
			if(!GetModelReferenceCount(model) && !*(DWORD*)((BYTE*)this + 0x87) &&
			   pGame->GetLoadedVehicleModelCount() > 80 && pGame->IsModelLoaded(model)) {
				_asm push model
				_asm mov edx, 0x4089a0
				_asm call edx
				_asm pop edx
			}
		}
	}
}

// R5: 0x100B8A10
void CVehicle::Add()
{
	if(!IsAdded()) {
		CEntity::Add();
		CVehicle *trailer = GetTrailer();
		if(trailer) trailer->Add();
	}
}

// R5: 0x100B8A40
void CVehicle::Remove()
{
	if(IsAdded()) {
		CVehicle *trailer = GetTrailer();
		if(trailer) trailer->Remove();
		CEntity::Remove();
	}
}

