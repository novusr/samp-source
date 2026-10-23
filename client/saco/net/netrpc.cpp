
#include "../main.h"

extern CNetGame *pNetGame;
extern CScoreBoard *pScoreBoard;
extern CGame *pGame;
extern RakNetStatisticsStruct RakServerStats;

// TODO: these RPCs
void Unk22(RPCParameters *rpcParams) {}
void Unk24(RPCParameters *rpcParams) {}
void Unk3A(RPCParameters *rpcParams) {}
void Unk3B(RPCParameters *rpcParams) {}
void Unk3D(RPCParameters *rpcParams) {}

// R5: 0x10010170
void SetCheckpoint(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	float fX, fY, fZ, fSize;

	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	bsData.Read(fSize);

	VECTOR Pos, Extent;
	Pos.X = fX; Pos.Y = fY; Pos.Z = fZ;
	Extent.X = fSize; Extent.Y = fSize; Extent.Z = fSize;

	if (pGame) {
		pGame->SetCheckpointInformation(&Pos, &Extent);
		pGame->SetCheckpointsEnabled(TRUE);
	}
}

// R5: 0x1000E5A0
void DisableCheckpoint(RPCParameters *rpcParams)
{
	if (pGame) pGame->SetCheckpointsEnabled(FALSE);
}

// R5: 0x10010280
void SetRaceCheckpoint(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	float fX, fY, fZ;
	BYTE byteType;
	VECTOR Pos, Next;

	bsData.Read(byteType);
	bsData.Read(fX); bsData.Read(fY); bsData.Read(fZ);
	Pos.X = fX; Pos.Y = fY; Pos.Z = fZ;

	bsData.Read(fX); bsData.Read(fY); bsData.Read(fZ);
	Next.X = fX; Next.Y = fY; Next.Z = fZ;

	bsData.Read(fX);

	if (pGame) {
		pGame->SetRaceCheckpointInformation(byteType, &Pos, &Next, fX);
		pGame->SetRaceCheckpointsEnabled(TRUE);
	}
}

// R5: 0x1000E5B0
void DisableRaceCheckpoint(RPCParameters *rpcParams)
{
	if (pGame) pGame->SetRaceCheckpointsEnabled(FALSE);
}

// R5: 0x100103E0
void UpdateScoresPingsIPs(RPCParameters *rpcParams)
{
	PCHAR Data = (PCHAR)rpcParams->input;
	int bitLength = rpcParams->numberOfBitsOfData;
	unsigned bytes = bitLength / 8;
	RakNet::BitStream bsData(Data, bytes + 1, false);
	if (bytes) {
		unsigned recordCount = bytes / 10;
		CPlayerPool *pool = pNetGame ? pNetGame->GetPlayerPool() : NULL;
		PLAYERID playerId;
		int score;
		DWORD ping;
		for (PLAYERID i = 0; i < recordCount; ++i) {
			bsData.Read(playerId);
			bsData.Read(score);
			bsData.Read(ping);
			if (pool && playerId < MAX_PLAYERS) {
				pool->UpdateScore(playerId, score);
				pool->UpdatePing(playerId, ping);
			}
		}
		if (pScoreBoard) pScoreBoard->UpdatePlayers();
	}
}

// R5: 0x1000E5C0
void SvrStats(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	bsData.Read((char *)&RakServerStats, sizeof(RakNetStatisticsStruct));
}

void GameModeRestart(RPCParameters *rpcParams) {}
void ConnectionRejected(RPCParameters *rpcParams) {}
void ClientMessage(RPCParameters *rpcParams) {}

// R5: 0x1000EEF0
void WorldTime(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	BYTE byteWorldTime;
	bsData.Read(byteWorldTime);
	if (pNetGame && pNetGame->GetSettings()) {
		pNetGame->GetSettings()->field_2C = byteWorldTime;
	}
}

void Pickup(RPCParameters *rpcParams) {}
void DestroyPickup(RPCParameters *rpcParams) {}
void DestroyWeaponPickup(RPCParameters *rpcParams) {}
void ScmEvent(RPCParameters *rpcParams) {}

// R5: 0x1000F370
void Weather(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	BYTE byteWeather;
	bsData.Read(byteWeather);
	if (pNetGame && pNetGame->GetSettings()) {
		pNetGame->GetSettings()->field_2E = byteWeather;
	}
	if (pGame) {
		pGame->SetWorldWeather(byteWeather);
	}
}

// R5: 0x1000F420
void SetTimeEx(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	BYTE byteHour;
	BYTE byteMinute;
	bsData.Read(byteHour);
	bsData.Read(byteMinute);

	if (pGame) {
		pGame->SetWorldTime(byteHour, byteMinute);
	}
	if (pNetGame && pNetGame->GetSettings()) {
		pNetGame->GetSettings()->field_2C = byteHour;
		pNetGame->GetSettings()->field_2D = byteMinute;
	}
}

// R5: 0x1000F500
void ToggleClock(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	BYTE byteClock;
	bsData.Read(byteClock);
	if (pGame) pGame->EnableClock(byteClock);
	if (pNetGame && pNetGame->GetSettings()) {
		if (byteClock) {
			pNetGame->GetSettings()->field_1F = 0;
		}
		else {
			pNetGame->GetSettings()->field_1F = 1;
			if (pGame) {
				pGame->GetWorldTime((BYTE*)&pNetGame->GetSettings()->field_2C, (BYTE*)&pNetGame->GetSettings()->field_2D);
			}
		}
	}
}
void Unk3C(RPCParameters *rpcParams) {}
void WorldPlayerAdd(RPCParameters *rpcParams) {}
void WorldPlayerDeath(RPCParameters *rpcParams) {}

// R5: 0x10010E00
void WorldPlayerRemove(RPCParameters *rpcParams)
{
	PCHAR Data = (PCHAR)rpcParams->input;
	int bitLength = rpcParams->numberOfBitsOfData;
	RakNet::BitStream bsData(Data, bitLength / 8 + 1, false);
	PLAYERID playerId = 0;
	CPlayerPool *pool = pNetGame ? pNetGame->GetPlayerPool() : NULL;
	bsData.Read(playerId);
	if (pool) {
		CRemotePlayer *remote = pool->GetAt(playerId);
		if (remote) remote->Remove();
	}
}

void WorldVehicleAdd(RPCParameters *rpcParams) {}
void WorldVehicleRemove(RPCParameters *rpcParams) {}
void DamageVehicle(RPCParameters *rpcParams) {}
void Unk18(RPCParameters *rpcParams) {}
void EnterVehicle(RPCParameters *rpcParams) {}
void ExitVehicle(RPCParameters *rpcParams) {}

// R5: 0x1000FCE0
void ServerJoin(RPCParameters *rpcParams)
{
	PCHAR Data = (PCHAR)rpcParams->input;
	int bitLength = rpcParams->numberOfBitsOfData;
	PlayerID sender = rpcParams->sender;
	CHAR playerName[256];
	PLAYERID playerId;
	BYTE nameLength;
	BYTE npc;
	BOOL isNPC;
	DWORD playerColor;
	RakNet::BitStream bsData(Data, bitLength / 8 + 1, false);
	CPlayerPool *pool = pNetGame ? pNetGame->GetPlayerPool() : NULL;
	memset(playerName, 0, sizeof(playerName));
	nameLength = 0;
	npc = 0;
	isNPC = FALSE;
	bsData.Read(playerId);
	bsData.Read(playerColor);
	bsData.Read(npc);
	bsData.Read(nameLength);
	bsData.Read(playerName, nameLength);
	playerName[nameLength] = 0;
	if (strlen(playerName) <= 24 && pool) {
		if (npc) isNPC = TRUE;
		pool->New(playerId, playerName, isNPC);
		if (playerColor && pool->GetSlotState(playerId)) {
			CRemotePlayer *pPlayer = pool->GetAt(playerId);
			if (pPlayer) pPlayer->SetPlayerColor(playerColor);
		}
	}
}
void ServerQuit(RPCParameters *rpcParams) {}
void InitGame(RPCParameters *rpcParams) {}
void Chat(RPCParameters *rpcParams) {}
void RequestClass(RPCParameters *rpcParams) {}
void RequestSpawn(RPCParameters *rpcParams) {}
void EditAttachedObject(RPCParameters *rpcParams) {}
void EditObject(RPCParameters *rpcParams) {}
void SelectObject(RPCParameters *rpcParams) {}
void Unk1C(RPCParameters *rpcParams) {}
void UnkAA(RPCParameters *rpcParams) {}
void ClientCheck(RPCParameters *rpcParams) {}
void UnkAB(RPCParameters *rpcParams) {}
void UnkAC(RPCParameters *rpcParams) {}

//----------------------------------------------------

void RegisterRPCs(RakClientInterface * pRakClient)
{
	REGISTER_STATIC_RPC(pRakClient,Unk22);
	REGISTER_STATIC_RPC(pRakClient,Unk24);
	REGISTER_STATIC_RPC(pRakClient,Unk3A);
	REGISTER_STATIC_RPC(pRakClient,Unk3B);
	REGISTER_STATIC_RPC(pRakClient,Unk3D);
	REGISTER_STATIC_RPC(pRakClient,SetCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,DisableCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,SetRaceCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,DisableRaceCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	REGISTER_STATIC_RPC(pRakClient,SvrStats);
	REGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	REGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	REGISTER_STATIC_RPC(pRakClient,ClientMessage);
	REGISTER_STATIC_RPC(pRakClient,WorldTime);
	REGISTER_STATIC_RPC(pRakClient,Pickup);
	REGISTER_STATIC_RPC(pRakClient,DestroyPickup);
	REGISTER_STATIC_RPC(pRakClient,DestroyWeaponPickup);
	REGISTER_STATIC_RPC(pRakClient,ScmEvent);
	REGISTER_STATIC_RPC(pRakClient,Weather);
	REGISTER_STATIC_RPC(pRakClient,SetTimeEx);
	REGISTER_STATIC_RPC(pRakClient,ToggleClock);
	REGISTER_STATIC_RPC(pRakClient,Unk3C);
	REGISTER_STATIC_RPC(pRakClient,WorldPlayerAdd);
	REGISTER_STATIC_RPC(pRakClient,WorldPlayerDeath);
	REGISTER_STATIC_RPC(pRakClient,WorldPlayerRemove);
	REGISTER_STATIC_RPC(pRakClient,WorldVehicleAdd);
	REGISTER_STATIC_RPC(pRakClient,WorldVehicleRemove);
	REGISTER_STATIC_RPC(pRakClient,DamageVehicle);
	REGISTER_STATIC_RPC(pRakClient,Unk18);
	REGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	REGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	REGISTER_STATIC_RPC(pRakClient,ServerJoin);
	REGISTER_STATIC_RPC(pRakClient,ServerQuit);
	REGISTER_STATIC_RPC(pRakClient,InitGame);
	REGISTER_STATIC_RPC(pRakClient,Chat);
	REGISTER_STATIC_RPC(pRakClient,RequestClass);
	REGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	REGISTER_STATIC_RPC(pRakClient,EditAttachedObject);
	REGISTER_STATIC_RPC(pRakClient,EditObject);
	REGISTER_STATIC_RPC(pRakClient,SelectObject);
	REGISTER_STATIC_RPC(pRakClient,Unk1C);
	REGISTER_STATIC_RPC(pRakClient,UnkAA);
	REGISTER_STATIC_RPC(pRakClient,ClientCheck);
	REGISTER_STATIC_RPC(pRakClient,UnkAB);
	REGISTER_STATIC_RPC(pRakClient,UnkAC);
}

//----------------------------------------------------

void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	UNREGISTER_STATIC_RPC(pRakClient,Unk3C);
	UNREGISTER_STATIC_RPC(pRakClient,WorldPlayerAdd);
	UNREGISTER_STATIC_RPC(pRakClient,WorldPlayerDeath);
	UNREGISTER_STATIC_RPC(pRakClient,WorldPlayerRemove);
	UNREGISTER_STATIC_RPC(pRakClient,WorldVehicleAdd);
	UNREGISTER_STATIC_RPC(pRakClient,WorldVehicleRemove);
	UNREGISTER_STATIC_RPC(pRakClient,Unk18);
	UNREGISTER_STATIC_RPC(pRakClient,RequestClass);
	UNREGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	UNREGISTER_STATIC_RPC(pRakClient,DamageVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	UNREGISTER_STATIC_RPC(pRakClient,Unk22);
	UNREGISTER_STATIC_RPC(pRakClient,Unk24);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3A);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3B);
	UNREGISTER_STATIC_RPC(pRakClient,Unk3D);
	UNREGISTER_STATIC_RPC(pRakClient,ServerJoin);
	UNREGISTER_STATIC_RPC(pRakClient,ServerQuit);
	UNREGISTER_STATIC_RPC(pRakClient,InitGame);
	UNREGISTER_STATIC_RPC(pRakClient,Chat);
	UNREGISTER_STATIC_RPC(pRakClient,SetCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,DisableCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,SetRaceCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,DisableRaceCheckpoint);
	UNREGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	UNREGISTER_STATIC_RPC(pRakClient,SvrStats);
	UNREGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	UNREGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	UNREGISTER_STATIC_RPC(pRakClient,ClientMessage);
	UNREGISTER_STATIC_RPC(pRakClient,WorldTime);
	UNREGISTER_STATIC_RPC(pRakClient,Pickup);
	UNREGISTER_STATIC_RPC(pRakClient,DestroyPickup);
	UNREGISTER_STATIC_RPC(pRakClient,DestroyWeaponPickup);
	UNREGISTER_STATIC_RPC(pRakClient,ScmEvent);
	UNREGISTER_STATIC_RPC(pRakClient,Weather);
	UNREGISTER_STATIC_RPC(pRakClient,SetTimeEx);
	UNREGISTER_STATIC_RPC(pRakClient,ToggleClock);
	UNREGISTER_STATIC_RPC(pRakClient,EditAttachedObject);
	UNREGISTER_STATIC_RPC(pRakClient,EditObject);
	UNREGISTER_STATIC_RPC(pRakClient,SelectObject);
	UNREGISTER_STATIC_RPC(pRakClient,Unk1C);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAA);
	UNREGISTER_STATIC_RPC(pRakClient,ClientCheck);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAB);
	UNREGISTER_STATIC_RPC(pRakClient,UnkAC);
}

//----------------------------------------------------
