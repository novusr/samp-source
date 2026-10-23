
#include "main.h"
#include "game/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;
extern CDeathWindow	 *pDeathWindow;
extern CNetGame		 *pNetGame;
extern CGame         *pGame;
extern GAME_SETTINGS tSettings;
extern CConfig		 *pConfig;
extern CFontRender   *pDefaultFont;

extern bool bShowDebugLabels;
extern bool bHudScaleFix;
bool bShowCameraTarget = false;
BOOL r5ObjectLightingToggle = TRUE;

extern void QuitGame();
extern PCHAR GetSAMPPath();
extern void RefreshApplicationUiFonts();

const SCRIPT_COMMAND r5CmdGetCarZAngle = { 0x0174, "iv" };
const SCRIPT_COMMAND r5CmdGetActorZAngle = { 0x0172, "iv" };

// R5: 0x100684E0
void cmdDefaultCmdProc(PCHAR szCmd)
{
	if(pNetGame) {
		CLocalPlayer *pLocalPlayer;
		pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		pLocalPlayer->Say(szCmd);
	}
}

// R5: 0x10068500
void cmdTestDeathWindow(PCHAR szCmd)
{
	if(pDeathWindow) {
		pDeathWindow->AddMessage("Pooper","Pooper333",0xFFFFFFFF,0xFFFFFFFF,1);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,5);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,15);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,14);
		pDeathWindow->AddMessage("Pooper","Pooper",0xFFFFFFFF,0xFFFFFFFF,2);
		pDeathWindow->AddMessage(0,"PooperPooperPooper0001",0xFFFFFFFF,0xFFFFFFFF,5);
		pDeathWindow->AddMessage(0,"Pooper",0xFFFFFFFF,0xFFFFFFFF,-1);
		pDeathWindow->AddMessage("Pooper","PooperPooperPooper0001",0xFFFFFFFF,0xFFFFFFFF,0);
	}
}

// R5: 0x100685E0
void cmdCameraTargetDebug(PCHAR szCmd)
{
	bShowCameraTarget = !bShowCameraTarget;
}

// R5: 0x100685F0
void cmdPageSize(PCHAR szCmd)
{
	int lines;
	if (strlen(szCmd) && (lines = atoi(szCmd)) >= 10 && lines <= 20) {
		if(pChatWindow) pChatWindow->SetPageSize(lines);
		if(pConfig) pConfig->SetIntVariable("pagesize", lines);
	} else {
		if(pChatWindow) pChatWindow->AddDebugMessage("pagesize [10-20] (lines)");
	}
}

// R5: 0x10068670
void cmdFontSize(PCHAR szCmd)
{
	int size;
	if(strlen(szCmd) && (size = atoi(szCmd)) >= -3 && size <= 5) {
		if(pConfig) pConfig->SetIntVariable("fontsize", size);
		if(pDefaultFont) pDefaultFont->CreateFonts();
		if(pChatWindow) {
			pChatWindow->CreateFonts();
			pChatWindow->FUNC_10067200();
		}
		if(pDeathWindow) pDeathWindow->CreateFonts();
		RefreshApplicationUiFonts();
	} else {
		if(pChatWindow) pChatWindow->AddDebugMessage("Valid fontsize: -3 to 5");
	}
}

// R5: 0x10068720
void cmdNameTagStatus(PCHAR szCmd)
{
	if(pNetGame) pNetGame->TogglePlayerStatus();
}

// R5: 0x10068730
void cmdTimestamp(PCHAR szCmd)
{
	if(pChatWindow) {
		if(pChatWindow->field_C) {
			pChatWindow->field_C = 0;
			pChatWindow->field_63DA = 1;
			if(pConfig) pConfig->SetIntVariable("timestamp", 0);
		} else {
			pChatWindow->field_C = 1;
			pChatWindow->field_63DA = 1;
			if(pConfig) pConfig->SetIntVariable("timestamp", 1);
		}
	}
}

// R5: 0x10068790
void cmdAudioMsg(PCHAR szCmd)
{
	if(pConfig->GetIntVariable("audiomsgoff") == 1) {
		pConfig->SetIntVariable("audiomsgoff", 0);
		if(pChatWindow) pChatWindow->AddInfoMessage("Audio messages: On");
	} else {
		pConfig->SetIntVariable("audiomsgoff", 1);
		if(pChatWindow) pChatWindow->AddInfoMessage("Audio messages: Off");
	}
}

// R5: 0x10068800
void cmdLogUrls(PCHAR szCmd)
{
	if(pConfig->GetIntVariable("logurls") == 1) {
		pConfig->SetIntVariable("logurls", 0);
		if(pChatWindow) pChatWindow->AddInfoMessage("URL messages: Off");
	} else {
		pConfig->SetIntVariable("logurls", 1);
		if(pChatWindow) pChatWindow->AddInfoMessage("URL messages: On");
	}
}

// R5: 0x10068870
void cmdHudScaleFix(PCHAR szCmd)
{
	if(bHudScaleFix)
	{
		bHudScaleFix = false;
		pConfig->SetIntVariable("nohudscalefix", 1);
	}
	else
	{
		bHudScaleFix = true;
		pConfig->SetIntVariable("nohudscalefix", 0);
	}
}

// R5: 0x100688B0
void cmdMem(PCHAR szCmd)
{
	pChatWindow->AddDebugMessage("Memory: %u",*(DWORD *)0x8A5A80);
}

// R5: 0x100688D0
void cmdSetFrameLimit(PCHAR szCmd)
{
	if(strlen(szCmd)) {
		DWORD limit = atoi(szCmd);
		if(limit >= 20 && limit <= 90) {
			pGame->SetFrameLimit(limit);
			if(pConfig) pConfig->SetIntVariable("fpslimit", limit);
			if(pChatWindow) pChatWindow->AddInfoMessage("-> Frame Limiter: %u", limit);
		} else {
			if(pChatWindow) pChatWindow->AddInfoMessage("-> Frame Limiter: valid amounts are 20-90", limit);
		}
	}
}

// R5: 0x10068960
void cmdHeadMove(PCHAR szCmd)
{
	if(pGame) {
		if(pGame->m_bHeadMove) {
			pGame->m_bHeadMove = FALSE;
			if(pConfig) pConfig->SetIntVariable("disableheadmove", 1);
			if(pChatWindow) pChatWindow->AddInfoMessage("-> Head movements disabled");
		} else {
			pGame->m_bHeadMove = TRUE;
			if(pConfig) pConfig->SetIntVariable("disableheadmove", 0);
			if(pChatWindow) pChatWindow->AddInfoMessage("-> Head movements enabled");
		}
	}
}

// R5: 0x100689E0
void cmdQuit(PCHAR szCmd)
{
	QuitGame();
}

// R5: 0x100689F0
void cmdCmpStat(PCHAR szCmd)
{
}

// R5: 0x10068A00
void cmdSavePos(PCHAR szCmd)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	FILE *fileOut;
	DWORD dwVehicleID;
	float fZAngle;

	char path[MAX_PATH];
	sprintf(path, "%s\\savedpositions.txt", GetSAMPPath());
	fileOut = fopen(path, "a");
	if(!fileOut) {
		pChatWindow->AddDebugMessage("I can't open the savepositions.txt file for append.");
		return;
	}

	// incar savepos
	if(pPlayer->IsInVehicle()) {
		VEHICLE_TYPE *pVehicle = pPlayer->GetGtaVehicle();
		dwVehicleID = GamePool_Vehicle_GetIndex(pVehicle);
		ScriptCommand(&r5CmdGetCarZAngle, dwVehicleID, &fZAngle);

		fprintf(fileOut, "AddStaticVehicle(%u,%.4f,%.4f,%.4f,%.4f,%u,%u); // %s\n",
			pVehicle->entity.nModelIndex, pVehicle->entity.mat->pos.X, pVehicle->entity.mat->pos.Y, pVehicle->entity.mat->pos.Z,
			fZAngle, pVehicle->byteColor1, pVehicle->byteColor2, szCmd);

		fclose(fileOut);
		if(pChatWindow) pChatWindow->AddInfoMessage("-> InCar position saved");
		return;
	}

	// onfoot savepos
	PED_TYPE *pActor = pPlayer->m_pPed;
	ScriptCommand(&r5CmdGetActorZAngle, pPlayer->m_dwGTAId, &fZAngle);

	fprintf(fileOut, "AddPlayerClass(%u,%.4f,%.4f,%.4f,%.4f,0,0,0,0,0,0); // %s\n", pPlayer->GetModelIndex(),
		pActor->entity.mat->pos.X, pActor->entity.mat->pos.Y, pActor->entity.mat->pos.Z, fZAngle, szCmd);

	fclose(fileOut);
	if(pChatWindow) pChatWindow->AddInfoMessage("-> OnFoot position saved");
}

// R5: 0x10068B80
void cmdRawSavePos(PCHAR szCmd)
{
	CPlayerPed *pPlayer = pGame->FindPlayerPed();
	FILE *fileOut;
	DWORD dwVehicleID;
	float fZAngle;

	if(pPlayer->IsInVehicle()) {
		fileOut = fopen("rawvehicles.txt", "a");
		if(!fileOut) {
			pChatWindow->AddDebugMessage("I can't open the rawvehicles.txt file for append.");
			return;
		}

		VEHICLE_TYPE *pVehicle = pPlayer->GetGtaVehicle();
		dwVehicleID = GamePool_Vehicle_GetIndex(pVehicle);
		ScriptCommand(&r5CmdGetCarZAngle, dwVehicleID, &fZAngle);

		fprintf(fileOut, "%u,%.4f,%.4f,%.4f,%.4f,%u,%u ; %s\n",
			pVehicle->entity.nModelIndex, pVehicle->entity.mat->pos.X, pVehicle->entity.mat->pos.Y, pVehicle->entity.mat->pos.Z,
			fZAngle, pVehicle->byteColor1, pVehicle->byteColor2, szCmd);

		fclose(fileOut);
		if(pChatWindow) pChatWindow->AddDebugMessage("-> InCar pos saved");
		return;
	}

	// onfoot savepos
	PED_TYPE *pActor = pPlayer->m_pPed;
	ScriptCommand(&r5CmdGetActorZAngle, pPlayer->m_dwGTAId, &fZAngle);

	char path[MAX_PATH];
	sprintf(path, "%s\\rawpositions.txt", GetSAMPPath());
	fileOut = fopen(path, "a");

	if(!fileOut) {
		pChatWindow->AddDebugMessage("I can't open the rawvehicles.txt file for append.");
		return;
	}

	fprintf(fileOut, "%.4f,%.4f,%.4f,%.4f ; %s\n", pActor->entity.mat->pos.X, pActor->entity.mat->pos.Y, pActor->entity.mat->pos.Z, fZAngle, szCmd);
	fclose(fileOut);
	if(pChatWindow) pChatWindow->AddDebugMessage("-> OnFoot pos saved");
}

// R5: 0x10068D00
void cmdPlayerSkin(PCHAR szCmd)
{
#ifndef _DEBUG
	if(!tSettings.bDebug) return;
#endif

	if(!strlen(szCmd)){
		pChatWindow->AddDebugMessage("Usage: player_skin (skin number).");
		return;
	}
	int iPlayerSkin = atoi(szCmd);

	if(pGame && pGame->IsGameLoaded())
	{
		CPlayerPed *pPlayer = pGame->FindPlayerPed();
		if(pPlayer)
		{
			pPlayer->SetModelIndex(iPlayerSkin);
		}
	}
}

// R5: 0x10068D70
void cmdCreateVehicle(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	if(!strlen(szCmd)){
		pChatWindow->AddDebugMessage("Usage: /v (vehicle id).");
		return;
	}
	int iVehicleType = atoi(szCmd);

	if(pGame && pGame->IsGameLoaded())
	{
		pGame->RequestModel(iVehicleType);
		pGame->LoadRequestedModels();

		CPlayerPed *pPlayer = pGame->FindPlayerPed();
		if(pPlayer) 
		{
			MATRIX4X4 matPlayer;
			pPlayer->GetMatrix(&matPlayer);
			CVehicle *pTestVehicle = pGame->NewVehicle(iVehicleType,
				(matPlayer.pos.X - 5.0f), (matPlayer.pos.Y - 5.0f),
				matPlayer.pos.Z + 1.0f, 0.0f, 0);

			if(pTestVehicle) {
				pTestVehicle->Add();
			}
			return;
		}
		else {
			pChatWindow->AddDebugMessage("I couldn't find the player actor.");
			return;
		}
	}
	else {
		pChatWindow->AddDebugMessage("game not loaded.");
	}
}

// R5: 0x10068EB0
void cmdSelectVehicle(PCHAR szCmd)
{
	if(!tSettings.bDebug) return;

	GameDebugEntity(0, 0, 10);
}

// R5: 0x10068ED0
void cmdSetWeather(PCHAR szCmd)
{
	if(!strlen(szCmd)){
		pChatWindow->AddDebugMessage("Usage: set_weather (weather number)");
		return;
	}
	int iWeatherID = atoi(szCmd);
	if(pGame) pGame->SetWorldWeather(iWeatherID);
}

// R5: 0x10068F20
void cmdSetTime(PCHAR szCmd)
{
	if(!strlen(szCmd)){
		pChatWindow->AddDebugMessage("Usage: set_time (hour) (minute)");
		return;
	}

	int iHour = 0, iMinute = 0;
	sscanf(szCmd, "%d%d", &iHour, &iMinute);

	if((iHour >= 0 && iHour <= 23) && (iMinute >= 0 && iMinute <= 59)) {
		if(pGame) pGame->SetWorldTime(iHour, iMinute);
	}
	else {
		pChatWindow->AddDebugMessage("Invalid Time. Use /set_time (hour 0-23) (minute 0-59)");
	}
}

// R5: 0x10068FD0
void cmdShowInterior(PCHAR szCmd)
{
	DWORD dwRet;
	ScriptCommand(&get_active_interior, &dwRet);
	pChatWindow->AddDebugMessage("Current Interior: %u", dwRet);
}

// R5: 0x10069000
void cmdToggleObjectLight(PCHAR szCmd)
{
	r5ObjectLightingToggle = !r5ObjectLightingToggle;
}

// R5: 0x10069020
void cmdDebugLabels(PCHAR szCmd)
{
	bShowDebugLabels = !bShowDebugLabels;
}

// R5: 0x10069030
void cmdRcon(PCHAR szCmd)
{
	if(!szCmd || !pNetGame) return;

	BYTE bytePacketId = ID_RCON_COMMAND;
	RakNet::BitStream bsCommand;
	bsCommand.Write(bytePacketId);
	DWORD dwCmdLen = (DWORD)strlen(szCmd);
	bsCommand.Write(dwCmdLen);
	bsCommand.Write(szCmd, dwCmdLen);
	pNetGame->GetRakClient()->Send(&bsCommand, HIGH_PRIORITY, RELIABLE, 0);
}

//----------------------------------------------------

// R5: 0x10069110
void SetupCommands()
{
	// RELEASE COMMANDS
	pCmdWindow->AddDefaultCmdProc(cmdDefaultCmdProc);
	pCmdWindow->AddCmdProc("quit",cmdQuit);
	pCmdWindow->AddCmdProc("q",cmdQuit);
	pCmdWindow->AddCmdProc("save",cmdSavePos);
	pCmdWindow->AddCmdProc("rs",cmdRawSavePos);
	pCmdWindow->AddCmdProc("rcon",cmdRcon);
	pCmdWindow->AddCmdProc("mem",cmdMem);
	pCmdWindow->AddCmdProc("fpslimit",cmdSetFrameLimit);
	pCmdWindow->AddCmdProc("pagesize",cmdPageSize);
	pCmdWindow->AddCmdProc("fontsize",cmdFontSize);
	pCmdWindow->AddCmdProc("nametagstatus",cmdNameTagStatus);
	pCmdWindow->AddCmdProc("timestamp",cmdTimestamp);
	pCmdWindow->AddCmdProc("headmove",cmdHeadMove);
	pCmdWindow->AddCmdProc("hudscalefix",cmdHudScaleFix);
	pCmdWindow->AddCmdProc("testdw",cmdTestDeathWindow);

#ifndef _DEBUG
	if(tSettings.bDebug)
	{
#endif
	pCmdWindow->AddCmdProc("vsel",cmdSelectVehicle);
	pCmdWindow->AddCmdProc("v",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("vehicle",cmdCreateVehicle);
	pCmdWindow->AddCmdProc("player_skin",cmdPlayerSkin);
	pCmdWindow->AddCmdProc("set_weather",cmdSetWeather);
	pCmdWindow->AddCmdProc("set_time",cmdSetTime);
#ifndef _DEBUG
	}
#endif

	pCmdWindow->AddCmdProc("interior",cmdShowInterior);
	pCmdWindow->AddCmdProc("togobjlight",cmdToggleObjectLight);
	pCmdWindow->AddCmdProc("cmpstat",cmdCmpStat);
	pCmdWindow->AddCmdProc("dl",cmdDebugLabels);
	pCmdWindow->AddCmdProc("ctd",cmdCameraTargetDebug);
	pCmdWindow->AddCmdProc("audiomsg",cmdAudioMsg);
	pCmdWindow->AddCmdProc("logurls",cmdLogUrls);
}
