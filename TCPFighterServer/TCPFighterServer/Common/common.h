#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "Winmm.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <list>
using namespace std;

#define FPS 25

#define dfLOG_LEVEL_DEBUG 0
#define dfLOG_LEVEL_WARNING 1
#define dfLOG_LEVEL_ERROR 2


// �����̴� ���� ����
#define dfACTION_MOVE_LL		0
#define dfACTION_MOVE_LU		1
#define dfACTION_MOVE_UU		2
#define dfACTION_MOVE_RU		3
#define dfACTION_MOVE_RR		4
#define dfACTION_MOVE_RD		5
#define dfACTION_MOVE_DD		6
#define dfACTION_MOVE_LD		7


#define dfDIR_LEFT			0
#define dfDIR_RIGHT			4

//-----------------------------------------------------------------
// ȭ�� �̵� ����. Ŭ���̾�Ʈ�� 2��
//-----------------------------------------------------------------
#define dfRANGE_MOVE_TOP	0
#define dfRANGE_MOVE_LEFT	0
#define dfRANGE_MOVE_RIGHT	6400
#define dfRANGE_MOVE_BOTTOM	6400


//-----------------------------------------------------------------
// ĳ���� �̵� �ӵ�
//-----------------------------------------------------------------
#define dfSPEED_PLAYER_X	6
#define dfSPEED_PLAYER_Y	4

#define dfRECKONING_SPEED_PLAYER_X 3
#define dfRECKONING_SPEED_PLAYER_Y 2

#define dfACTION_ATTACK1		11
#define dfACTION_ATTACK2		12
#define dfACTION_ATTACK3		13

#define dfSPRITE_MIDPOINT_CHARETER_x 71
#define dfSPRITE_MIDPOINT_CHARETER_y 90

#define dfACTION_STAND			255

struct st_Monitor
{
	SYSTEMTIME  stProcTime;

	DWORD		dwFPSCount;
	DWORD		dwLoopCount;
};

extern int g_iLogLevel;
extern WCHAR g_szLogBuffer[1024];

#define _LOG(LogLevel, fmt, ...)						\
do {													\
	if(g_iLogLevel <= LogLevel)							\
	{													\
		wsprintf(g_szLogBuffer, fmt, ##__VA_ARGS__);	\
		Log(g_szLogBuffer, LogLevel);					\
	}													\
}while(0)				


#define _OUTPUTDEBUG(fmt, ...)							


BOOL FrameCheck();

// ����͸� ����
// [��:��:�� ��:��:��]  Frame : �ʴ� ������, Recv : �ʴ� Recv ī��Ʈ
// ���� �Լ��� 1�� ���� ����ɼ� �ְԲ� ó���ؾ� ��.
void Monitoring();


void Log(WCHAR *szStr, int iLogLevel);

void ServerControl();

