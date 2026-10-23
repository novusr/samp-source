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

