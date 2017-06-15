#include "common.h"
#include "../Network/NetworkProc.h"
#include "../Content/ContentProc.h"
 
extern DWORD dwLoopCount;
extern DWORD dwFPSCount;

DWORD iTick;

int g_iLogLevel = 0;
WCHAR g_szLogBuffer[1024];

UINT	g_Try = 0;
UINT	g_Fail = 0;
UINT	g_Success = 0;

void Monitoring()
{
	DWORD Tick = timeGetTime();
	static DWORD timeTickCount = timeGetTime();


	if (Tick - timeTickCount  > 1000)
	{
		st_Monitor pMonitor;
		pMonitor.dwLoopCount = dwLoopCount;
		GetLocalTime(&pMonitor.stProcTime);

		WCHAR LogStr[1024];


		wprintf(L"%s\n", LogStr);

		Tick = 0;
		dwLoopCount = 0;
		dwFPSCount = 0;
		iTick = 0;

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

