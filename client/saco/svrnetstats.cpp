
#include "main.h"
#include <stdio.h>

char szSvrDispBuf[16384];
char szSvrStatBuf[16384];
char szSvrDrawLine[1024];
RakNetStatisticsStruct RakServerStats;

// R5: 0x10071220
CSvrNetStats::CSvrNetStats(IDirect3DDevice9 *pD3DDevice)
{
	m_dwLastUpdateTick = GetTickCount();
	m_dwLastTotalBytesRecv = 0;
	m_dwLastTotalBytesSent = 0;
	m_dwBPSDownload = 0;
	m_dwBPSUpload = 0;
	m_pD3DDevice = pD3DDevice;
	memset(&RakServerStats,0,sizeof(RakNetStatisticsStruct));
}

void CSvrNetStats::Draw()
{
	return;
}
