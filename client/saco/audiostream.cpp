#include "main.h"
#include "bass.h"

int DAT_1012e7b4 = 0;
extern CGame *pGame;
extern CConfig *pConfig;

struct AudioStreamGameView {
	CAudio *pAudio;
};

// R5: 0x10066480
BOOL CAudioStream::Reset()
{
	DAT_1012e7b4 = 0;
	BASS_Free();
	if (BASS_Init(-1, 44100, 0, 0, NULL))
	{
		BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, "SA-MP/0.3");
		float fVolume = ((AudioStreamGameView*)pGame)->pAudio->GetRadioVolume();
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, (DWORD)(fVolume * 7000.0f));
		BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 1);
		BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, 10000);
		if (!pConfig->IsVariableExist("audioproxyoff")) {
			pConfig->SetIntVariable("audioproxyoff", 0, 0);
		}
		if (pConfig->GetIntVariable("audioproxyoff")) {
			BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, NULL);
		}
		BASS_SetEAXParameters(-1, 0.0f, -1.0f, -1.0f);
		field_0 = 1;
		return TRUE;
	}
	return FALSE;
}

void CAudioStream::Stop()
{
	// TODO: CAudioStream::sub_10066560 10066560
}

void CAudioStream::ConstructInfo()
{
	// TODO: CAudioStream::sub_100665C0 100665C0
}

void CAudioStream::SyncProc()
{
	// TODO: CAudioStream::sub_100666F0 100666F0
}

void CAudioStream::Process()
{
	// TODO: CAudioStream::sub_10066700 10066700
}

void CAudioStream::Play()
{
	// TODO: CAudioStream::sub_10066960 10066960
}

void CAudioStream::ControlGameRadio()
{
	// TODO: CAudioStream::sub_10066A80 10066A80
}

void CAudioStream::DrawInfo()
{
	// TODO: CAudioStream::sub_10066AB0 10066AB0
}
