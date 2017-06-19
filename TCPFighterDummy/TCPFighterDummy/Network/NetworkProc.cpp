
#include "NetworkProc.h"
#include "RecvPacket.h"
#include "../Content/ContentProc.h"
#include <algorithm>



DWORD dwClientIDNo = 0;
map<DWORD, st_CLIENT *> g_ClientMap;
extern int	g_Try;
extern int	g_Fail;

bool NetworkInit()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	int ClientCount;
	WCHAR szIP[20];
	// 입력
	
	wprintf(L"Client Connect : ");
	cin >> ClientCount;

	wprintf(L"\nServer IP : ");
	wcin >> szIP;

	g_Try = ClientCount;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPton(AF_INET, szIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(PORT);

	st_CLIENT *pClient = new st_CLIENT[ClientCount];
	for (int i = 0; i < ClientCount; i++)
	{
		pClient[i].dwClientNo = i + 1;
		pClient[i].Socket = socket(AF_INET, SOCK_STREAM, 0);
		
		if (pClient[i].Socket == INVALID_SOCKET)
		{
			DeleteCharacter(pClient->dwClientNo, pClient->dwPlayerNo);
			continue;
		}

		// Nodelay 옵션 ON!
		bool optval = true;
		setsockopt(pClient[i].Socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&optval), sizeof(optval));

		// 동기 상태로 연결
		int err = connect(pClient[i].Socket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

		if (err == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				DeleteCharacter(pClient->dwClientNo, pClient->dwPlayerNo);
				continue;
			}
		}

		// 연결뒤 비동기화
		u_long on = 1;
		err = ioctlsocket(pClient[i].Socket, FIONBIO, &on);
		if (err == SOCKET_ERROR)
		{
			DeleteCharacter(pClient->dwClientNo, pClient->dwPlayerNo);
			continue;
		}


		wprintf(L"Connect : %u Client : %u\n", pClient[i].Socket, pClient[i].dwClientNo);

		g_ClientMap.insert({ i+1  , &pClient[i]});
	}

	wprintf(L"For Stable to Server (Wait 5 Sec) \n");
	Sleep(5000);

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
				RecvProc(pTableNo[iCnt]);
		}
	}
	else if (iResult == SOCKET_ERROR)
	{
		wprintf(L"Select Socket Error ! \n");
		return;
	}
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
		DeleteCharacter(pClient->dwClientNo, pClient->dwPlayerNo);

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
		DeleteCharacter(pClient->dwClientNo, pClient->dwPlayerNo);
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

	CSerializeBuffer Buffer(dfRECV_BUFF);

	if (Header.bySize != Buffer.PutData(pClient->RecvQ.GetReadBufferPtr(), Header.bySize))
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
bool PacketProc(st_CLIENT *pClient, const BYTE& MsgType, CSerializeBuffer *Buffer)
{
	switch (MsgType)
	{
	default:
		_LOG(dfLOG_LEVEL_ERROR, L"####### Packet ERROR #######  Client No: %d \n", pClient->dwClientNo);
		wprintf(L"####### Packet ERROR #######  Client No: %d \n", pClient->dwClientNo);
		return false;
	case dfPACKET_SC_CREATE_MY_CHARACTER:
		netPacket_CreateMyCharacter(pClient, Buffer);
		break;
	case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		netPacket_CreateOtherCharacter(pClient, Buffer);
		break;
	case dfPACKET_SC_DELETE_CHARACTER:
		netPacket_DeleteCharacter(pClient, Buffer);
		break;
	case dfPACKET_SC_MOVE_START:
		netPacket_SC_MoveStart(pClient, Buffer);
		break;
	case dfPACKET_SC_MOVE_STOP:
		netPacket_SC_MoveStop(pClient, Buffer);
		break;
	case dfPACKET_SC_ATTACK1:
		netPacket_SC_ATTACK1(pClient, Buffer);
		break;
	case dfPACKET_SC_ATTACK2:
		netPacket_SC_ATTACK2(pClient, Buffer);
		break;
	case dfPACKET_SC_ATTACK3:
		netPacket_SC_ATTACK3(pClient, Buffer);
		break;
	case dfPACKET_SC_DAMAGE:
		netPacket_SC_DAMAGE(pClient, Buffer);
		break;
	case dfPACKET_SC_SYNC:
		netPacket_SC_SYNC(pClient, Buffer);
		break;
	case dfPACKET_SC_ECHO:
		netPacket_SC_ECHO(pClient, Buffer);
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
	//delete pClient;
}
