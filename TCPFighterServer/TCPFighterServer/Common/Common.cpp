#include "common.h"
#include "../Network/NetworkProc.h"
#include "../Content/ContentProc.h"
 
extern DWORD dwLoopCount;
extern DWORD dwFPSCount;

DWORD iTick;

int g_iLogLevel = 0;
WCHAR g_szLogBuffer[1024];

extern bool isQuit;
BOOL FrameCheck()
{
	static DWORD m_dwSystemTick = 0;
	static DWORD m_iOneFrameTick = 1000 / FPS;

	BYTE flag;
	DWORD dwTick = GetTickCount64();
	iTick += dwTick - m_dwSystemTick;
	flag = false;

	if (dwFPSCount * m_iOneFrameTick < iTick
		&&	(dwFPSCount + 1) * m_iOneFrameTick > iTick)
		flag = true;
	

	m_dwSystemTick = GetTickCount64();

	return flag;
}

void Monitoring()
{
	DWORD Tick = GetTickCount64();
	static DWORD timeTickCount = GetTickCount64();


	if (Tick - timeTickCount  > 1000)
	{
		st_Monitor *pMonitor = new st_Monitor;
		pMonitor->dwLoopCount = dwLoopCount;
		pMonitor->dwFPSCount = dwFPSCount;
		GetLocalTime(&pMonitor->stProcTime);

		WCHAR LogStr[1024];
		wsprintf(LogStr ,L"[%4d:%02d:%02d %02d:%02d:%02d] Frame : %d Loop : %d", 
			pMonitor->stProcTime.wYear, pMonitor->stProcTime.wMonth, pMonitor->stProcTime.wDay,
			pMonitor->stProcTime.wHour, pMonitor->stProcTime.wMinute, pMonitor->stProcTime.wSecond, 
			dwFPSCount, dwLoopCount);

		wprintf(L"%s\n", LogStr);
		
		_LOG(dfLOG_LEVEL_DEBUG, LogStr);

		Tick = 0;
		dwLoopCount = 0;
		dwFPSCount = 0;
		iTick = 0;

		timeTickCount = GetTickCount64();
		delete pMonitor;
	}
}

void Log(WCHAR *szStr, int iLogLevel)
{
	SYSTEMTIME ProcTime;
	GetLocalTime(&ProcTime);

	WCHAR *szFileName = new WCHAR[100];
	swprintf_s(szFileName, sizeof(WCHAR) * 100 , L"Log\\Log%4d.%02d.%02d.txt",
		ProcTime.wYear, ProcTime.wMonth, ProcTime.wDay, ProcTime.wHour, ProcTime.wMinute);

	FILE *fp;

	_wfopen_s(&fp, szFileName, L"ab");
	fseek(fp, 0, SEEK_END);		// ������ ������ �̵�

	if (fp == nullptr)
		return;

	WCHAR *pPrint = new WCHAR[2000];
	switch (iLogLevel) 
	{
	case dfLOG_LEVEL_DEBUG:
		wsprintf(pPrint, L"Log[DEBUG]:");
		break;
	case dfLOG_LEVEL_WARNING:
		wsprintf(pPrint, L"Log[WARNING]:");
		break;
	case dfLOG_LEVEL_ERROR:
		wsprintf(pPrint, L"Log[ERROR]:");
		break;
	}
	lstrcatW(pPrint, szStr);

	fwprintf(fp, L"%s\r\n", pPrint);

	fclose(fp);
	delete szFileName;
	delete pPrint;
}

void ServerControl()
{
	static bool LockControl = true;

	// ��Ʈ�� �� Ǯ�� : u, ��Ʈ�� �� �ϱ� l, ���� ���� q

	if (GetAsyncKeyState(0x55))
	{
		if (LockControl == false)
			return;
		wprintf(L"Control Lock is Disable \n");
		LockControl = false;
	}

	if (GetAsyncKeyState(0x4C))
	{
		if (LockControl == false)
			true;
		wprintf(L"Control Lock is Enable \n");
		LockControl = true;
	}
	

	if (!LockControl)
	{
		wprintf(L"Control Mode : Press Q : Quit \n");

		if (GetAsyncKeyState(0x51))
			isQuit = true;
	}
}