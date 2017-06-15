
#include "NetworkProc.h"
#include "RecvPacket.h"
#include "../Content/ContentProc.h"
#include <algorithm>

SOCKET g_ListenSocket;

DWORD dwClientIDNo = 0;
DWORD dwLoopCount = 0;
map<DWORD, st_CLIENT *> g_ClientMap;


bool NetworkInit()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	g_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"0.0.0.0", &servAddr.sin_addr);
	servAddr.sin_port = htons(PORT);


	if (bind(g_ListenSocket, (SOCKADDR *)&servAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		wprintf(L"bind Error() \n");
		return false;
	}

	bool optval = true;
	setsockopt(g_ListenSocket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&optval), sizeof(optval));


	// 포트 오픈
	if (listen(g_ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		wprintf(L"Listen error \n");
		return false;
	}

	u_long on = 1;
	 ioctlsocket(g_ListenSocket, FIONBIO, &on);

	wprintf(L"Server Open.! \n");
	return true;
}
void NetworkProcess()
{
	st_CLIENT *pClient = nullptr;

	DWORD UserTable_No[FD_SETSIZE];
	SOCKET UserTable_SOCKET[FD_SETSIZE];
	int iSocketCount = 0;


	FD_SET RSet;
	FD_SET WSet;

	FD_ZERO(&RSet);
	FD_ZERO(&WSet);
	memset(UserTable_No, -1, sizeof(DWORD) * FD_SETSIZE);
	memset(UserTable_SOCKET, INVALID_SOCKET, sizeof(SOCKET) *FD_SETSIZE);

	// 리슨소켓은 0으로 함.
	FD_SET(g_ListenSocket, &RSet);
	UserTable_No[iSocketCount] = 0;
	UserTable_SOCKET[iSocketCount] = g_ListenSocket;
	iSocketCount++;
	dwLoopCount++;


	// 리슨소켓 및 접속중인 모든 클라이언트에 대해 SOCKET 체크
	auto begin = g_ClientMap.begin();
	auto end = g_ClientMap.end();
	for (auto iter = begin; iter != end;)
	{
		pClient = iter->second;
		iter++;

		UserTable_No[iSocketCount] = pClient->dwClientNo;
		UserTable_SOCKET[iSocketCount] = pClient->Socket;

		FD_SET(pClient->Socket, &RSet);

		if (pClient->SendQ.GetDataSize() > 0)
			FD_SET(pClient->Socket, &WSet);

		iSocketCount++;

		// select 최대치 도달, 만들어진 테이블 정보로 select 호출뒤 정리
		if (FD_SETSIZE <= iSocketCount)
		{
			SelectSocket(UserTable_No, UserTable_SOCKET, &RSet, &WSet);

			FD_ZERO(&RSet);
			FD_ZERO(&WSet);

			memset(UserTable_No, -1, sizeof(DWORD) * FD_SETSIZE);
			memset(UserTable_SOCKET, INVALID_SOCKET, sizeof(SOCKET) * FD_SETSIZE);
			iSocketCount = 0;
			// 왜 Sleep 걸면 되냐?
		}
	}
	// 전체 클라이언트 for문 종료 후, socketCount 수치가 있다면
	// 추가적으로 마지막 select 호출을 한다.
	if (iSocketCount > 0)
	{
		SelectSocket(UserTable_No, UserTable_SOCKET, &RSet, &WSet);
	}
}
void SelectSocket(DWORD* pTableNo, SOCKET* pTableSocket, FD_SET *pReadSet, FD_SET *pWriteSet)
{
	timeval Time;
	int iResult;

	Time.tv_sec = 0;
	Time.tv_usec = 0;

	iResult = select(0, pReadSet, pWriteSet, NULL, &Time);

	// 리턴값이 0보다 이상은 데이터 도착
	if (0 < iResult)
	{
		// TableSocket을 돌면서 어떤 소켓에 반응이 온것인지 확인
		for (auto iCnt = 0; iCnt < FD_SETSIZE; iCnt++)
		{
			if (pTableSocket[iCnt] == INVALID_SOCKET)
				continue;

			// WRITE 체크
			if (FD_ISSET(pTableSocket[iCnt], pWriteSet))
				SendProc(pTableNo[iCnt]);

			if (FD_ISSET(pTableSocket[iCnt], pReadSet))
			{
				if (pTableNo[iCnt] == 0)
					AcceptProc();
				else
					RecvProc(pTableNo[iCnt]);
			}
		}
	}
	else if (iResult == SOCKET_ERROR)
	{
		wprintf(L"Select Socket Error ! \n");
		return;
	}
}
void AcceptProc()
{
	st_CLIENT *pUser = nullptr;
	pUser = new st_CLIENT;
	SOCKADDR_IN ConnectAdr;
	int addrlen = sizeof(SOCKADDR_IN);

	pUser->Socket = accept(g_ListenSocket, reinterpret_cast<SOCKADDR *>(&ConnectAdr), &addrlen);
	if (pUser->Socket == INVALID_SOCKET)
	{
		delete pUser;
		return;
	}
	dwClientIDNo++;

	pUser->dwClientNo = dwClientIDNo;
	pUser->dwRecvCount = 0;
	pUser->dwRecvTick = timeGetTime();
	pUser->dwRecvSecondTick = 0;

	g_ClientMap.insert(map<UINT64, st_CLIENT *>::value_type(dwClientIDNo, pUser));

	WCHAR szClientIP[16] = { 0 };
	InetNtop(AF_INET, &ConnectAdr.sin_addr, szClientIP, 16);
	wprintf(L"Accpet - %s:%d Socket : %d \n", szClientIP, ntohs(ConnectAdr.sin_port), pUser->Socket);
	_LOG(dfLOG_LEVEL_DEBUG, L"Accpet - %s:%d Socket : %d \n", szClientIP, ntohs(ConnectAdr.sin_port), pUser->Socket);

	// 콘텐츠의 케릭터 할당 시켜야 된다.
	CreateCharacter(pUser);
}
void SendProc(DWORD& UserNo)
{
	st_CLIENT *pClient;

	pClient = FindClient(UserNo);

	if (pClient == nullptr)
		return;

	int iSendSize = pClient->SendQ.GetDataSize();
	iSendSize = min(dfRECV_BUFF, iSendSize);

	if (0 >= iSendSize)
		return;

	int iResult = send(pClient->Socket, pClient->SendQ.GetReadBufferPtr(), iSendSize, 0);


	if (iResult == SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		if (dwError == WSAEWOULDBLOCK)
			return;

		_LOG(dfLOG_LEVEL_ERROR, L"Send Proc -> SOCKET_ERROR : UserNo : %d", UserNo);
		DeleteCharacter(pClient->dwClientNo);

		return;
	}
	else if (iResult > iSendSize)
	{

		return;
	}
	pClient->SendQ.MoveReadPos(iResult);
}
void RecvProc(DWORD& UserNo)
{
	st_CLIENT *pClient;
	int iResult;
	pClient = FindClient(UserNo);

	if (pClient == nullptr)
		return;

	iResult = recv(pClient->Socket, pClient->RecvQ.GetWriteBufferPtr(), dfRECV_BUFF, 0);
	pClient->RecvQ.MoveWritePos(iResult);
	

	// 틱 저장
	pClient->dwRecvCount++;
	pClient->dwRecvSecondTick = pClient->dwRecvTick;
	pClient->dwRecvTick = timeGetTime() - pClient->dwRecvSecondTick;

	if (SOCKET_ERROR == iResult || 0 == iResult)
	{
		DeleteCharacter(pClient->dwClientNo);
		return;
	}

	if (0 < iResult)
	{
		//  패킷이 완료되었는지 확인
		// 패킷 처리중 문제 발생하면 종료
		// 패킷은 하나 이상의 버퍼에 있을수 있으므로 한번에 전부 처리해야 됨.
		while (1)
		{
			iResult = CompleteRecvPackcet(pClient);

			if (iResult == 1)
				break;

			if (iResult == -1)
			{
				_LOG(dfLOG_LEVEL_ERROR, L"Recv Proc -> Packet Proc : UserNo : %d", UserNo);
				return;
			}

		}
	}
}
int CompleteRecvPackcet(st_CLIENT *pClient)
{
	int iRecvSize = pClient->RecvQ.GetDataSize();

	// 헤더보다 적을때
	if (sizeof(st_NETWORK_PACKET_HEADER) > iRecvSize)
		return 1;


	st_NETWORK_PACKET_HEADER Header;
	pClient->RecvQ.PeekData((char *)&Header, sizeof(st_NETWORK_PACKET_HEADER));

	// 패킷 시작 코드  확인
	if (Header.byCode != (BYTE)dfNETWORK_PACKET_CODE)
		return -1;

	// 최소 사이즈량 확인
	if (Header.bySize + sizeof(st_NETWORK_PACKET_HEADER) > (WORD)iRecvSize)
		return 1;

	// 최대 사이즈량 확인 -> 혹시나 모를 패킷공격 대비
	if (iRecvSize > dfRECV_BUFF)
		return -1;

	// PeekData데이터 만큼 움직인다.
	pClient->RecvQ.MoveReadPos(sizeof(st_NETWORK_PACKET_HEADER));

	CRingBuffer Buffer(dfRECV_BUFF);

	if (Header.bySize != Buffer.Put(pClient->RecvQ.GetReadBufferPtr(), Header.bySize))
	{
		//OutputDebugString(L"CRingBuffer Put Error \n");
		_LOG(dfLOG_LEVEL_ERROR, L"ComplteRecvPacket -> Temp RingBuffer Put Error : UserNo : %d", pClient->dwClientNo);
		return -1;
	}

	if (pClient->RecvQ.MoveReadPos(Header.bySize) != Header.bySize)
	{
		//OutputDebugString(L"Move Read Pos Error \n");
		_LOG(dfLOG_LEVEL_ERROR, L"ComplteRecvPacket -> RecvQ MoveReadPos Size Error : UserNo : %d", pClient->dwClientNo);

		return -1;
	}

	// 엔드코드 체킹
	BYTE endCode;
	pClient->RecvQ >> endCode;
	if (endCode != dfNETWORK_PACKET_END)
		return -1;

	if (!PacketProc(pClient, Header.byType, &Buffer))
		return -1;


	return 0;
}
bool PacketProc(st_CLIENT *pClient, const BYTE& MsgType, CRingBuffer *Buffer)
{
	switch (MsgType)
	{
	default:
		OutputDebugString(L"Packet Error");
		return false;
	case dfPACKET_CS_MOVE_START:
		RecvPacket_MoveStart(pClient, Buffer);
		break;
	case dfPACKET_CS_MOVE_STOP:
		RecvPacket_MoveStop(pClient, Buffer);
		break;
	case dfPACKET_CS_ATTACK1:
		RecvPacket_Attack1(pClient, Buffer);
		break;
	case dfPACKET_CS_ATTACK2:
		RecvPacket_Attack2(pClient, Buffer);
		break;
	case dfPACKET_CS_ATTACK3:
		RecvPacket_Attack3(pClient, Buffer);
		break;
	case dfPACKET_CS_ECHO:
		RecvPacket_Echo(pClient, Buffer);
		break;
	}
	return true;
}
st_CLIENT* FindClient(DWORD &UserNo)
{
	auto Finditer = g_ClientMap.find(UserNo);

	if (Finditer != g_ClientMap.end())
		return Finditer->second;

	return nullptr;
}
void NetworkClear()
{
	auto begin = g_ClientMap.begin();
	auto end = g_ClientMap.end();

	for (auto iter = begin; iter != end; )
	{
		st_CLIENT *pClient = (*iter).second;

		closesocket(pClient->Socket);
		iter = g_ClientMap.erase(iter);

		delete pClient;
	}
	
}
void DisconnectClient(DWORD &UserNo)
{
	st_CLIENT *pClient = FindClient(UserNo);

	if (pClient == nullptr)
		return;

	closesocket(pClient->Socket);
	g_ClientMap.erase(UserNo);
	delete pClient;
}
