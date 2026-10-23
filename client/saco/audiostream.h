#pragma once

#include <windows.h>

class CAudioStream // size=1
{
private:
	char field_0;

public:

	CAudioStream() {
		field_0 = 0;
	}

	BOOL Reset();
	void Stop();
	void ConstructInfo();
	void SyncProc();
	void Process();
	void Play();
	void ControlGameRadio();
	void DrawInfo();
};
