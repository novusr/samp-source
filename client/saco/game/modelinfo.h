#pragma once

#include <windows.h>

BYTE *__stdcall GetModelInfo(int model);
int __stdcall ModelInfoLoaded(UINT model);
void __stdcall SetCustomModelReferenceCount(int model, int count);
