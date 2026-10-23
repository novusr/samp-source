
#include "main.h"

// R5: 0x10060D40
CNetStats::CNetStats(IDirect3DDevice9 *pD3DDevice)
{
	field_8 = GetTickCount();
	field_4 = 0;
	field_0 = 0;
	field_10 = 0;
	field_C = 0;
	m_pD3DDevice = pD3DDevice;
}
