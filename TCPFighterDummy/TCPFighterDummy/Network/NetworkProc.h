#pragma once


#include "RingBuffer.h"
#include "SerializeBuffer.h"
#include "PacketDefine.h"

#define dfRECV_BUFF 3000

struct st_CLIENT
{
	SOCKET Socket;
	// Ŭ���̾�Ʈ ������ȣ
	DWORD dwClientNo;	

	CSerializeBuffer SendQ;
	CSerializeBuffer RecvQ;

	// Recv Ƚ�� ����
	DWORD dwRecvCount;
	// Recv �ð� ����
	DWORD dwRecvTick;
	// Recv �ð� ������ ���� �񱳽ð�(��ó�� �ð�)
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

