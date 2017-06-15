#pragma once


#include "RingBuffer.h"
#include "SerializeBuffer.h"
#include "PacketDefine.h"

#define dfRECV_BUFF 3000

struct st_CLIENT
{
	SOCKET Socket;
	// 클라이언트 고유번호
	DWORD dwClientNo;	

	CSerializeBuffer SendQ;
	CSerializeBuffer RecvQ;

	// Recv 횟수 저장
	DWORD dwRecvCount;
	// Recv 시간 저장
	DWORD dwRecvTick;
	// Recv 시간 저장을 위한 비교시간(전처리 시간)
	DWORD dwRecvSecondTick;
};

bool NetworkInit();
void NetworkClear();
void NetworkProcess();
void SelectSocket(DWORD* pTableNo, SOCKET* pTableSocket, FD_SET *pReadSet, FD_SET *pWriteSet);
void SendProc(DWORD& UserNo);
void RecvProc(DWORD& UserNo);
int CompleteRecvPackcet(st_CLIENT *pClient);
bool PacketProc(st_CLIENT *pClient, const BYTE& MsgType, CSerializeBuffer *Buffer);


st_CLIENT* FindClient(DWORD &UserNo);
void DisconnectClient(DWORD &UserNo);

