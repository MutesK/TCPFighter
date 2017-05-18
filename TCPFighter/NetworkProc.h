#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "stdafx.h"
#include "PacketDefine.h"

#define PORT 20000
#define WM_NETWORK WM_USER + 1

bool NetworkInit(WCHAR *g_zIP);
bool NetworkProc(WPARAM, LPARAM);
void CloseNetwork();