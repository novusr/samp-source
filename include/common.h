/* Common header for SA-MP 0.3.7 R5 reverse-engineering matching
 * Defines shared types, math structs, GTA structures, DirectX types, and forward declarations.
 */
#ifndef R5_COMMON_H
#define R5_COMMON_H

#include "ghidra.h"

#pragma pack(push, 1)

// Basic Win32 types if not included from windows.h
#ifndef _WINDEF_
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef int                 INT;
typedef unsigned int        UINT;
typedef void*               HWND;
typedef void*               HINSTANCE;
typedef void*               HDC;
typedef void*               HMONITOR;
typedef void*               HMENU;
typedef void*               HKL;
typedef void*               HIMC;
typedef void*               HIMCC;

typedef struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
} RECT, *PRECT, *LPRECT;

typedef struct tagPOINT {
    long x;
    long y;
} POINT, *PPOINT, *LPPOINT;

typedef struct tagSIZE {
    long cx;
    long cy;
} SIZE, *PSIZE, *LPSIZE;
#endif

// SA-MP Constants
#define MAX_PLAYERS         1004
#define MAX_ACTORS          1000
#define MAX_VEHICLES        2000
#define MAX_OBJECTS         1000
#define MAX_MENUS           128
#define MAX_TEXT_DRAWS      2304
#define MAX_GANG_ZONES      1024
#define MAX_LABELS          2048

typedef unsigned short VEHICLEID;
typedef unsigned short ACTORID;
typedef unsigned short PLAYERID;

// Common Math & Vector structures
typedef struct _VECTOR {
    float X, Y, Z;
} VECTOR, *PVECTOR;

typedef struct _VECTOR2D {
    float X, Y;
} VECTOR2D, *PVECTOR2D;

typedef struct _FRECT {
    float l, t, r, b;
} FRECT, *PFRECT;

typedef struct _MATRIX4X4 {
    VECTOR right;
    DWORD  flags;
    VECTOR up;
    float  pad_u;
    VECTOR at;
    float  pad_a;
    VECTOR pos;
    float  pad_p;
} MATRIX4X4, *PMATRIX4X4;

typedef struct _RGBA {
    unsigned char r, g, b, a;
} RGBA, *PRGBA;

typedef struct _CAMERA_AIM {
    VECTOR pos1;
    VECTOR pos2;
    VECTOR pos3;
    VECTOR pos4;
} CAMERA_AIM, *PCAMERA_AIM;

typedef struct _GTA_CONTROLSET {
    unsigned short wKeys1[2];
    unsigned short wKeys2[2];
    unsigned short wKeys3[2];
} GTA_CONTROLSET, *PGTA_CONTROLSET;

// Core GTA Structures
struct _PED_TYPE {
    void* vtable;
    char gap0[0x10];
    struct _MATRIX4X4* mat;
    void* pRwObject;
    char gap1[0x44];
    unsigned char byteState;
    char gap2[0x4ec];
    unsigned long dwStateFlags;
    char gap3[0x8];
    float fHealth;
    float fArmour;
    char gap4[0x4];
    float fRotation;
    char gap5[0x18];
    void* pGtaVehicle;
    char gap6[0x114];
    struct _PED_TYPE* pContactEntity;
    char gap7[0x8];
    struct _VEHICLE_TYPE* pContactVehicle;
};

struct _VEHICLE_TYPE {
    void* vtable;
    char gap0[0x10];
    struct _MATRIX4X4* mat;
    void* pRwObject;
    char gap0a[0x6];
    unsigned short nModelIndex;
    char gap1[0x43c];
    struct _PED_TYPE* pDriver;
    char gap2[0xb0];
    unsigned char byteHorn2;
    char gap3[0x40];
    float fHealth;
    char gap4[0x60];
    struct _VEHICLE_TYPE* pTrailer;
};

struct _ENTITY_TYPE {
    void* vtable;
    char gap0[0x10];
    struct _MATRIX4X4* mat;
    void* pRwObject;
};

struct _BUILDING_TYPE {
    void* vtable;
    char gap0[0x10];
    struct _MATRIX4X4* mat;
    void* pRwObject;
};

struct _DUMMY_TYPE {
    void* vtable;
    char gap0[0x10];
    struct _MATRIX4X4* mat;
    void* pRwObject;
};

struct _OBJECT_TYPE {
    void* vtable;
    char gap0[0x10];
    struct _MATRIX4X4* mat;
    void* pRwObject;
};

// Forward Declarations of DirectX & DXUT classes
struct IDirect3D9;
struct IDirect3DDevice9;
struct ID3DXSprite;
struct ID3DXFont;
class CDXUTDialog;
class CDXUTControl;
class CDXUTElement;
class CDXUTButton;
class CDXUTStatic;
class CDXUTCheckBox;
class CDXUTRadioButton;
class CDXUTListBox;
class CDXUTScrollBar;
class CDXUTSlider;
class CDXUTComboBox;
class CDXUTEditBox;
class CDXUTIMEEditBox;
class CDXUTTimer;
class CD3DArcBall;
class CD3DEnumeration;
class CDXUTResourceCache;
class DXUTState;

// Forward Declarations of SA-MP Game & Entity classes
class CEntity;
class CGame;
class CPlayerPed;
class CActorPed;
class CVehicle;
class CNetGame;
class CLocalPlayer;
class CRemotePlayer;
class CMenu;
class CCmdWindow;
class CChatWindow;
class CDeathWindow;
class CSpawnScreen;
class CFontRender;
class CUniBuffer;
class CConfig;
class CHttpClient;
class TEABlockEncryptor;
class CSHA1;

// Forward Declarations of Pools
class PlayerPool;
class CActorPool;
class ActorPool;
class CVehiclePool;
class VehiclePool;
class CObjectPool;
class ObjectPool;
class CPickupPool;
class PickupPool;
class CMenuPool;
class MenuPool;
class CTextDrawPool;
class TextDrawPool;
class CGangZonePool;
class GangZonePool;
class CLabelPool;
class LabelPool;

// Forward Declarations of Tasks
class CTask;
class CTaskJetpack;
class CTaskTakeDamageFall;
class CTaskGoggles;
class CTaskGoToPoint;
class CTaskKillPedOnFootArmed;

// RakNet
namespace RakNet {
    class BitStream;
}
struct PlayerID;
struct Packet;
struct RPCParameters;

#pragma pack(pop)

#endif // R5_COMMON_H
