#include "MakePacket.h"
#include "../Content/Sector.h"

extern list<st_PLAYER *> g_Sector[][dfSECTOR_MAX_X];

void MakeCreateMyCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;
	
	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;
	Buffer << (char)pPlayer->chHP;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_CREATE_MY_CHARACTER;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);
	
	*pBuffer << dfNETWORK_PACKET_END;

}

void MakeCreateOtherCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;
	Buffer << (char)pPlayer->chHP;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}



void MakeDeleteCharacterInSector(st_SECTOR_POS *pPos, st_PLAYER* pExcept, CSerializeBuffer *pBuffer)
{
	if (pPos == nullptr)
		return;

	list <st_PLAYER *> &list = g_Sector[pPos->iY][pPos->iX];
	//_OUTPUTDEBUG(L"MakeDeleteCharacterInSector [%d %d]\n", pPos->iY, pPos->iX);

	auto begin = list.begin();
	auto end = list.end();
	for (auto iter = begin; iter != end; ++iter)
	{
		st_PLAYER *pPlayer = (*iter);

		if (pPlayer != pExcept)
			MakeDeleteCharacter(pPlayer, pBuffer);
	}
}


void MakeDeleteCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;

	//_OUTPUTDEBUG(L"Make Packet DeleteCharacter Player ID : %d \n", pPlayer->dwClientNo);

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = (BYTE)Buffer.GetDataSize();
	Header.byType = (BYTE)dfPACKET_SC_DELETE_CHARACTER;



	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << (BYTE)dfNETWORK_PACKET_END;
}

void MakeMoveStart(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << (DWORD)pPlayer->dwClientNo;
	Buffer << (BYTE)pPlayer->dwAction;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = (BYTE)Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_MOVE_START;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeMoveStop(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_MOVE_STOP;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeAttack1(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_ATTACK1;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeAttack2(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_ATTACK2;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeAttack3(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;
	Buffer << pPlayer->byDirection;
	Buffer << (WORD)pPlayer->woX;
	Buffer << (WORD)pPlayer->woY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_ATTACK3;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeDamage(st_PLAYER *pAttacker, st_PLAYER *pDamager, CSerializeBuffer *pBuffer)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pAttacker->dwClientNo;
	Buffer << pDamager->dwClientNo;
	Buffer << pDamager->chHP;		

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_DAMAGE;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}

void MakeSync(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer, WORD wX, WORD wY)
{
	st_NETWORK_PACKET_HEADER Header;

	CSerializeBuffer Buffer;
	Buffer << pPlayer->dwClientNo;
	Buffer << wX;
	Buffer << wY;

	Header.byCode = dfNETWORK_PACKET_CODE;
	Header.bySize = Buffer.GetDataSize();
	Header.byType = dfPACKET_SC_DAMAGE;

	pBuffer->PutData(reinterpret_cast<char *>(&Header), sizeof(st_NETWORK_PACKET_HEADER));
	pBuffer->PutData(Buffer.GetReadBufferPtr(), Header.bySize);

	*pBuffer << dfNETWORK_PACKET_END;
}