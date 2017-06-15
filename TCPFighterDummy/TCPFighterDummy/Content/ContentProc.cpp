#include "ContentProc.h"
#include "../Network/MakePacket.h"

///////////////////////////////////////////////
// Key : Ŭ���̾�Ʈ ��ȣ
// Value : �÷��̾� ������
///////////////////////////////////////////////
map <DWORD, st_PLAYER *> g_PlayerMap;
DWORD dwFPSCount = 0;
extern UINT	g_Fail;
extern UINT	g_Success;

st_PLAYER * FindPlayer(DWORD &ClientNo)
{
	auto Finditer = g_PlayerMap.find(ClientNo);

	if (Finditer != g_PlayerMap.end())
		return Finditer->second;

	return nullptr;
}

// �׼� ���� �ð��� �������϶�� ��ŵ.
void Update()
{
	auto begin = g_PlayerMap.begin();
	auto end = g_PlayerMap.end();

	int MaxProcessClient = 60;
	int ProcessClientCnt = 0;
	Echo();

	for (auto iter = begin; iter != end; ++iter)
	{
		st_PLAYER *pPlayer = (*iter).second;

		if (ProcessClientCnt >= MaxProcessClient)
			break;

		// 50�� �̻� ����ߴٸ�
		if (timeGetTime() - pPlayer->dwActionTick >= 50000)
		{
// �ൿ�� Ÿ���� ���Ѵ�.  
/*
		Move, Stand, Attack1, Attack2, Attack3
*/
			int Action = rand() % 5;
			switch (Action)
			{
			case 0:
				if (pPlayer->dwAction < dfACTION_MOVE_LL || pPlayer->dwAction > dfACTION_MOVE_LD)
					continue;

				CharacterMoveStart(pPlayer);
				break;
			case 1:
				if (pPlayer->dwAction == dfACTION_STAND)
					continue;

				CharacterMoveStop(pPlayer);
				break;
				// �����̴ٰ� �ٷ� �����Ҽ�����.
			case 2:
				if (pPlayer->dwAction < dfACTION_MOVE_LL || pPlayer->dwAction > dfACTION_MOVE_LD)
					continue;

				CharacterAttack1(pPlayer);
				break;
			case 3:
				if (pPlayer->dwAction < dfACTION_MOVE_LL || pPlayer->dwAction > dfACTION_MOVE_LD)
					continue;

				CharacterAttack2(pPlayer);
				break;
			case 4:
				if (pPlayer->dwAction < dfACTION_MOVE_LL || pPlayer->dwAction > dfACTION_MOVE_LD)
					continue;

				CharacterAttack3(pPlayer);
				break;
			}

			ProcessClientCnt++;
		}
	}
	
}

int DeadReckoning(DWORD dwAction, DWORD dwActionTick, int OldPosX, int OldPosY, int *pOutX, int *pOutY)
{
	DWORD dwIntervalTick = timeGetTime() - dwActionTick;

	int iActionFrame = dwIntervalTick / 20;
	int iRemoveFrame = 0;

	int iValue;
	int iPosX = OldPosX;
	int iPosY = OldPosY;

	// ���� ���������� X, Y �� �̵����� ���Ѵ�.
	int iDX = iActionFrame * dfRECKONING_SPEED_PLAYER_X;
	int iDY = iActionFrame * dfRECKONING_SPEED_PLAYER_Y;

	switch (dwAction)
	{
	case dfACTION_MOVE_LL:
		iPosX = OldPosX - iDX;
		iPosY = OldPosY;
		break;
	case dfACTION_MOVE_LU:
		iPosX = OldPosX - iDX;
		iPosY = OldPosY - iDY;
		break;
	case dfACTION_MOVE_UU:
		iPosX = OldPosX;
		iPosY = OldPosY - iDY;
		break;
	case dfACTION_MOVE_RU:
		iPosX = OldPosX + iDX;
		iPosY = OldPosY - iDY;
		break;
	case dfACTION_MOVE_RR:
		iPosX = OldPosX + iDX;
		iPosY = OldPosY;
		break;
	case dfACTION_MOVE_RD:
		iPosX = OldPosX + iDX;
		iPosY = OldPosY + iDY;
		break;
	case dfACTION_MOVE_DD:
		iPosX = OldPosX;
		iPosY = OldPosY + iDY;
		break;
	case dfACTION_MOVE_LD:
		iPosX = OldPosX - iDX;
		iPosY = OldPosY + iDY;
		break;
	}

	// ���� ���� ��ǥ�� ȭ���� �̵� ������ ����ٸ�, �� �׼��� �߶󳻱� ���ؼ�
	// ������ ��� ������ �������� ���
	if (iPosX <= dfRANGE_MOVE_LEFT)
	{
		iValue = abs(dfRANGE_MOVE_LEFT - abs(iPosX)) / dfRECKONING_SPEED_PLAYER_X;
		iRemoveFrame = max(iValue, iRemoveFrame);
	}

	if (iPosY <= dfRANGE_MOVE_TOP)
	{
		iValue = abs(dfRANGE_MOVE_TOP - abs(iPosY)) / dfRECKONING_SPEED_PLAYER_Y;
		iRemoveFrame = max(iValue, iRemoveFrame);
	}

	if (iPosX >= dfRANGE_MOVE_RIGHT)
	{
		iValue = abs(dfRANGE_MOVE_RIGHT - iPosX) / dfRECKONING_SPEED_PLAYER_X;
		iRemoveFrame = max(iValue, iRemoveFrame);
	}

	if (iPosY >= dfRANGE_MOVE_BOTTOM)
	{
		iValue = abs(dfRANGE_MOVE_BOTTOM - iPosY) / dfRECKONING_SPEED_PLAYER_Y;
		iRemoveFrame = max(iValue, iRemoveFrame);
	}

	// ������ ���� ��� ���� �Ǿ�� �� �������� ��Ÿ���ٸ�, ��ǥ�� �� ����Ѵ�.
	if (iRemoveFrame > 0)
	{
		iActionFrame -= iRemoveFrame;

		iDX = iActionFrame * dfRECKONING_SPEED_PLAYER_X;
		iDY = iActionFrame * dfRECKONING_SPEED_PLAYER_Y;

		switch (dwAction)
		{
		case dfACTION_MOVE_LL:
			iPosX = OldPosX - iDX;
			iPosY = OldPosY;
			break;
		case dfACTION_MOVE_LU:
			iPosX = OldPosX - iDX;
			iPosY = OldPosY - iDY;
			break;
		case dfACTION_MOVE_UU:
			iPosX = OldPosX;
			iPosY = OldPosY - iDY;
			break;
		case dfACTION_MOVE_RU:
			iPosX = OldPosX + iDX;
			iPosY = OldPosY - iDY;
			break;
		case dfACTION_MOVE_RR:
			iPosX = OldPosX + iDX;
			iPosY = OldPosY;
			break;
		case dfACTION_MOVE_RD:
			iPosX = OldPosX + iDX;
			iPosY = OldPosY + iDY;
			break;
		case dfACTION_MOVE_DD:
			iPosX = OldPosX;
			iPosY = OldPosY + iDY;
			break;
		case dfACTION_MOVE_LD:
			iPosX = OldPosX - iDX;
			iPosY = OldPosY + iDY;
			break;
		}
	}

	iPosX = min(iPosX, dfRANGE_MOVE_RIGHT);
	iPosX = max(iPosX, dfRANGE_MOVE_LEFT);
	iPosY = min(iPosY, dfRANGE_MOVE_BOTTOM);
	iPosY = max(iPosY, dfRANGE_MOVE_TOP);
	
	*pOutX = iPosX;
	*pOutY = iPosY;

	return iActionFrame;

}
////////////////////////////////////////////////////////////////
// �׼� ���� �Լ� 
// Recv ��Ŷ ����
////////////////////////////////////////////////////////////////

void CreateCharacter(st_CLIENT *pClient, DWORD &id, BYTE &Direction, WORD &X, WORD &Y, BYTE &HP)
{
	g_Success++;

	st_PLAYER *pPlayer = new st_PLAYER;
	pPlayer->dwClientNo = pClient->dwClientNo;
	pPlayer->dwPlayerNo = id;

	pPlayer->pClient = pClient;

	pPlayer->dwPlayerNo = pClient->dwClientNo;

	pPlayer->dwAction = dfACTION_STAND;
	pPlayer->dwActionTick = timeGetTime();
	pPlayer->byDirection = Direction;

	pPlayer->woX = X;
	pPlayer->woY = Y;

	g_PlayerMap.insert({ pPlayer->dwPlayerNo, pPlayer });
}


void DeleteCharacter(DWORD &UserNo, DWORD &PlayerNo)
{
	g_Success--;
	g_Fail++;

	st_PLAYER *pPlayer = FindPlayer(PlayerNo);
	if (pPlayer == nullptr)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"Player Not Found %u \n", UserNo);
		return;
	}

	DisconnectClient(pPlayer->dwClientNo);
	g_PlayerMap.erase(pPlayer->dwPlayerNo);
	
	delete pPlayer;
}
//////////////// ����Ŭ���̾�Ʈ�� ��¥�� �������� �ʴ´�. ����� �����̴� �ð��� �����ϰ�, ���巹Ŀ��
void CharacterMoveStart(st_PLAYER *pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}
	// ������ ����, ����ð��� �׼ǽ��۽ð����� ���Ѵ�.
	pPlayer->dwAction = rand() % (dfPACKET_MOVE_DIR_LD + 1);

	
	switch (pPlayer->dwAction)
	{
	case dfACTION_MOVE_LL:
	case dfACTION_MOVE_LU:
	case dfACTION_MOVE_LD:
		pPlayer->dwAction = dfDIR_LEFT;
		break;
	case dfACTION_MOVE_RU:
	case dfACTION_MOVE_RR:
	case dfACTION_MOVE_RD:
		pPlayer->dwAction = dfDIR_RIGHT;
		break;
	}


	CSerializeBuffer *pBuffer = &pPlayer->pClient->SendQ;
	MakeMoveStart(pPlayer, pBuffer);

	pPlayer->dwActionTick = timeGetTime();
}

void CharacterMoveStop(st_PLAYER *pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}

	int X, Y;
	DeadReckoning(pPlayer->dwAction, pPlayer->dwActionTick, pPlayer->woX, pPlayer->woY, &X, &Y);

	pPlayer->dwAction = dfACTION_STAND;
	pPlayer->woX = X;
	pPlayer->woY = Y;

	CSerializeBuffer *pBuffer = &pPlayer->pClient->SendQ;
	MakeMoveStop(pPlayer, pBuffer);

	pPlayer->dwActionTick = timeGetTime();
}

void CharacterAttack1(st_PLAYER *pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}


	pPlayer->dwAction = dfPACKET_CS_ATTACK1;

	CSerializeBuffer *pBuffer = &pPlayer->pClient->SendQ;
	MakeAttack1(pPlayer, pBuffer);

	pPlayer->dwActionTick = timeGetTime();
}

void CharacterAttack2(st_PLAYER *pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->dwAction = dfPACKET_CS_ATTACK2;


	CSerializeBuffer *pBuffer = &pPlayer->pClient->SendQ;
	MakeAttack2(pPlayer, pBuffer);

	pPlayer->dwActionTick = timeGetTime();
}

void CharacterAttack3(st_PLAYER *pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->dwAction = dfPACKET_CS_ATTACK3;

	CSerializeBuffer *pBuffer = &pPlayer->pClient->SendQ;
	MakeAttack3(pPlayer, pBuffer);

	pPlayer->dwActionTick = timeGetTime();
}

void CharcterSync(DWORD &iD, WORD &X, WORD &Y)
{
	st_PLAYER *pPlayer = FindPlayer(iD);

	if (pPlayer == nullptr)
		return;


	_LOG(dfLOG_LEVEL_WARNING, L" Character Sync Packet] Player : %u  Xpos : %d : Ypos : %d => [%d , %d]", iD, pPlayer->woX, pPlayer->woY, X, Y);
	wprintf(L" Character Sync Packet] Player : %u  Xpos : %d : Ypos : %d => [%d , %d]", iD, pPlayer->woX, pPlayer->woY, X, Y);

	pPlayer->woX = X;
	pPlayer->woY = Y;
}

void Echo()
{
	// ���ڴ� ù��° UserNo�� ������� �Ҳ���. & ActionTick���� ���Խ�Ű�� �ʴ´�.
	DWORD UserNo = 1;
	st_PLAYER *pPlayer = FindPlayer(UserNo);

	if (pPlayer == nullptr)
		return;

	DWORD ClientTick = timeGetTime();

	MakeEcho(&pPlayer->pClient->SendQ, ClientTick);

}