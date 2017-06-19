#include "MakePacket.h"



void MakeMoveStart(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << (BYTE)pPlayer->dwAction;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = (BYTE)Buffer.GetDataSize();
	Header.byType = dfPACKET_CS_MOVE_START;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeMoveStop(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_CS_MOVE_STOP;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeAttack1(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_CS_ATTACK1;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeAttack2(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_CS_ATTACK2;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeAttack3(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_CS_ATTACK3;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}



void MakeEcho(CSerializeBuffer *pBuffer, DWORD& ClientTick)
{
	st_NETWORK_PACKET_HEADER Header;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = 4;
	Header.byType = dfPACKET_CS_ECHO;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	*pBuffer << (DWORD)ClientTick;

	*pBuffer << dfNETWORK_PACKET_END;

}