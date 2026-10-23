
#include "../main.h"

using namespace RakNet;
extern CNetGame*	pNetGame;
extern CGame * pGame;

#define RPC_ScrPlaySound RPC_ScrUnk10
#define RPC_ScrRespawnVehicle RPC_ScrUnk40
#define RPC_ScrDestroyObject RPC_ScrUnk2F
#define RPC_ScrSetVehiclePos RPC_ScrUnk9F
#define RPC_ScrSetVehicleHealth RPC_ScrUnk93

struct RPCSoundGameView {
	CAudio *audio;
	CAudio *GetAudio() { return audio; }
};

struct RPCObjectPoolView {
	int lastId;
	BOOL slots[MAX_OBJECTS];
	CEntity *objects[MAX_OBJECTS];
	CEntity *GetAt(WORD id) {
		if (id > 1000) return NULL;
		if (slots[id]) return objects[id];
		return NULL;
	}
};

struct RPCPlayerSurfingView {
	BYTE gap[0x2DA];
	CEntity *surfObject;
	DWORD gap2DE;
	DWORD surfMode;
};

struct RPCInitMenuPoolView {
	CMenu *menus[MAX_MENUS];
	BOOL slots[MAX_MENUS];
	BOOL GetSlotState(BYTE id) {
		if (id > 128) return FALSE;
		return slots[id];
	}
};

static char rpc_menu_text_buffer[257];

//----------------------------------------------------

// TODO: These script RPCs
void ScrUnkA7(RPCParameters *rpcParams) {}
void ScrUnk38(RPCParameters *rpcParams) {}
void ScrUnk90(RPCParameters *rpcParams) {}
void ScrUnk91(RPCParameters *rpcParams) {}
// R5: 0x1001ACD0
void ScrSetGravity(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	float fGravity;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	bsData.Read(fGravity);

	pGame->SetGravity(fGravity);
}
// R5: 0x1001AD70
void ScrSetVehicleHealth(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	WORD VehicleID;
	float fVehicleHealth;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	bsData.Read(VehicleID);
	bsData.Read(fVehicleHealth);

	if (pNetGame && pNetGame->GetVehiclePool()) {
		CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
		if (pVehiclePool->GetSlotState(VehicleID)) {
			CVehicle *pVehicle = pVehiclePool->GetAt(VehicleID);
			if (pVehicle) pVehicle->SetHealth(fVehicleHealth);
		}
	}
}
void ScrUnk94(RPCParameters *rpcParams) {}
void ScrUnk95(RPCParameters *rpcParams) {}
void ScrUnk2C(RPCParameters *rpcParams) {}
void ScrUnk2D(RPCParameters *rpcParams) {}
void ScrUnk2E(RPCParameters *rpcParams) {}
// R5: 0x1001BC20
void ScrDestroyObject(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int bits = rpcParams->numberOfBitsOfData;
	WORD objectId;
	RakNet::BitStream stream(Data, (bits / 8) + 1, false);
	stream.Read(objectId);

	if (!pNetGame) return;
	CObjectPool *pObjectPool = pNetGame->GetObjectPool();
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	if (!pObjectPool) return;

	RPCObjectPoolView *pool = (RPCObjectPoolView *)pObjectPool;
	RPCPlayerSurfingView *local = pPlayerPool ? (RPCPlayerSurfingView *)pPlayerPool->GetLocalPlayer() : NULL;
	CEntity *object = pool->GetAt(objectId);
	if (object) {
		if (local && local->surfObject == object) {
			local->surfObject = NULL;
			local->surfMode = 0;
		}
		pObjectPool->Delete(objectId);
	}
}
void ScrUnk4F(RPCParameters *rpcParams) {}
void ScrUnk50(RPCParameters *rpcParams) {}
void ScrUnk63(RPCParameters *rpcParams) {}
void ScrUnk7A(RPCParameters *rpcParams) {}
void ScrUnk7B(RPCParameters *rpcParams) {}
void ScrUnk7C(RPCParameters *rpcParams) {}

void ScrUnk7D(RPCParameters *rpcParams)
{
	// nothing
}

void ScrUnk7E(RPCParameters *rpcParams) {}
void ScrUnk7F(RPCParameters *rpcParams) {}
void ScrUnk39(RPCParameters *rpcParams) {}
void ScrUnk4B(RPCParameters *rpcParams) {}
void ScrUnk85(RPCParameters *rpcParams) {}
void ScrUnk86(RPCParameters *rpcParams) {}
void ScrUnk87(RPCParameters *rpcParams) {}
void ScrUnk69(RPCParameters *rpcParams) {}
void ScrAddGangZone(RPCParameters *rpcParams) {}
void ScrRemoveGangZone(RPCParameters *rpcParams) {}
void ScrFlashGangZone(RPCParameters *rpcParams) {}
void ScrStopFlashGangZone(RPCParameters *rpcParams) {}
#define RPC_ScrSetMapIconTransfer RPC_ScrUnk56
#define RPC_ScrDisableMapIconTransfer RPC_ScrUnk57

// R5: 0x1001A790
void ScrSetMapIconTransfer(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	BYTE byteStyle;
	BYTE byteIndex;
	BYTE byteIcon;
	DWORD byteColor;
	float fPos[3];

	bsData.Read(byteIndex);
	bsData.Read(fPos[0]);
	bsData.Read(fPos[1]);
	bsData.Read(fPos[2]);
	bsData.Read(byteIcon);
	bsData.Read(byteColor);
	bsData.Read(byteStyle);

	pNetGame->SetMapIcon(byteIndex, fPos[0], fPos[1], fPos[2], byteIcon, byteColor, byteStyle);
}

// R5: 0x1001A8B0
void ScrDisableMapIconTransfer(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	BYTE byteIndex;

	bsData.Read(byteIndex);

	pNetGame->DisableMapIcon(byteIndex);
}
void ScrUnk58(RPCParameters *rpcParams) {}
void ScrUnk59(RPCParameters *rpcParams) {}
void ScrUnk5A(RPCParameters *rpcParams) {}
void ScrUnk5B(RPCParameters *rpcParams) {}
void ScrUnk6F(RPCParameters *rpcParams) {}
void ScrUnk62(RPCParameters *rpcParams) {}
void ScrUnk70(RPCParameters *rpcParams) {}
void ScrSetSpawnInfo(RPCParameters *rpcParams) {}
void ScrUnk45(RPCParameters *rpcParams) {}
void ScrUnk99(RPCParameters *rpcParams) {}
void ScrSetPlayerPos(RPCParameters *rpcParams) {}
void ScrUnk0D(RPCParameters *rpcParams) {}
void ScrPutPlayerInVehicle(RPCParameters *rpcParams) {}
void ScrRemovePlayerFromVehicle(RPCParameters *rpcParams) {}
// R5: 0x1001A1C0
// R5: 0x100198F0
void ScrDisplayGameText(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int bits = rpcParams->numberOfBitsOfData;
	RakNet::BitStream stream(Data, (bits / 8) + 1, false);
	char message[256];
	memset(message, 0, sizeof(message));
	int type, time, length;
	stream.Read(type);
	stream.Read(time);
	stream.Read(length);
	if (length >= 1 && length <= 200) {
		stream.Read(message, length);
		message[length] = '\0';
		pGame->DisplayGameText(message, time, type);
	}
}

// R5: 0x10019A00
void ScrSetInterior(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char *)Data, (iBitLength/8)+1, false);

	BYTE byteInterior;
	bsData.Read(byteInterior);

	pGame->FindPlayerPed()->SetInterior(byteInterior, TRUE);
}

// R5: 0x10019C70
void ScrSetVehiclePos(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	WORD VehicleId;
	float fX, fY, fZ;
	bsData.Read(VehicleId);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);

	if (pNetGame && pNetGame->GetVehiclePool()) {
		CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
		if (pVehiclePool->GetSlotState(VehicleId)) {
			CVehicle *pVehicle = pVehiclePool->GetAt(VehicleId);
			if (pVehicle) pVehicle->TeleportTo(fX, fY, fZ);
		}
	}
}
void ScrUnkA0(RPCParameters *rpcParams) {}
void ScrUnkA1(RPCParameters *rpcParams) {}
void ScrUnk0F(RPCParameters *rpcParams) {}
// R5: 0x1001A330
void ScrPlaySound(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	int iSound;
	float fX, fY, fZ;
	bsData.Read(iSound);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);

	((RPCSoundGameView *)pGame)->GetAudio()->PlaySound(iSound, fX, fY, fZ);
}
void ScrUnk11(RPCParameters *rpcParams) {}
// R5: 0x1001A500
void ScrUnk12(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	int iAmount;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	bsData.Read(iAmount);
	pGame->AddToLocalMoney(iAmount);
}
void ScrSetPlayerFacingAngle(RPCParameters *rpcParams) {}
void ScrUnk16(RPCParameters *rpcParams) {}
// R5: 0x1001A700
void ScrRespawnVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	VEHICLEID VehicleID;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	bsData.Read(VehicleID);
}
void ScrUnk41(RPCParameters *rpcParams) {}
void ScrUnk42(RPCParameters *rpcParams) {}
void ScrUnk37(RPCParameters *rpcParams) {}
void ScrUnk21(RPCParameters *rpcParams) {}
void ScrUnk23(RPCParameters *rpcParams) {}
void ScrUnk43(RPCParameters *rpcParams) {}
void ScrUnk71(RPCParameters *rpcParams) {}
void ScrUnk29(RPCParameters *rpcParams) {}
void ScrUnk2A(RPCParameters *rpcParams) {}
void ScrUnk2B(RPCParameters *rpcParams) {}
void ScrUnk51(RPCParameters *rpcParams) {}
void ScrUnk52(RPCParameters *rpcParams) {}
void ScrUnk53(RPCParameters *rpcParams) {}
void ScrUnk54(RPCParameters *rpcParams) {}
void ScrUnkA9(RPCParameters *rpcParams) {}
void ScrUnkAD(RPCParameters *rpcParams) {}
void ScrUnkAE(RPCParameters *rpcParams) {}
void ScrUnkAF(RPCParameters *rpcParams) {}
void ScrUnkB0(RPCParameters *rpcParams) {}
void ScrUnkB2(RPCParameters *rpcParams) {}
void ScrUnk30(RPCParameters *rpcParams) {}
// R5: 0x1001C870
void ScrInitMenu(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	if (!pNetGame) return;
	CMenuPool *pMenuPool = pNetGame->GetMenuPool();
	if (!pMenuPool) return;

	BYTE byteMenuID;
	BOOL bColumns; // 0 = 1, 1 = 2

	float fX;
	float fY;
	float fCol1;
	float fCol2 = 0.0f;
	MENU_INT MenuInteraction;

	bsData.Read(byteMenuID);
	bsData.Read(bColumns);
	memset(rpc_menu_text_buffer, 0, sizeof(rpc_menu_text_buffer));
	bsData.Read(rpc_menu_text_buffer, MAX_MENU_LINE);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fCol1);
	if (bColumns) bsData.Read(fCol2);
	bsData.Read(MenuInteraction.bMenu);
	for (BYTE i = 0; i < MAX_MENU_ITEMS; i++)
	{
		bsData.Read(MenuInteraction.bRow[i]);
	}

	CMenu *pMenu;

	if (((RPCInitMenuPoolView *)pMenuPool)->GetSlotState(byteMenuID))
	{
		pMenuPool->Delete(byteMenuID);
	}

	pMenu = pMenuPool->New(byteMenuID, fX, fY, ((BYTE)bColumns) + 1, fCol1, fCol2, &MenuInteraction);

	if (!pMenu) return;
	pMenu->SetTitle(rpc_menu_text_buffer);

	BYTE byteColCount;
	memset(rpc_menu_text_buffer, 0, sizeof(rpc_menu_text_buffer));
	bsData.Read(rpc_menu_text_buffer, MAX_MENU_LINE);
	pMenu->SetColumnTitle(0, rpc_menu_text_buffer);

	bsData.Read(byteColCount);
	if (byteColCount > MAX_MENU_ITEMS) return;
	for (BYTE i = 0; i < byteColCount; i++)
	{
		memset(rpc_menu_text_buffer, 0, sizeof(rpc_menu_text_buffer));
		bsData.Read(rpc_menu_text_buffer, MAX_MENU_LINE);
		pMenu->AddMenuItem(0, i, rpc_menu_text_buffer);
	}

	if (bColumns)
	{
		memset(rpc_menu_text_buffer, 0, sizeof(rpc_menu_text_buffer));
		bsData.Read(rpc_menu_text_buffer, MAX_MENU_LINE);
		pMenu->SetColumnTitle(1, rpc_menu_text_buffer);

		bsData.Read(byteColCount);
		if (byteColCount > MAX_MENU_ITEMS) return;
		for (BYTE i = 0; i < byteColCount; i++)
		{
			memset(rpc_menu_text_buffer, 0, sizeof(rpc_menu_text_buffer));
			bsData.Read(rpc_menu_text_buffer, MAX_MENU_LINE);
			pMenu->AddMenuItem(1, i, rpc_menu_text_buffer);
		}
	}
}

// R5: 0x1001CB90
void ScrShowMenu(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	if (!pNetGame) return;
	CMenuPool *pMenuPool = pNetGame->GetMenuPool();

	BYTE byteMenuID;
	bsData.Read(byteMenuID);
	if (pMenuPool) pMenuPool->ShowMenu(byteMenuID);
}

// R5: 0x1001CC40
void ScrHideMenu(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	if (!pNetGame) return;
	CMenuPool *pMenuPool = pNetGame->GetMenuPool();

	BYTE byteMenuID;
	bsData.Read(byteMenuID);
	if (pMenuPool) pMenuPool->HideMenu(byteMenuID);
}

//----------------------------------------------------

void ScrSetPlayerName(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	PLAYERID playerId;
	BYTE byteNickLen;
	char szNewName[MAX_PLAYER_NAME+1];
	BYTE byteSuccess;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();

	bsData.Read(playerId);
	bsData.Read(byteNickLen);

	if(byteNickLen > MAX_PLAYER_NAME) return;

	bsData.Read(szNewName, byteNickLen);
	bsData.Read(byteSuccess);

	szNewName[byteNickLen] = '\0';

	if (byteSuccess == 1) pPlayerPool->SetPlayerName(playerId, szNewName);

	// Extra addition which we need to do if this is the local player;
	if( pPlayerPool->GetLocalPlayerID() == playerId )
		pPlayerPool->SetLocalPlayerName( szNewName );
}

//----------------------------------------------------

void ScrSetPlayerHealth(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	float fHealth;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	bsData.Read(fHealth);

	pLocalPlayer->GetPlayerPed()->SetHealth(fHealth);
}

//----------------------------------------------------

void ScrSetPlayerColor(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	PLAYERID playerId;
	DWORD dwColor;

	bsData.Read(playerId);
	bsData.Read(dwColor);

	if(playerId == pPlayerPool->GetLocalPlayerID()) {
		pPlayerPool->GetLocalPlayer()->SetPlayerColor(dwColor);
	} else {
		CRemotePlayer *pPlayer = pPlayerPool->GetAt(playerId);
		if(pPlayer)	pPlayer->SetPlayerColor(dwColor);
	}
}


//----------------------------------------------------

// R5: 0x10019AA0
void ScrSetCameraPos(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	VECTOR vecPos;
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	pGame->GetCamera()->SetPosition(vecPos.X,vecPos.Y,vecPos.Z,0.0f,0.0f,0.0f);
}

//----------------------------------------------------

// R5: 0x10019B70
void ScrSetCameraLookAt(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);
	VECTOR vecPos;
	BYTE byteType=0;
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(byteType);
	if(byteType < 1 || byteType > 2) {
		byteType = 2;
	}
	pGame->GetCamera()->LookAtPoint(vecPos.X,vecPos.Y,vecPos.Z,byteType);
}

//----------------------------------------------------

// R5: 0x10018080
void ScrSetCameraBehindPlayer(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	pGame->GetCamera()->SetBehindPlayer();
}

//----------------------------------------------------

// R5: 0x10018090
void ScrResetMoney(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	pGame->ResetLocalMoney();
}

//----------------------------------------------------

// R5: 0x100180A0
void ScrResetPlayerWeapons(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;

	CPlayerPed *pPlayerPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
	pPlayerPed->ClearAllWeapons();
}

//----------------------------------------------------

// R5: 0x100180D0
void ScrForceSpawnSelection(RPCParameters *rpcParams)
{
	pNetGame->GetPlayerPool()->GetLocalPlayer()->ReturnToClassSelection();
}

//----------------------------------------------------

// R5: 0x10017D50
void ScrEnableStuntBonus(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	bool bStuntBonusEnabled;
	bsData.Read(bStuntBonusEnabled);
	pGame->EnableStuntBonus(bStuntBonusEnabled);
}

//----------------------------------------------------

void ScrSetPlayerDrunkVisuals(RPCParameters* rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	int iVisuals;
	bsData.Read(iVisuals);

	CPlayerPed* pPlayer = pGame->FindPlayerPed();
	if (pPlayer) {
		ScriptCommand(&set_player_drunk_visuals, 0, iVisuals);
	}
}

//----------------------------------------------------

void ScrSetPlayerDrunkHandling(RPCParameters* rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data,(iBitLength/8)+1,false);

	int iVisuals;
	bsData.Read(iVisuals);

	CPlayerPed* pPlayer = pGame->FindPlayerPed();
	if (pPlayer) {
		ScriptCommand(&handling_responsiveness, 0, iVisuals);
	}
}

//----------------------------------------------------

void RegisterScriptRPCs(RakClientInterface* pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA7);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	REGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehicleHealth);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	REGISTER_STATIC_RPC(pRakClient, ScrDestroyObject);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	REGISTER_STATIC_RPC(pRakClient, ScrForceSpawnSelection);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk69);
	REGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	REGISTER_STATIC_RPC(pRakClient, ScrSetMapIconTransfer);
	REGISTER_STATIC_RPC(pRakClient, ScrDisableMapIconTransfer);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk58);
	REGISTER_STATIC_RPC(pRakClient, ScrEnableStuntBonus);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk59);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk5A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk5B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk6F);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk62);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkVisuals);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkHandling);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk70);
	REGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerHealth);
	REGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	REGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	REGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	REGISTER_STATIC_RPC(pRakClient, ScrSetVehiclePos);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	REGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	REGISTER_STATIC_RPC(pRakClient, ScrPlaySound);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	REGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	REGISTER_STATIC_RPC(pRakClient, ScrResetMoney);
	REGISTER_STATIC_RPC(pRakClient, ScrResetPlayerWeapons);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	REGISTER_STATIC_RPC(pRakClient, ScrRespawnVehicle);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk42);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk21);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk23);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk43);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk71);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk29);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2A);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk2B);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk51);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk52);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk53);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk54);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAD);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAE);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkAF);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkB0);
	REGISTER_STATIC_RPC(pRakClient, ScrUnkB2);
	REGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	REGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	REGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------

void UnRegisterScriptRPCs(RakClientInterface* pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA7);
	UNREGISTER_STATIC_RPC(pRakClient, ScrForceSpawnSelection);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk85);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk86);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk87);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk69);
	UNREGISTER_STATIC_RPC(pRakClient, ScrAddGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemoveGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrStopFlashGangZone);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetMapIconTransfer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDisableMapIconTransfer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk58);
	UNREGISTER_STATIC_RPC(pRakClient, ScrEnableStuntBonus);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk59);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk5A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk5B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk6F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk62);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkVisuals);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerDrunkHandling);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk70);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk38);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk90);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk91);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetGravity);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehicleHealth);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk94);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk95);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDestroyObject);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk4F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk50);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk63);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7C);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7E);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk7F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk39);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk21);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk23);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk43);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk71);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk29);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2A);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk2B);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetSpawnInfo);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk45);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerName);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk99);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0D);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerHealth);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPutPlayerInVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRemovePlayerFromVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerColor);
	UNREGISTER_STATIC_RPC(pRakClient, ScrDisplayGameText);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetInterior);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraPos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraLookAt);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetVehiclePos);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA0);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA1);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetCameraBehindPlayer);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk0F);
	UNREGISTER_STATIC_RPC(pRakClient, ScrPlaySound);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk11);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk12);
	UNREGISTER_STATIC_RPC(pRakClient, ScrSetPlayerFacingAngle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrResetMoney);
	UNREGISTER_STATIC_RPC(pRakClient, ScrResetPlayerWeapons);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk16);
	UNREGISTER_STATIC_RPC(pRakClient, ScrRespawnVehicle);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk41);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk37);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk51);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk52);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk53);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk54);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkA9);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAD);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAE);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkAF);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkB0);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnkB2);
	UNREGISTER_STATIC_RPC(pRakClient, ScrUnk30);
	UNREGISTER_STATIC_RPC(pRakClient, ScrInitMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrShowMenu);
	UNREGISTER_STATIC_RPC(pRakClient, ScrHideMenu);
}

//----------------------------------------------------
