
#include <stdio.h>
#include <windows.h>
#include "common.h"
#include "keystuff.h"
#include "address.h"

//-----------------------------------------------------------

GTA_CONTROLSET *pGcsInternalKeys = (GTA_CONTROLSET *)ADDR_KEYSTATES;
GTA_CONTROLSET GcsLocalPlayerKeys;
GTA_CONTROLSET GcsRemotePlayerKeys[PLAYER_PED_SLOTS];

BYTE	byteSaveDriveByLeft;
BYTE	byteSaveDriveByRight;
BYTE   *pbyteDriveByLeft =  (BYTE *)0xB6F1A4;
BYTE   *pbyteDriveByRight = (BYTE *)0xB6F1A5;

//-----------------------------------------------------------

// R5: 0x100A72A0
void GameKeyStatesInit()
{
	int x=0;
	memset(&GcsLocalPlayerKeys,0,sizeof(GTA_CONTROLSET));
	while(x!=PLAYER_PED_SLOTS) {
		memset(&GcsRemotePlayerKeys[x],0,sizeof(GTA_CONTROLSET));
		x++;
	}
}

//-----------------------------------------------------------

// R5: 0x100A72C0
void GameStoreLocalPlayerKeys()
{
	memcpy(&GcsLocalPlayerKeys,pGcsInternalKeys,sizeof(GTA_CONTROLSET));

	// save current driveby state
	byteSaveDriveByLeft = *pbyteDriveByLeft;
	byteSaveDriveByRight = *pbyteDriveByRight;
}

//-----------------------------------------------------------

// R5: 0x100A7300
void GameSetLocalPlayerKeys()
{
	memcpy(pGcsInternalKeys,&GcsLocalPlayerKeys,sizeof(GTA_CONTROLSET));

	// restore the driveby state also
	*pbyteDriveByLeft = byteSaveDriveByLeft;
	*pbyteDriveByRight = byteSaveDriveByRight;
}

//-----------------------------------------------------------

// R5: 0x100A7340
void GameStoreRemotePlayerKeys(int iPlayer, GTA_CONTROLSET * pGcsKeyStates)
{
	memcpy(&GcsRemotePlayerKeys[iPlayer],pGcsKeyStates,sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

// R5: 0x100A7360
void GameSetRemotePlayerKeys(int iPlayer)
{
	// Use this to sync between player and remote player.
	//memcpy(&GcsRemotePlayerKeys[iPlayer],&GcsLocalPlayerKeys,96);

	memcpy(pGcsInternalKeys,&GcsRemotePlayerKeys[iPlayer],sizeof(GTA_CONTROLSET));

	if(GcsRemotePlayerKeys[iPlayer].wKeys1[5]) { // lookl
		*pbyteDriveByLeft = 1;
	} else {
		*pbyteDriveByLeft = 0;
	}

	if(GcsRemotePlayerKeys[iPlayer].wKeys1[7]) { // lookr
		*pbyteDriveByRight = 1;
	} else {
		*pbyteDriveByRight = 0;
	}
}

//-----------------------------------------------------------

// R5: 0x100A73B0
GTA_CONTROLSET * GameGetInternalKeys()
{
	return pGcsInternalKeys;
}

//-----------------------------------------------------------

GTA_CONTROLSET * GameGetLocalPlayerKeys()
{
	return &GcsLocalPlayerKeys;
}

//-----------------------------------------------------------

// R5: 0x100A73D0
GTA_CONTROLSET * GameGetPlayerKeys(int iPlayer)
{
	return &GcsRemotePlayerKeys[iPlayer];
}

//-----------------------------------------------------------

// R5: 0x100A73E0
void GameResetPlayerKeys(int iPlayer)
{
	memset(&GcsRemotePlayerKeys[iPlayer],0,sizeof(GTA_CONTROLSET));
}

//-----------------------------------------------------------

void GameResetInternalKeys()
{
	GTA_CONTROLSET *pInternals = GameGetInternalKeys();
	memset((PVOID)pInternals,0,48 * sizeof(WORD));
}

//-----------------------------------------------------------
