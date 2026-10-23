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

