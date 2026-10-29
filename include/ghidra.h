/* Ghidra's primitive types and helper macros for machine drafts in SA-MP R5 */
#ifndef GHIDRA_H
#define GHIDRA_H

typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined3;
typedef unsigned int undefined4;
typedef unsigned __int64 undefined8;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned short word;
typedef unsigned int uint;
typedef unsigned int uint3;
typedef unsigned long ulong;
typedef unsigned int dword;
typedef __int64 longlong;
typedef unsigned __int64 ulonglong;
typedef unsigned __int64 qword;
typedef double longdouble;
typedef double float10;
typedef int code(...);

#define CONCAT11(a, b) ((unsigned short)(((unsigned short)(unsigned char)(a) << 8) | (unsigned char)(b)))
#define CONCAT22(a, b) ((unsigned int)(((unsigned int)(unsigned short)(a) << 16) | (unsigned short)(b)))
#define CONCAT31(a, b) ((unsigned int)(((unsigned int)(a) << 8) | (unsigned char)(b)))
#define CONCAT13(a, b) ((unsigned int)(((unsigned int)(unsigned char)(a) << 24) | ((unsigned int)(b) & 0xffffff)))
#define CONCAT44(a, b) ((unsigned __int64)(((unsigned __int64)(unsigned int)(a) << 32) | (unsigned int)(b)))
#define SUB41(x, n) ((unsigned char)((unsigned int)(x) >> ((n) * 8)))
#define SUB42(x, n) ((unsigned short)((unsigned int)(x) >> ((n) * 8)))
#define SUB84(x, n) ((unsigned int)((unsigned __int64)(x) >> ((n) * 8)))
#define ZEXT14(x) ((unsigned int)(unsigned char)(x))
#define ZEXT24(x) ((unsigned int)(unsigned short)(x))
#define ZEXT48(x) ((unsigned __int64)(unsigned int)(x))
#define SEXT14(x) ((int)(signed char)(x))
#define SEXT24(x) ((int)(short)(x))
#define SEXT48(x) ((__int64)(int)(x))

#endif
