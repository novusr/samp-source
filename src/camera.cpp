// External objects are resolved by the matching verifier, never inferred
// from bytes that happen to differ. Their backing values are checked in R5.
extern "C" {
extern float* cameraZoom;
extern float* aspectRatio;
extern float localZoom;
extern float localAspect;
extern float remoteZoom[210];
extern float remoteAspect[210];

void __stdcall StoreLocalCamera() {
    localZoom = *cameraZoom;
    localAspect = *aspectRatio;
}
void __stdcall RestoreLocalCamera() {
    *cameraZoom = localZoom;
    *aspectRatio = localAspect;
}
void __stdcall SetPlayerCamera(unsigned char id, float zoom, float aspect) {
    remoteZoom[id] = zoom;
    remoteAspect[id] = aspect;
}
float __stdcall GetLocalAspect() {
    return *aspectRatio - 1.0f;
}
float __stdcall GetLocalZoom() {
    return (*cameraZoom - 35.0f) / 35.0f;
}
void __stdcall SetRemoteCamera(unsigned char id) {
    float zoom = remoteZoom[id] * 35.0f;
    zoom += 35.0f;
    *cameraZoom = zoom;
    *aspectRatio = remoteAspect[id] + 1.0f;
}

double __cdecl sqrt(double);
double __cdecl atan2(double, double);
#pragma intrinsic(sqrt, atan2)

float _sqrtf(float x) {
    return (float)sqrt((double)x);
}

float _atan2f(float y, float x) {
    return (float)atan2((double)y, (double)x);
}
}

#pragma pack(push, 1)
struct _VECTOR {
    float X, Y, Z;
};
#pragma pack(pop)

#define _CAMERA_DEFS_
struct _MATRIX4X4 {
    _VECTOR right;
    unsigned long flags;
    _VECTOR up;
    float pad_u;
    _VECTOR at;
    float pad_a;
    _VECTOR pos;
    float pad_p;
};

class CEntity;

struct SCRIPT_COMMAND;
int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);
extern const SCRIPT_COMMAND restore_camera_to_user_defined;
extern const SCRIPT_COMMAND set_camera_position;
extern const SCRIPT_COMMAND restore_camera_jumpcut;
extern const SCRIPT_COMMAND lock_camera_position;
extern const SCRIPT_COMMAND set_camera_behind_player;
extern const SCRIPT_COMMAND point_camera;

class CCamera {
public:
    CEntity* m_pEntity;
    _MATRIX4X4* m_matPos;

    void GetMatrix(_MATRIX4X4* pMatrix);
    void SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ);
    void Restore();
    void Attach(int unk1, int unk2, int unk3);
    void SetBehindPlayer();
    void LookAtPoint(float fX, float fY, float fZ, int iType);
    float GetDistanceFromPoint(_VECTOR* vecPos);
};

struct D3DXVECTOR3 {
    float x, y, z;
};

struct D3DXMATRIX {
    float m[4][4];
};

extern "C" D3DXMATRIX* __stdcall D3DXMatrixPerspectiveFovLH(D3DXMATRIX *pOut, float fovy, float Aspect, float zn, float zf);

struct POINT {
    long x, y;
};

struct D3DXVECTOR2 {
    float x, y;
};

struct RECT {
    long left;
    long top;
    long right;
    long bottom;
};

#pragma pack(push, 1)
class CBaseCamera {
public:
    CBaseCamera();
    virtual long HandleMessages(void* hWnd, unsigned int uMsg, unsigned int wParam, long lParam);
    virtual void FrameMove(float fElapsedTime) = 0;
    virtual void Reset();
    virtual void SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt);
    virtual void SetProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane);
    virtual void SetDragRect(RECT& rc);

    D3DXMATRIX m_mView;
    D3DXMATRIX m_mProj;
    unsigned char m_aKeys[8];
    POINT m_ptLastMousePosition;
    bool m_bMouseLButtonDown;
    bool m_bMouseMButtonDown;
    bool m_bMouseRButtonDown;
    int m_nCurrentButtonMask;
    int m_nMouseWheelDelta;
    D3DXVECTOR2 m_vMouseDelta;
    float m_fFramesToSmoothMouseData;
    D3DXVECTOR3 m_vDefaultEye;
    D3DXVECTOR3 m_vDefaultLookAt;
    D3DXVECTOR3 m_vEye;
    D3DXVECTOR3 m_vLookAt;
    float m_fCameraYawAngle;
    float m_fCameraPitchAngle;
    RECT m_rcDrag;
    D3DXVECTOR3 m_vVelocity;
    bool m_bMovementDrag;
    D3DXVECTOR3 m_vVelocityDrag;
    float m_fDragTimer;
    float m_fTotalDragTimeToZero;
    D3DXVECTOR2 m_vRotVelocity;
    float m_fFOV;
    float m_fAspect;
    float m_fNearPlane;
    float m_fFarPlane;
    float m_fRotationScaler;
    float m_fMoveScaler;
    bool m_bInvertPitch;
    bool m_bEnablePositionMovement;
    bool m_bEnableYAxisMovement;
    bool m_bClipToBoundary;
    D3DXVECTOR3 m_vMinBoundary;
    D3DXVECTOR3 m_vMaxBoundary;
    bool m_bResetCursorAfterMove;
};

class CFirstPersonCamera : public CBaseCamera {
public:
    CFirstPersonCamera();
    virtual void FrameMove(float fElapsedTime);

    D3DXMATRIX m_mCameraWorld;
    int m_nActiveButtonMask;
};
#pragma pack(pop)

float FloatDifference(float f1, float f2) {
    return f1 - f2;
}

float FloatOffset(float f1, float f2) {
    if (f1 >= f2) return f1 - f2;
    else return f2 - f1;
}

float __stdcall SquaredDistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2) {
    float fSX = (x1 - x2) * (x1 - x2);
    float fSY = (y1 - y2) * (y1 - y2);
    return fSX + fSY;
}

float __stdcall DistanceBetweenHorizontalPoints(float x1, float y1, float x2, float y2) {
    float fSX = (x1 - x2) * (x1 - x2);
    float fSY = (y1 - y2) * (y1 - y2);
    return (float)sqrt((double)(fSX + fSY));
}

float __stdcall DistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2) {
    float fSX = (x1 - x2) * (x1 - x2);
    float fSY = (y1 - y2) * (y1 - y2);
    float fSZ = (z1 - z2) * (z1 - z2);
    return (float)sqrt((double)(fSX + fSY + fSZ));
}

float __stdcall SquaredDistanceBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2) {
    float fSX = (x1 - x2) * (x1 - x2);
    float fSY = (y1 - y2) * (y1 - y2);
    float fSZ = (z1 - z2) * (z1 - z2);
    return fSX + fSY + fSZ;
}

float __stdcall SquaredDistanceBetweenPoints(_VECTOR* vec1, _VECTOR* vec2) {
    float fDZ = vec1->Z - vec2->Z;
    float fDY = vec1->Y - vec2->Y;
    float fDX = vec1->X - vec2->X;
    return (fDX * fDX) + (fDY * fDY) + (fDZ * fDZ);
}

int IsHexChar(char c)
{
	if (c >= '0' && c <= '9') return 1;
	if (c >= 'A' && c <= 'F') return 1;
	if (c >= 'a' && c <= 'f') return 1;
	return 0;
}

int IsHexChar(unsigned short c)
{
	if (c >= '0' && c <= '9') return 1;
	if (c >= 'A' && c <= 'F') return 1;
	if (c >= 'a' && c <= 'f') return 1;
	return 0;
}

// Matched ?IsNumeric@@YA_NPBD@Z (0x100b5aa0)
bool IsNumeric(const char* szString) {
    while (*szString) {
        if (*szString < '0' || *szString > '9') return false;
        szString++;
    }
    return true;
}

// Matched ?FilterInvalidChars@@YAXPAD@Z (0x100b5de0)
void FilterInvalidChars(char* szString) {
    if (*szString != 0) {
        do {
            unsigned char c = (unsigned char)*szString;
            if (c > 0x7f || c < 0x20) {
                *szString = 0x20;
            }
            szString++;
        } while (*szString);
    }
}

void Util_UrlUnencode(char *enc)
{
	char *write_pos = enc;

	while(*enc)
	{
		if(*enc=='%')
		{
			*write_pos = (*++enc>'/'&&*enc<':')?((*enc-('0'))<<4):((*enc-('7'))<<4);
			*write_pos |= (*++enc>'/'&&*enc<':')?(*enc-'0'):(*enc-'7');
		}
		else if (*enc=='+')
			*write_pos= ' ';
		else
			*write_pos= *enc;

		write_pos++; enc++;
	}
	*write_pos='\0';
}

void CCamera::Restore()
{
    m_pEntity = 0;
    ScriptCommand(&restore_camera_jumpcut);
}

inline char Util_toupper(char c) {
    return ((c > (char)0x60) && (c < (char)0x7b)) ? c - 0x20 : c;
}


char* Util_stristr(const char* String, const char* Pattern) {
    char *pptri, *sptri, *starti;

    for (starti = (char*)String; *starti != '\0'; starti++) {
        for (; ((*starti != '\0') && (Util_toupper(*starti) != Util_toupper(*Pattern))); starti++);

        pptri = (char*)Pattern;
        sptri = (char*)starti;

        while (Util_toupper(*sptri) == Util_toupper(*pptri)) {
            sptri++;
            pptri++;

            if ('\0' == *pptri)
                return (starti);
        }
    }

    return 0;
}

int Util_wildcmp(char* wild, char* string) {
    char *cp, *mp;

    while ((*string) && (*wild != '*')) {
        if ((*wild != *string) && (*wild != '?')) {
            return 0;
        }
        wild++;
        string++;
    }

    while (*string) {
        if (*wild == '*') {
            if (!*++wild) {
                return 1;
            }
            mp = wild;
            cp = string + 1;
        } else if ((*wild == *string) || (*wild == '?')) {
            wild++;
            string++;
        } else {
            wild = mp;
            string = cp++;
        }
    }

    while (*wild == '*') {
        wild++;
    }

    return !*wild;
}

int Util_strnicmp(const char* s1, const char* s2, unsigned int n) {
    if (n == 0) return 0;

    do {
        if (Util_toupper((unsigned char)*s1) != Util_toupper((unsigned char)*s2++))
            return (int)Util_toupper((unsigned char)*s1) - (int)Util_toupper((unsigned char)*--s2);
        if (*s1++ == 0)
            break;
    } while (--n != 0);

    return 0;
}

char* Util_itoa(int v, char* s, int r) {
    int i, neg = 0;
    char* p = s;
    char* q = s;

    if (r < 0 || r > 35) {
        *s = 0;
        return (s);
    }
    if (r == 0) r = 10;
    if (v == 0) {
        *p++ = '0';
        *p = 0;
        return (s);
    }
    if (v < 0) {
        neg = 1;
        v = -v;
    }
    while (v > 0) {
        i = v % r;
        if (i > 9) i += 7;
        *p++ = '0' + i;
        v /= r;
    }
    if (neg) *p++ = '-';
    *p-- = 0;
    q = s;
    while (p > q) {
        i = *q;
        *q++ = *p;
        *p-- = i;
    }
    return (s);
}

extern "C" void __cdecl free(void*);

namespace RakNet {
#pragma pack(push, 1)
class BitStream {
public:
    int numberOfBitsUsed;
    int numberOfBitsAllocated;
    int readOffset;
    unsigned char* data;
    bool copyData;
    unsigned char stackData[256];

    BitStream();
    ~BitStream();
    void Reset();
    void ResetReadPointer();
    void ResetWritePointer();
    bool ReadBit();
    void AlignWriteToByteBoundary();
    void AlignReadToByteBoundary();
    void SetData(unsigned char* input);
    void IgnoreBits(const int numberOfBits);
    void SetWriteOffset(const int offset);
    int GetWriteOffset() const;
    void SetNumberOfBitsAllocated(const unsigned int lengthInBits);
    void AssertStreamEmpty();
    void Write0();
    void Write1();

    void AddBitsAndReallocate(const int numberOfBitsToWrite);

private:
    bool DoEndianSwap() const;
    friend void RakNet_BitStream_Anchor();
};
#pragma pack(pop)
void RakNet_BitStream_Anchor();
}

void RakNet::RakNet_BitStream_Anchor() {
    bool (BitStream::*p)() const = &BitStream::DoEndianSwap;
}

int RakNet::BitStream::GetWriteOffset() const {
    return numberOfBitsUsed;
}

RakNet::BitStream::BitStream() {
    numberOfBitsUsed = 0;
    numberOfBitsAllocated = 256 * 8;
    readOffset = 0;
    data = (unsigned char*)stackData;
    copyData = true;
}

void RakNet::BitStream::SetNumberOfBitsAllocated(const unsigned int lengthInBits) {
    numberOfBitsAllocated = lengthInBits;
}

RakNet::BitStream::~BitStream() {
    if (copyData && numberOfBitsAllocated > 256 * 8)
        free(data);
}

void RakNet::BitStream::Reset() {
    numberOfBitsUsed = 0;
    readOffset = 0;
}

CFirstPersonCamera::CFirstPersonCamera() :
    m_nActiveButtonMask(0x07)
{
}

void CCamera::SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ) {
    ScriptCommand(&restore_camera_to_user_defined);
    m_pEntity = 0;
    ScriptCommand(&set_camera_position, fX, fY, fZ, fRotationX, fRotationY, fRotationZ);
}

void RakNet::BitStream::ResetReadPointer() {
    readOffset = 0;
}

void RakNet::BitStream::ResetWritePointer() {
    numberOfBitsUsed = 0;
}

bool RakNet::BitStream::ReadBit() {
    return (bool)(data[readOffset >> 3] & (0x80 >> (readOffset++ & 7)));
}

void RakNet::BitStream::AlignWriteToByteBoundary() {
    if (numberOfBitsUsed)
        numberOfBitsUsed += 8 - (((numberOfBitsUsed - 1) & 7) + 1);
}

void RakNet::BitStream::AlignReadToByteBoundary() {
    if (readOffset)
        readOffset += 8 - (((readOffset - 1) & 7) + 1);
}

void RakNet::BitStream::SetData(unsigned char* input) {
    data = input;
    copyData = false;
}

void RakNet::BitStream::IgnoreBits(const int numberOfBits) {
    readOffset += numberOfBits;
}

void RakNet::BitStream::SetWriteOffset(const int offset) {
    numberOfBitsUsed = offset;
}

bool RakNet::BitStream::DoEndianSwap() const {
    return false;
}

void RakNet::BitStream::AssertStreamEmpty() {
}

void RakNet::BitStream::Write0() {
    AddBitsAndReallocate(1);
    if ((numberOfBitsUsed & 7) == 0)
        data[numberOfBitsUsed >> 3] = 0;
    numberOfBitsUsed++;
}

void RakNet::BitStream::Write1() {
    AddBitsAndReallocate(1);
    int numberOfBitsMod8 = numberOfBitsUsed & 7;
    if (numberOfBitsMod8 == 0)
        data[numberOfBitsUsed >> 3] = 0x80;
    else
        data[numberOfBitsUsed >> 3] |= (0x80 >> numberOfBitsMod8);
    numberOfBitsUsed++;
}


// Matched ?GetMatrix@CCamera@@QAEXPAU_MATRIX4X4@@@Z (0x1009d460)
#ifndef _CAMERA_DEFS_
#define _CAMERA_DEFS_
struct _MATRIX4X4 {
    _VECTOR right;
    unsigned long flags;
    _VECTOR up;
    float pad_u;
    _VECTOR at;
    float pad_a;
    _VECTOR pos;
    float pad_p;
};

class CEntity;

struct SCRIPT_COMMAND;
int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);
extern const SCRIPT_COMMAND restore_camera_to_user_defined;
extern const SCRIPT_COMMAND set_camera_position;
extern const SCRIPT_COMMAND restore_camera_jumpcut;
extern const SCRIPT_COMMAND lock_camera_position;
extern const SCRIPT_COMMAND set_camera_behind_player;
extern const SCRIPT_COMMAND point_camera;

class CCamera {
public:
    CEntity* m_pEntity;
    _MATRIX4X4* m_matPos;

    void GetMatrix(_MATRIX4X4* pMatrix);
    void SetPosition(float fX, float fY, float fZ, float fRotationX, float fRotationY, float fRotationZ);
    void Restore();
    void Attach(int unk1, int unk2, int unk3);
    void SetBehindPlayer();
    void LookAtPoint(float fX, float fY, float fZ, int iType);
    float GetDistanceFromPoint(_VECTOR* vecPos);
};

struct D3DXVECTOR3 {
    float x, y, z;
};

struct D3DXMATRIX {
    float m[4][4];
};

extern "C" D3DXMATRIX* __stdcall D3DXMatrixPerspectiveFovLH(D3DXMATRIX *pOut, float fovy, float Aspect, float zn, float zf);

struct POINT {
    long x, y;
};

struct D3DXVECTOR2 {
    float x, y;
};

struct RECT {
    long left;
    long top;
    long right;
    long bottom;
};

#pragma pack(push, 1)
class CBaseCamera {
public:
    CBaseCamera();
    virtual long HandleMessages(void* hWnd, unsigned int uMsg, unsigned int wParam, long lParam);
    virtual void FrameMove(float fElapsedTime) = 0;
    virtual void Reset();
    virtual void SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt);
    virtual void SetProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane);
    virtual void SetDragRect(RECT& rc);

    D3DXMATRIX m_mView;
    D3DXMATRIX m_mProj;
    unsigned char m_aKeys[8];
    POINT m_ptLastMousePosition;
    bool m_bMouseLButtonDown;
    bool m_bMouseMButtonDown;
    bool m_bMouseRButtonDown;
    int m_nCurrentButtonMask;
    int m_nMouseWheelDelta;
    D3DXVECTOR2 m_vMouseDelta;
    float m_fFramesToSmoothMouseData;
    D3DXVECTOR3 m_vDefaultEye;
    D3DXVECTOR3 m_vDefaultLookAt;
    D3DXVECTOR3 m_vEye;
    D3DXVECTOR3 m_vLookAt;
    float m_fCameraYawAngle;
    float m_fCameraPitchAngle;
    RECT m_rcDrag;
    D3DXVECTOR3 m_vVelocity;
    bool m_bMovementDrag;
    D3DXVECTOR3 m_vVelocityDrag;
    float m_fDragTimer;
    float m_fTotalDragTimeToZero;
    D3DXVECTOR2 m_vRotVelocity;
    float m_fFOV;
    float m_fAspect;
    float m_fNearPlane;
    float m_fFarPlane;
    float m_fRotationScaler;
    float m_fMoveScaler;
    bool m_bInvertPitch;
    bool m_bEnablePositionMovement;
    bool m_bEnableYAxisMovement;
    bool m_bClipToBoundary;
    D3DXVECTOR3 m_vMinBoundary;
    D3DXVECTOR3 m_vMaxBoundary;
    bool m_bResetCursorAfterMove;
};

class CFirstPersonCamera : public CBaseCamera {
public:
    CFirstPersonCamera();
    virtual void FrameMove(float fElapsedTime);

    D3DXMATRIX m_mCameraWorld;
    int m_nActiveButtonMask;
};
#pragma pack(pop)
#endif

void CCamera::GetMatrix(_MATRIX4X4 *pMatrix)
{
    pMatrix->right.X = m_matPos->right.X;
    pMatrix->right.Y = m_matPos->right.Y;
    pMatrix->right.Z = m_matPos->right.Z;

    pMatrix->up.X = m_matPos->up.X;
    pMatrix->up.Y = m_matPos->up.Y;
    pMatrix->up.Z = m_matPos->up.Z;

    pMatrix->at.X = m_matPos->at.X;
    pMatrix->at.Y = m_matPos->at.Y;
    pMatrix->at.Z = m_matPos->at.Z;

    pMatrix->pos.X = m_matPos->pos.X;
    pMatrix->pos.Y = m_matPos->pos.Y;
    pMatrix->pos.Z = m_matPos->pos.Z;
}

typedef unsigned long uint32;

struct MD5Context {
	uint32 buf[4];
	uint32 bits[2];
	unsigned char in[64];
};

// Matched ?MD5Init@@YAXPAUMD5Context@@@Z (0x100630b0)
void MD5Init(struct MD5Context *ctx)
{
	ctx->buf[0] = 0x67452301;
	ctx->buf[1] = 0xefcdab89;
	ctx->buf[2] = 0x98badcfe;
	ctx->buf[3] = 0x10325476;

	ctx->bits[0] = 0;
	ctx->bits[1] = 0;
}

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

// Matched ?MD5Transform@@YAXQAKQBK@Z (0x100630e0)
void MD5Transform(uint32 buf[4], uint32 const in[16])
{
	register uint32 a, b, c, d;

	a = buf[0];
	b = buf[1];
	c = buf[2];
	d = buf[3];

	MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
	MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
	MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
	MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
	MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
	MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
	MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
	MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
	MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
	MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
	MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
	MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
	MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
	MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
	MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
	MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

	MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
	MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
	MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
	MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
	MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
	MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
	MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
	MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
	MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
	MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
	MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
	MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
	MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
	MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
	MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
	MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

	MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
	MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
	MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
	MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
	MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
	MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
	MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
	MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
	MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
	MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
	MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
	MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
	MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
	MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
	MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
	MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

	MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
	MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
	MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
	MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
	MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
	MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
	MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
	MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
	MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
	MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
	MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
	MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
	MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
	MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
	MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
	MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

namespace fhicl {

class SHA1
{
public:
	SHA1();
	virtual ~SHA1();

	void Reset();
	bool Result(unsigned *message_digest_array);

	void Input(const unsigned char *message_array, unsigned length);
	void Input(const char *message_array, unsigned length);
	void Input(unsigned char message_element);
	void Input(char message_element);
	SHA1& operator<<(const char *message_array);
	SHA1& operator<<(const unsigned char *message_array);
	SHA1& operator<<(const char message_element);
	SHA1& operator<<(const unsigned char message_element);

private:
	void ProcessMessageBlock();
	void PadMessage();
	inline unsigned CircularShift(int bits, unsigned word);

	unsigned H[5];
	unsigned Length_Low;
	unsigned Length_High;
	unsigned char Message_Block[64];
	int Message_Block_Index;
	bool Computed;
	bool Corrupted;
};

void SHA1::Reset()
{
	Length_Low          = 0;
	Length_High         = 0;
	Message_Block_Index = 0;

	H[0]        = 0x67452301;
	H[1]        = 0xEFCDAB89;
	H[2]        = 0x98BADCFE;
	H[3]        = 0x10325476;
	H[4]        = 0xC3D2E1F0;

	Computed    = false;
	Corrupted   = false;
}

unsigned SHA1::CircularShift(int bits, unsigned word)
{
	return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32-bits));
}

// Process one 512-bit message block according to SHA-1 standard.
void SHA1::ProcessMessageBlock()
{
	const unsigned K[] =	{
								0x5A827999,
								0x6ED9EBA1,
								0x8F1BBCDC,
								0xCA62C1D6
							};
	int			t;
	unsigned	temp;
	unsigned	W[80];
	unsigned	A, B, C, D, E;

	for(t = 0; t < 16; t++)
	{
		W[t] = ((unsigned) Message_Block[t * 4]) << 24;
		W[t] |= ((unsigned) Message_Block[t * 4 + 1]) << 16;
		W[t] |= ((unsigned) Message_Block[t * 4 + 2]) << 8;
		W[t] |= ((unsigned) Message_Block[t * 4 + 3]);
	}

	for(t = 16; t < 80; t++)
	{
		W[t] = CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
	}

	A = H[0];
	B = H[1];
	C = H[2];
	D = H[3];
	E = H[4];

	for(t = 0; t < 20; t++)
	{
		temp = CircularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = CircularShift(30,B);
		B = A;
		A = temp;
	}

	for(t = 20; t < 40; t++)
	{
		temp = CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = CircularShift(30,B);
		B = A;
		A = temp;
	}

	for(t = 40; t < 60; t++)
	{
		temp = CircularShift(5,A) +
			((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = CircularShift(30,B);
		B = A;
		A = temp;
	}

	for(t = 60; t < 80; t++)
	{
		temp = CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = CircularShift(30,B);
		B = A;
		A = temp;
	}

	H[0] = (H[0] + A) & 0xFFFFFFFF;
	H[1] = (H[1] + B) & 0xFFFFFFFF;
	H[2] = (H[2] + C) & 0xFFFFFFFF;
	H[3] = (H[3] + D) & 0xFFFFFFFF;
	H[4] = (H[4] + E) & 0xFFFFFFFF;

	Message_Block_Index = 0;
}

} // namespace fhicl

