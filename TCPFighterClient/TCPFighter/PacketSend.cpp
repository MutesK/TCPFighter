#include "stdafx.h"
#include "NetworkProc.h"
#include "PacketSend.h"
#include "RingBuffer.h"
#include "Player.h"
#include "SerializeBuffer.h"


CRingBuffer SendQ(10000);
CSerializeBuffer SendBuffer;
extern bool g_bSend;
extern SOCKET g_ClientSocket;




bool SendPacket()
{
	if (!g_bSend)
		return false;

	//SendQ.Put((char *)pHeader, 4);
	//SendQ.Put((char *)Packet, pHeader->bySize);
	//char endCode = dfNETWORK_PACKET_END;
	//SendQ.Put(&endCode, 1);
	
	char *Write = SendQ.GetWriteBufferPtr();
	int size = SendBuffer.GetData(Write, SendBuffer.GetDataSize());
	SendQ.MoveWritePos(size);
	

	SendEvent();
	SendBuffer.Clear();
}

bool SendEvent()
{
	char tBuffer[100];

	if (!g_bSend)
		return true;

	if (SendQ.GetUseSize() <= 0)
		return true;

	while (1)
	{
		if (SendQ.GetUseSize() <= 9)
			break;

		int PeekSize = SendQ.Peek(tBuffer, min(SendQ.GetUseSize(), 100));

		WSABUF wsabuf;
		int bufcount = 1;

		wsabuf.len = PeekSize;
		wsabuf.buf = tBuffer;

		DWORD SendSize = 0;
		DWORD Flag = 0;


		int err = WSASend(g_ClientSocket, &wsabuf, bufcount, &SendSize, Flag, NULL, NULL);

		if (err == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				g_bSend = false;
				break;
			}
			else
			{
				//err_Print(L"WSASend");
				return false;
			}
		}

		SendQ.RemoveData(SendSize);
	}

	return true;
}

void SendStop(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	st_NETWORK_PACKET_HEADER Header;
	stPacket_CS_MOVESTOP Packet;

	makePacket_MoveStop(_iDir, Xpos, Ypos);
	SendPacket();
}
void SendMove(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	st_NETWORK_PACKET_HEADER Header;
	stPacket_CS_MOVESTART Packet;

	makePacket_MoveStart(_iDir, Xpos, Ypos);
	SendPacket();
}
void SendAttack1(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	st_NETWORK_PACKET_HEADER Header;
	stPacket_CS_ATTACK Packet;

	makePacket_ATTACK1(_iDir, Xpos, Ypos);
	SendPacket();
}
void SendAttack2(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	st_NETWORK_PACKET_HEADER Header;
	stPacket_CS_ATTACK Packet;

	makePacket_ATTACK2(_iDir, Xpos, Ypos);
	SendPacket();
}
void SendAttack3(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	st_NETWORK_PACKET_HEADER Header;
	stPacket_CS_ATTACK Packet;

	makePacket_ATTACK3(_iDir, Xpos, Ypos);
	SendPacket();
}


void makePacket_MoveStart(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	/*
	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = 5;
	pHeader->byType = dfPACKET_CS_MOVE_START;
	pHeader->byTemp = (BYTE)0;

	pPacket->Direction = _iDir;
	pPacket->X = Xpos;
	pPacket->Y = Ypos;

	//printf("# Move Start # Xpos %d Ypos %d Direction %d \n", Xpos, Ypos, _iDir);
	*/

	
	SendBuffer << dfNETWORK_PACKET_CODE << (BYTE)5 << (BYTE)dfPACKET_CS_MOVE_START << (BYTE)0;
	SendBuffer << _iDir << Xpos << Ypos << dfNETWORK_PACKET_END;
}

void makePacket_MoveStop(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	SendBuffer << dfNETWORK_PACKET_CODE << (BYTE)5 << (BYTE)dfPACKET_CS_MOVE_STOP << (BYTE)0;
	SendBuffer << _iDir << Xpos << Ypos << dfNETWORK_PACKET_END;
}

void makePacket_ATTACK1(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	SendBuffer << dfNETWORK_PACKET_CODE << (BYTE)5 << (BYTE)dfPACKET_CS_ATTACK1 << (BYTE)0;
	SendBuffer << _iDir << Xpos << Ypos << dfNETWORK_PACKET_END;
}

void makePacket_ATTACK2(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	SendBuffer << dfNETWORK_PACKET_CODE << (BYTE)5 << (BYTE)dfPACKET_CS_ATTACK2 << (BYTE)0;
	SendBuffer << _iDir << Xpos << Ypos << dfNETWORK_PACKET_END;
}

void makePacket_ATTACK3(BYTE _iDir, unsigned short Xpos, unsigned short Ypos)
{
	SendBuffer << dfNETWORK_PACKET_CODE << (BYTE)5 << (BYTE)dfPACKET_CS_ATTACK3 << (BYTE)0;
	SendBuffer << _iDir << Xpos << Ypos << dfNETWORK_PACKET_END;

}