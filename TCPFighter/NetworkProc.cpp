#include "stdafx.h"
#include "NetworkProc.h"
#include "PacketRecv.h"
#include "PacketSend.h"

SOCKET g_ClientSocket;
bool g_bConnect;
bool g_bSend;

extern HWND g_hWnd;

bool NetworkInit(WCHAR *g_zIP)
{
	g_bConnect = false;
	g_bSend = false;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	g_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (g_ClientSocket == INVALID_SOCKET)
		return false;
	
	WSAAsyncSelect(g_ClientSocket, g_hWnd, WM_NETWORK, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPton(AF_INET, g_zIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(PORT);

	int err = connect(g_ClientSocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (err == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return false;
	}

	return true;

}
bool NetworkProc(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam != 0))
		return false;

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		g_bConnect = true;
		break;
	case FD_READ:
		RecvEvent();
		break;
	case FD_WRITE:
		g_bSend = true;
		SendEvent();
		break;
	case FD_CLOSE:
		CloseNetwork();
		break;
	}
}
void CloseNetwork()
{
	g_bConnect = false;
	g_bSend = false;

	closesocket(g_ClientSocket);

	g_ClientSocket = INVALID_SOCKET;

}