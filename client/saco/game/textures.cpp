
#include <windows.h>
#include "textures.h"

//-----------------------------------------------------------

// R5: 0x100B3880
int FindTextureSlot(char *szName)
{
	int iRetVal;

	_asm push szName
	_asm mov edx, 0x731850
	_asm call edx
	_asm pop edx
	_asm mov iRetVal, eax

	return iRetVal;
}

//-----------------------------------------------------------

// R5: 0x100B38A0
int AddTextureSlot(char *szName)
{
	int iResult;

	_asm push szName
	_asm mov edx, 0x731C80
	_asm call edx
	_asm pop edx
	_asm mov iResult, eax

	return iResult;
}

//-----------------------------------------------------------

// R5: 0x100B38C0
int LoadTexture(int iIndex, char *szName)
{
	BYTE byteResult = 0;

	_asm xor eax, eax
	_asm push szName
	_asm push iIndex
	_asm mov edx, 0x7320B0
	_asm call edx
	_asm mov byteResult, al
	_asm pop edx
	_asm pop edx

	return byteResult;
}

//-----------------------------------------------------------

// R5: 0x100B38F0
void AddTextureRef(int iIndex)
{
	_asm push iIndex
	_asm mov edx, 0x731A00
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

// R5: 0x100B3900
void RemoveTextureRef(int iIndex)
{
	_asm push iIndex
	_asm mov edx, 0x731A30
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

// R5: 0x100B3910
void PushCurrentTexture()
{
	_asm mov edx, 0x7316A0
	_asm call edx
}

//-----------------------------------------------------------

// R5: 0x100B3920
void PopCurrentTexture()
{
	_asm mov edx, 0x7316B0
	_asm call edx
}

//-----------------------------------------------------------

// R5: 0x100B3930
void SetCurrentTexture(int iIndex)
{
	_asm push iIndex
	_asm mov edx, 0x7319C0
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

// R5: 0x100B3940
DWORD ReloadTexture(char *szName)
{
	DWORD dwThisTexture = 0;

	_asm push szName
	_asm lea ecx, dwThisTexture
	_asm mov edx, 0x727270
	_asm call edx

	return dwThisTexture;
}

//-----------------------------------------------------------

// R5: 0x100B3960
void DestroyTexture(DWORD dwTexture)
{
	_asm mov eax, dwTexture
	_asm mov dwTexture, eax
	_asm lea ecx, dwTexture
	_asm mov edx, 0x7281E0
	_asm call edx
}

//-----------------------------------------------------------

// R5: 0x100B3980
void RemoveTextureSlotByName(char *szName)
{
	int iIndex = FindTextureSlot(szName);

	if(iIndex == -1) return;

	_asm push iIndex
	_asm mov edx, 0x731CD0
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

// R5: 0x100B39B0
void RemoveTexture(int iIndex)
{
	if(iIndex == -1) return;

	_asm push iIndex
	_asm mov edx, 0x731E90
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

// R5: 0x100B39D0
void RemoveTextureSlot(int iIndex)
{
	if(iIndex == -1) return;

	_asm push iIndex
	_asm mov edx, 0x731CD0
	_asm call edx
	_asm pop edx
}

//-----------------------------------------------------------

// R5: 0x100B39F0
int GetTextureNumRefs(int iIndex)
{
	int iRetVal = -1;

	if(iIndex == -1) return -1;

	_asm push iIndex
	_asm mov edx, 0x731AA0
	_asm call edx
	_asm mov iRetVal, eax
	_asm pop edx

	return iRetVal;
}

//-----------------------------------------------------------
// EOF
