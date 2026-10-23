
#include "../main.h"
#include "rwstuff.h"

extern int iGtaVersion;

//----------------------------------------------------------

// R5: 0x100B16D0
RwRaster* RwRasterCreate(int width, int height, int depth, int flags)
{
	RwRaster* pRaster = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7FB270 : 0x7FB230;

	_asm push flags
	_asm push depth
	_asm push height
	_asm push width
	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pRaster, eax
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx

	return pRaster;
}

// R5: 0x100B1720
RwTexture* RwTextureCreate(RwRaster *raster)
{
	RwTexture* pTexture = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F3800 : 0x7F37C0;

	_asm push raster
	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pTexture, eax
	_asm pop edx

	return pTexture;
}

// R5: 0x100B1760
void RwTextureDestroy(RwTexture *texture)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F3860 : 0x7F3820;

	_asm push texture
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1790
RwFrame* RwFrameCreate()
{
	RwFrame* pFrame = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F0450 : 0x7F0410;

	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pFrame, eax

	return pFrame;
}

// R5: 0x100B17D0
void RwFrameDestroy(RwFrame *frame)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F05E0 : 0x7F05A0;

	_asm push frame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1800
void RpAtomicDestroy(RpAtomic *atomic)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x749E10 : 0x749DC0;

	_asm push atomic
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1830
RwCamera* RwCameraCreate()
{
	RwCamera* pCamera = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE530 : 0x7EE4F0;

	_asm mov edx, dwFunc
	_asm call edx
	_asm mov pCamera, eax

	return pCamera;
}

// R5: 0x100B1870
void RwRasterDestroy(RwRaster *raster)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7FB060 : 0x7FB020;

	_asm push raster
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx

}

// R5: 0x100B18A0
void CShadowCamera_Create(int iRasterSize)
{
	_asm push iRasterSize
	_asm mov edx, 0x705B60
	_asm call edx
	_asm pop edx
}

// R5: 0x100B18B0
void RpWorldAddCamera(RwCamera *camera)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if(!dwWorld) return;

	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x750F70 : 0x750F20;

	_asm push camera
	_asm push dwWorld
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B18F0
void RwCameraClear(RwCamera *camera, RwRGBA *color, int clearMode)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE380 : 0x7EE340;

	_asm push clearMode
	_asm push color
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1920
void RwCameraBeginUpdate(RwCamera *camera)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE1D0 : 0x7EE190;

	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1950
void RwCameraEndUpdate(RwCamera *camera)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE1C0 : 0x7EE180;

	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1980
void RwObjectHasFrameSetFrame(RwCamera *camera, RwFrame *frame)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x804F30 : 0x804EF0l;

	_asm push frame
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B19B0
void SetCameraFrameAndZBuffer(RwCamera *camera, RwRaster *frameBuffer, RwRaster *zBuffer)
{
	_asm mov ebx, camera

	_asm mov edx, frameBuffer
	_asm mov [ebx+0x60], edx

	_asm mov edx, zBuffer
	_asm mov [ebx+0x64], edx
}

// R5: 0x100B19D0
void RpClumpRender(RpClump *clump)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x749B70 : 0x749B20;

	_asm push clump
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1A00
void RpWorldAddClump(RpClump *clump)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if (!dwWorld) return;

	_asm push clump
	_asm push dwWorld
	_asm mov edx, 0x751300
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1A30
void RpWorldRemoveClump(RpClump *clump)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if (!dwWorld) return;

	_asm push clump
	_asm push dwWorld
	_asm mov edx, 0x751460
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1A60
void RpWorldRender()
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if (!dwWorld) return;

	_asm push dwWorld
	_asm mov edx, 0x74F570
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1A80
void RwCameraSetClipPlane(RwCamera *camera, float farClip, float nearClip)
{
	DWORD dwRwCameraSetFarClipPlane;
	DWORD dwRwCameraSetNearClipPlane;

	if(iGtaVersion == GTASA_VERSION_USA10)
	{
		dwRwCameraSetFarClipPlane = 0x7EE2A0;
		dwRwCameraSetNearClipPlane = 0x7EE1D0;
	}
	else
	{
		dwRwCameraSetFarClipPlane = 0x7EE2E0;
		dwRwCameraSetNearClipPlane = 0x7EE210;
	}

	_asm push farClip
	_asm push camera
	_asm mov edx, dwRwCameraSetFarClipPlane
	_asm call edx
	_asm pop edx
	_asm pop edx

	_asm push nearClip
	_asm push camera
	_asm mov edx, dwRwCameraSetNearClipPlane
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1AD0
void RwCameraSetViewWindow(RwCamera *camera, VECTOR2D *viewWindow)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE450 : 0x7EE410;

	_asm push viewWindow
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1B00
RpLight* RpLightCreate(int _type)
{
	RpLight* pLight = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x752160 : 0x752110;

	_asm push _type
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm mov pLight, eax

	return pLight;
}

// R5: 0x100B1B40
void RpClumpAddLight(RpClump *clump, RpLight *light)
{
	RpClump* pResult = NULL;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x74A540 : 0x74A4F0;

	_asm push light
	_asm push clump
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm mov pResult, eax
}

// R5: 0x100B1B70
void RpLightSetColor(RpLight *light, RwRGBAReal *color)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x751AE0 : 0x751A90;

	_asm push color
	_asm push light
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1BA0
void RpLightSetRadius(RpLight *light, float radius)
{
	DWORD dwFunc = 0x751A70;

	_asm push radius
	_asm push light
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1BC0
void RpWorldAddLight(RpLight *light)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if(!dwWorld) return;

	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x751960 : 0x751910;

	_asm push light
	_asm push dwWorld
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1C00
void RpWorldRemoveLight(RpLight *light)
{
	DWORD dwWorld = *(DWORD*)0xC17038;
	if(!dwWorld) return;

	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7519B0 : 0x751960;

	_asm push light
	_asm push dwWorld
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1C40
void CVisibilityPlugins_SetRenderWareCamera(RwCamera *camera)
{
	_asm push camera
	_asm mov edx, 0x7328C0
	_asm call edx
	_asm pop edx
}

// R5: 0x100B1C50
RpClump * RpClumpClone(RpClump *clump)
{
	RpClump *pResult = NULL;

	if(!clump) return NULL;

	_asm push clump
	_asm mov edx, 0x749F70
	_asm call edx
	_asm pop edx
	_asm mov pResult, eax

	return pResult;
}

// R5: 0x100B1C80
int RpClumpDestroy(RpClump *clump)
{
	int bResult = 0;
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x74A360 : 0x74A310;

	_asm push clump
	_asm mov eax, dwFunc
	_asm call eax
	_asm mov bResult, eax
	_asm pop eax

	return bResult;
}

// R5: 0x100B1CC0
void RwCameraSetProjection(RwCamera *camera, int projection)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7EE3E0 : 0x7EE3A0;

	_asm push projection
	_asm push camera
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
}

// R5: 0x100B1CF0
void RwFrameTranslate(RwFrame *frame, VECTOR *v, int combine)
{
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F0E70 : 0x7F0E30;

	_asm push combine
	_asm push v
	_asm push frame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

VECTOR stru_10117384[3] = {
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
};

// R5: 0x100B1D20
void RwFrameRotate(RwFrame* frame, int axis, float angle)
{
	VECTOR* pAxis = &stru_10117384[axis];
	DWORD dwFunc = (iGtaVersion != GTASA_VERSION_USA10) ? 0x7F1050 : 0x7F1010;

	_asm push 1
	_asm push angle
	_asm push pAxis
	_asm push frame
	_asm mov edx, dwFunc
	_asm call edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
	_asm pop edx
}

