#include "common.h"
#include "../Network/NetworkProc.h"
#include "../Content/ContentProc.h"
 


DWORD iTick;

int g_iLogLevel = 0;
WCHAR g_szLogBuffer[1024];

int	g_Try = 0;
int	g_Fail = 0;
int	g_Success = 0;

UINT g_EchoCount;
DWORD g_dwClientTick;
DWORD g_dwAvrClintTick;
DWORD g_dwMaxClientTick;

int g_AttackPacket = 0;
int g_MovePacket = 0;

void Monitoring()
{
	DWORD Tick = timeGetTime();
	static DWORD timeTickCount = timeGetTime();

	if (Tick - timeTickCount  > 1000)
	{
		WCHAR LogStr[1024];

		if(g_dwClientTick != 0 || g_EchoCount != 0)
			g_dwAvrClintTick = g_dwClientTick / g_EchoCount / 1000;

		DWORD Stand = g_Success - (g_AttackPacket + g_MovePacket);

		wprintf(L"=========================================\n");
		wprintf(L"Try : %d,  Success : %d,   Fail : %d  \n", g_Try, g_Success, g_Fail);
		wprintf(L"RTT : Average : %lld ms, Max : %lld ms EchoCount : %u\n", g_dwAvrClintTick, g_dwMaxClientTick / 1000, g_EchoCount);
		wprintf(L"Stand : %lld, Attack : %lld, Move : %lld \n", Stand, g_AttackPacket, g_MovePacket);
		wprintf(L"=========================================\n");

		Tick = 0;
		iTick = 0;

		g_EchoCount = 0;
		g_dwClientTick = 0;
		g_dwAvrClintTick = 0;
		g_dwMaxClientTick = 0;
		g_AttackPacket = g_MovePacket = 0;
		timeTickCount = timeGetTime();
	}
}

void Log(WCHAR *szStr, int iLogLevel)
{
	SYSTEMTIME ProcTime;
	GetLocalTime(&ProcTime);

	WCHAR *szFileName = new WCHAR[100];
	swprintf_s(szFileName,  100 , L"Log%4d.%02d.%02d.txt",
		ProcTime.wYear, ProcTime.wMonth, ProcTime.wDay);

	FILE *fp;

	_wfopen_s(&fp, szFileName, L"ab");
	fseek(fp, 0, SEEK_END);		// 파일의 끝으로 이동

	if (fp == nullptr)
		return;

	WCHAR *pPrint = new WCHAR[2000];
	switch (iLogLevel) 
	{
	case dfLOG_LEVEL_DEBUG:
		wsprintf(pPrint, L"Log[DEBUG]:[%4d:%02d:%02d %02d:%02d:%02d]", 
			ProcTime.wYear, ProcTime.wMonth, ProcTime.wDay, ProcTime.wHour, ProcTime.wMinute, ProcTime.wSecond);
		break;
	case dfLOG_LEVEL_WARNING:
		wsprintf(pPrint, L"Log[WARNING]:[%4d:%02d:%02d %02d:%02d:%02d]",
			ProcTime.wYear, ProcTime.wMonth, ProcTime.wDay, ProcTime.wHour, ProcTime.wMinute, ProcTime.wSecond);
		break;
	case dfLOG_LEVEL_ERROR:
		wsprintf(pPrint, L"Log[ERROR]:[%4d:%02d:%02d %02d:%02d:%02d]",
			ProcTime.wYear, ProcTime.wMonth, ProcTime.wDay, ProcTime.wHour, ProcTime.wMinute, ProcTime.wSecond);
		break;
	}
	lstrcatW(pPrint, szStr);

	fwprintf(fp, L"%s\r\n", pPrint);

	fclose(fp);
	delete[] szFileName;
	delete[] pPrint;
}

