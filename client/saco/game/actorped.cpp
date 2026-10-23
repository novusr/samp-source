
#include "../main.h"
#include "game.h"
#include "util.h"

extern CGame *pGame;

//-----------------------------------------------------------
// This is the constructor for creating new player.

// R5: 0x1009C2B0
CActorPed::CActorPed(int iSkin, float fX, float fY,float fZ,float fRotation)
{
	m_pPed=0;
	m_dwGTAId=0;
	m_byteImmune=0;
	int iCounter=0;
	DWORD dwActorID=0;

	if(!pGame->IsModelLoaded(iSkin)) {
		pGame->RequestModel(iSkin);
		pGame->LoadRequestedModels();
		while(!pGame->IsModelLoaded(iSkin)) Sleep(1);
	}

	ScriptCommand(&create_actor,5,iSkin,fX,fY,fZ-1.0f,&dwActorID);
	ScriptCommand(&set_actor_z_angle,dwActorID,fRotation);

	m_dwGTAId = dwActorID;
	m_pPed = GamePool_Ped_GetAt(m_dwGTAId);
	m_pEntity = (ENTITY_TYPE *)m_pPed;
	ScriptCommand(&set_actor_can_be_decapitated,m_dwGTAId,0);
	ScriptCommand(&set_actor_dicision,m_dwGTAId,65542);
}

//-----------------------------------------------------------

// R5: 0x1009C750
CActorPed::~CActorPed()
{
	Destroy();
}

//-----------------------------------------------------------

// R5: 0x1009C400
void CActorPed::Destroy()
{
	DWORD dwPedPtr = (DWORD)m_pPed;

	// If it points to the CPlaceable vtable it's not valid
	if(!m_pPed || !GamePool_Ped_GetAt(m_dwGTAId) || m_pPed->entity.vtable == 0x863C40)
	{
		m_pPed = NULL;
		m_pEntity = NULL;
		m_dwGTAId = 0;
		return;
	}

	// DESTROY METHOD
	_asm mov ecx, dwPedPtr
	_asm mov ebx, [ecx] ; vtable
	_asm push 1
	_asm call [ebx] ; destroy

	m_pPed = NULL;
	m_pEntity = NULL;
}

//-----------------------------------------------------------

// R5: 0x1009C460
void CActorPed::ApplyAnimation( char *szAnimName, char *szAnimFile, float fT,
								int opt1, int opt2, int opt3, int opt4, int iUnk )
{
	int iWaitAnimLoad=0;

	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	// Can't allow 'naughty' anims!
	if( !stricmp(szAnimFile,"SEX") )
		return;

	if (!pGame->IsAnimationLoaded(szAnimFile)) {
		pGame->RequestAnimation(szAnimFile);
		while(!pGame->IsAnimationLoaded(szAnimFile)) {
			Sleep(1);
			iWaitAnimLoad++;
			if(iWaitAnimLoad == 15) return; // we can't wait forever
		}
	}

	ScriptCommand(&apply_animation,m_dwGTAId,szAnimName,szAnimFile,fT,opt1,opt2,opt3,opt4,iUnk);
}

//-----------------------------------------------------------

DWORD dwActorPed=0;

BOOL __declspec(naked) FlushPedIntelligence()
{
	_asm mov edx, dwActorPed
	_asm mov ecx, [edx+0x47C]
	_asm push 1
	_asm mov eax, 0x601640
	_asm call eax
	_asm mov eax, 1
	_asm ret
}

// R5: 0x1009C550
void CActorPed::ClearAnimations()
{
	dwActorPed = (DWORD)m_pPed;
	if(dwActorPed) {
		FlushPedIntelligence();
	}
}

//-----------------------------------------------------------

// R5: 0x1009C570
void CActorPed::SetTargetRotation(float fRotation)
{
	if(!m_pPed) return;
	if(!GamePool_Ped_GetAt(m_dwGTAId)) return;

	m_pPed->fRotation2 = DegToRad(fRotation);
}

//-----------------------------------------------------------

// R5: 0x1009C5B0
float CActorPed::GetHealth()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fHealth;
}

//-----------------------------------------------------------

// R5: 0x1009C5D0
void CActorPed::SetHealth(float fHealth)
{
	if(!m_pPed) return;
	m_pPed->fHealth = fHealth;

	if(m_pPed->fHealth <= 0.0f) {
		ScriptCommand(&kill_actor,m_dwGTAId);
	}
}

//-----------------------------------------------------------

// R5: 0x1009C610
float CActorPed::GetArmour()
{
	if(!m_pPed) return 0.0f;
	return m_pPed->fArmour;
}

//-----------------------------------------------------------

// R5: 0x1009C630
void CActorPed::SetArmour(float fArmour)
{
	if(!m_pPed) return;
	m_pPed->fArmour = fArmour;
}

//-----------------------------------------------------------

// R5: 0x1009C650
DWORD CActorPed::GetStateFlags()
{
	if(!m_pPed) return 0;
	return m_pPed->dwStateFlags;
}

//-----------------------------------------------------------

// R5: 0x1009C660
void CActorPed::SetStateFlags(DWORD dwState)
{
	if(!m_pPed) return;
	m_pPed->dwStateFlags = dwState;
}

//-----------------------------------------------------------

// R5: 0x1009C680
BOOL CActorPed::IsDead()
{
	if(!m_pPed) return TRUE;
	if(m_pPed->fHealth > 0.0f) return FALSE;
	return TRUE;
}

//-----------------------------------------------------------

// R5: 0x1009C6B0
BYTE CActorPed::GetActionTrigger()
{
	return (BYTE)m_pPed->dwAction;
}

//-----------------------------------------------------------

// R5: 0x1009C6C0
void CActorPed::SetActionTrigger(BYTE byteTrigger)
{
	if(!m_pPed) return;

	m_pPed->dwAction = byteTrigger;
}

//-----------------------------------------------------------

// R5: 0x1009C6E0
BOOL CActorPed::IsInVehicle()
{
	if(!m_pPed) return FALSE;

	if(IN_VEHICLE(m_pPed)) {
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------

// R5: 0x1009C700
void CActorPed::ToggleImmunity(BYTE byteEnable)
{
	if(!m_pPed) return;

	m_byteImmune = byteEnable;

	if(byteEnable)
		ScriptCommand(&set_actor_immunities,m_dwGTAId,1,1,1,1,1);
	else
		ScriptCommand(&set_actor_immunities,m_dwGTAId,0,0,0,1,0);
}

//-----------------------------------------------------------
