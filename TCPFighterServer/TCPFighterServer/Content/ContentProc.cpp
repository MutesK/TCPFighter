#include "ContentProc.h"
#include "../Network/MakePacket.h"
#include "../Network/SendPacket.h"


///////////////////////////////////////////////
// Key : Ŭ���̾�Ʈ ��ȣ
// Value : �÷��̾� ������
///////////////////////////////////////////////
map <DWORD, st_PLAYER *> g_PlayerMap;
DWORD dwFPSCount = 0;

extern list<st_PLAYER *> g_Sector[][dfSECTOR_MAX_X];

st_PLAYER * FindPlayer(DWORD &ClientNo)
{
	auto Finditer = g_PlayerMap.find(ClientNo);

	if (Finditer != g_PlayerMap.end())
		return Finditer->second;

	return nullptr;
}


void Update()
{
	// ������ üũ �Ѵ�.

	while (dwFPSCount < FPS)
	{
		dwFPSCount++;

		auto begin = g_PlayerMap.begin();
		auto end = g_PlayerMap.end();

		for (auto iter = begin; iter != end; ++iter)
		{
			st_PLAYER *pPlayer = (*iter).second;

			UpdateAction(pPlayer);

			if (pPlayer->chHP <= 0)
			{
				//DeleteCharacter(pPlayer->dwClientNo);
			}

			DWORD dwTick = GetTickCount64();

			if (dwTick - pPlayer->pClient->dwRecvTick > dfNETWORK_PACKET_RECV_TIMEOUT)
			{
				//DeleteCharacter(pPlayer->dwClientNo);
				continue;
			}
		}
	}
}

void UpdateAction(st_PLAYER *pPlayer)
{
	if (pPlayer->dwAction >= dfACTION_MOVE_LL && pPlayer->dwAction <= dfACTION_MOVE_LD)
	{
		UpdateMove(pPlayer);
	}

}

void UpdateMove(st_PLAYER *pPlayer)
{
	// woAction ��ǥ�� ������Ʈ �Ѵ�.
	// ��ǥ ���濡 ���ؼ� ���Ͱ� ����ɼ� �ִ�. CurPos�� ������Ʈ�Ѵ�. 

	switch (pPlayer->dwAction)
	{
	case dfACTION_MOVE_LL:
		pPlayer->woX = max(0, pPlayer->woX - dfSPEED_PLAYER_X);
		break;
	case dfACTION_MOVE_LU:
		pPlayer->woX = max(0, pPlayer->woX - dfSPEED_PLAYER_X);
		pPlayer->woY = max(0, pPlayer->woY - dfSPEED_PLAYER_Y);
		break;
	case dfACTION_MOVE_UU:
		pPlayer->woY = max(0, pPlayer->woY - dfSPEED_PLAYER_Y);
		break;
	case dfACTION_MOVE_RU:
		pPlayer->woX = min(dfRANGE_MOVE_RIGHT, pPlayer->woX + dfSPEED_PLAYER_X);
		pPlayer->woY = max(0, pPlayer->woY - dfSPEED_PLAYER_Y);
		break;
	case dfACTION_MOVE_RR:
		pPlayer->woX = min(dfRANGE_MOVE_RIGHT, pPlayer->woX + dfSPEED_PLAYER_X);
		break;
	case dfACTION_MOVE_RD:
		pPlayer->woX = min(dfRANGE_MOVE_RIGHT, pPlayer->woX + dfSPEED_PLAYER_X);
		pPlayer->woY = min(dfRANGE_MOVE_BOTTOM, pPlayer->woY + dfSPEED_PLAYER_Y);
		break;
	case dfACTION_MOVE_DD:
		pPlayer->woY = min(dfRANGE_MOVE_BOTTOM, pPlayer->woY + dfSPEED_PLAYER_Y);
		break;
	case dfACTION_MOVE_LD:
		pPlayer->woX = max(0, pPlayer->woX - dfSPEED_PLAYER_X);
		pPlayer->woY = min(dfRANGE_MOVE_BOTTOM, pPlayer->woY + dfSPEED_PLAYER_Y);
		break;
	}

	if (PlayerSectorUpdate(pPlayer))
	{
		ReadySendUpdate(pPlayer);
	}
	
}
void ReadySendUpdate(st_PLAYER *pPlayer)
{
	st_SECTOR_AROUND AddSector, RemoveSector;
	CSerializeBuffer Buffer;
	int iCnt;
	
	_OUTPUTDEBUG(L"RemoveSector SendSectorAround \n");
	GetUpdateSectorAround(pPlayer, &RemoveSector, &AddSector);
	


	MakeDeleteCharacter(pPlayer, &Buffer);
	// 1. RemoveSector�� �ɸ��� ���� ��Ŷ ������
	SendSectorAround(&RemoveSector, pPlayer, &Buffer);

	

	// 2. ���� �����̴� �༮����, RemoveSector�� �ɸ��͵� ���� ��Ŷ ������
	for (iCnt = 0; iCnt < RemoveSector.iCount; iCnt++)
	{
		Buffer.Clear();
		_OUTPUTDEBUG(L"ReadySendUpdate : RemoveSector [%d %d] \n", RemoveSector.Around[iCnt].iY, RemoveSector.Around[iCnt].iX);
		
		if (RemoveSector.Around[iCnt].iX == -1 || RemoveSector.Around[iCnt].iY == -1)
			continue;

		list <st_PLAYER *> *pList = &g_Sector[RemoveSector.Around[iCnt].iY][RemoveSector.Around[iCnt].iX];

		for (auto iter = pList->begin(); iter != pList->end(); ++iter)
		{
			if ((*iter) != pPlayer)
			{
				MakeDeleteCharacter((*iter), &Buffer);
				SendUniCast(pPlayer, &Buffer);
			}
		}
	}
	
	_OUTPUTDEBUG(L"AddSector Player Create \n");

	// 3. AddSector�� �ɸ��� ���� ��Ŷ ������ -> ���⼭ �ڲ� ������ �����.... (���϶� �𼭸����� ������ �����.)
	Buffer.Clear();
	MakeCreateOtherCharacter(pPlayer, &Buffer);
	SendSectorAround(&AddSector, pPlayer, &Buffer);
	
	_OUTPUTDEBUG(L"AddSector Player Move Create \n");

	// 3-1. AddSector�� ������ �ɸ��� �̵� ��Ŷ ������
	Buffer.Clear();
	MakeMoveStart(pPlayer, &Buffer);
	SendSectorAround(&AddSector, pPlayer, &Buffer);
	

	// 4. �̵��� ��ƿ��� AddSector �ɸ��͵��� ����.
	Buffer.Clear();
	for (iCnt = 0; iCnt < AddSector.iCount; iCnt++)
	{
		_OUTPUTDEBUG(L"ReadySendUpdate AddSecter [%d %d]\n", AddSector.Around[iCnt].iY, AddSector.Around[iCnt].iX);

		if (AddSector.Around[iCnt].iX <= -1  || AddSector.Around[iCnt].iX > dfSECTOR_MAX_X || AddSector.Around[iCnt].iY <= -1 ||
			AddSector.Around[iCnt].iY > dfSECTOR_MAX_Y)
			continue;

		list <st_PLAYER *> *pList = &g_Sector[AddSector.Around[iCnt].iY][AddSector.Around[iCnt].iX];


		for (auto iter = pList->begin(); iter != pList->end(); ++iter)
		{
			st_PLAYER * pExistPlayer = (*iter);

			if (pExistPlayer == pPlayer)
				continue;

			MakeCreateOtherCharacter(pExistPlayer, &Buffer);
			SendUniCast(pPlayer, &Buffer);


			Buffer.Clear();
			switch (pExistPlayer->dwAction)
			{
			case dfACTION_MOVE_LL:
			case dfACTION_MOVE_LU:
			case dfACTION_MOVE_UU:
			case dfACTION_MOVE_RU:
			case dfACTION_MOVE_RR:
			case dfACTION_MOVE_RD:
			case dfACTION_MOVE_DD:
			case dfACTION_MOVE_LD:
				MakeMoveStart(pExistPlayer, &Buffer);
				SendUniCast(pPlayer, &Buffer);
				break;
			case dfACTION_ATTACK1:
				MakeAttack1(pExistPlayer, &Buffer);
				SendUniCast(pPlayer, &Buffer);
				break;
			case dfACTION_ATTACK2:
				MakeAttack2(pExistPlayer, &Buffer);
				SendUniCast(pPlayer, &Buffer);
				break;
			case dfACTION_ATTACK3:
				MakeAttack3(pExistPlayer, &Buffer);
				SendUniCast(pPlayer, &Buffer);
				break;
			}
		}
	}
}

int DeadReckoning(DWORD dwAction, DWORD dwActionTick, int OldPosX, int OldPosY, int *pOutX, int *pOutY)
{
	DWORD dwIntervalTick = GetTickCount64() - dwActionTick;

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

void CreateCharacter(st_CLIENT *pClient)
{
	st_PLAYER *pPlayer = new st_PLAYER;
	memset(pPlayer, -1, sizeof(st_PLAYER));
	pPlayer->pClient = std::move(pClient); // ���� �ʼ������� üũ�ʿ�
	pPlayer->dwClientNo = pClient->dwClientNo;

	pPlayer->dwAction = dfACTION_STAND;
	pPlayer->dwActionTick = 0;
	pPlayer->byDirection = dfDIR_LEFT;

	pPlayer->woX = rand() % dfRANGE_MOVE_RIGHT;
	pPlayer->woY =	rand() % dfRANGE_MOVE_BOTTOM;
	pPlayer->woActionX = pPlayer->woX;
	pPlayer->woActionY = pPlayer->woY;

	pPlayer->chHP = 100;

	// ���Ϳ� ����Ѵ�.
	Sector_AddCharacter(pPlayer);

	CSerializeBuffer Buffer;
	MakeCreateMyCharacter(pPlayer, &Buffer);
	SendUniCast(pPlayer, &Buffer);

	
	Buffer.Clear();
	// MakeCreateMyCharacter ��Ŷ , MakeCreateOtherCharacter ��Ŷ�� �����.
	MakeCreateOtherCharacter(pPlayer, &Buffer);
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
	SendSectorAround(&pPlayer->Around, pPlayer, &Buffer);

	
	for (int i = 0; i < pPlayer->Around.iCount; i++)
	{
		_OUTPUTDEBUG(L"CreateCharacter  [%d %d]\n", pPlayer->Around.Around[i].iY, pPlayer->Around.Around[i].iX);
		_OUTPUTDEBUG(L"CreateCharacter Player No : %d \n", pPlayer->dwClientNo);

		if (pPlayer->Around.Around[i].iX || pPlayer->Around.Around[i].iY == -1)
			continue;

		list<st_PLAYER *> *pList = &g_Sector[pPlayer->Around.Around[i].iY][pPlayer->Around.Around[i].iX];
		Buffer.Clear();
		for (auto iter = pList->begin(); iter != pList->end(); ++iter)
		{
			if ((*iter)->pClient != pPlayer->pClient)
			{
				MakeCreateOtherCharacter((*iter), &Buffer);
				SendUniCast(pPlayer, &Buffer);
			}
		}
	}

	
	g_PlayerMap.insert({ pPlayer->dwClientNo, pPlayer });
}

// �ɸ��� �����ϴ� �Լ�
// �ش� ������� ����� �ȴ�.
void DeleteCharacter(DWORD &UserNo)
{
	st_PLAYER *pPlayer = FindPlayer(UserNo);

	DisconnectClient(UserNo);
	_OUTPUTDEBUG(L"Disconnect Player No : %d \n", pPlayer->dwClientNo);

	if (pPlayer == nullptr)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# DeleteCharacter > Session ID : %d Player isn't Not Found!", UserNo);
		DisconnectClient(pPlayer->dwClientNo);

		// ����� ���
		return;	// �����, �α�
	}
	/////////////////////////////////////////////////////////
	CSerializeBuffer SectorAround(dfRECV_BUFF);
	MakeDeleteCharacter(pPlayer, &SectorAround);

	/////////////////////////////////////////////////////////
	// �ϴ� ���Ϳ��� ���´�.
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
	Sector_RemoveCharacter(pPlayer);
	SendSectorAround(&pPlayer->Around, nullptr, &SectorAround);

	auto iter = g_PlayerMap.find(UserNo);
	if (iter == g_PlayerMap.end())
		return;

	g_PlayerMap.erase(UserNo);

	delete pPlayer;
	
}

void CharacterMoveStart(st_CLIENT *pClient, stPACKET_CS_MOVE_START *pPacket)
{
	_LOG(dfLOG_LEVEL_WARNING, L"# MOVESTART # Session ID : %d / Direction : %d / X : %d / Y : %d",
		pClient->dwClientNo, pPacket->Direction, pPacket->X, pPacket->Y);

	st_PLAYER *pPlayer = FindPlayer(pClient->dwClientNo);


	if (pPlayer == nullptr)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# MoveStart > Session ID %d Player Not Found", pClient->dwClientNo);
		DisconnectClient(pClient->dwClientNo);
		return;
	}

	if (abs(pPlayer->woX - pPacket->X) > dfERRORRANGE || abs(pPlayer->woY - pPacket->Y) > dfERRORRANGE)
	{
		int wdrX, wdrY;

		int iDeadFrame = DeadReckoning(pPlayer->dwAction, pPlayer->dwActionTick, pPlayer->woActionX, pPlayer->woActionY, &wdrX, &wdrY);

		if (abs(wdrX - pPacket->X) > dfERRORRANGE || abs(wdrY - pPacket->Y) > dfERRORRANGE)
		{
			CSerializeBuffer Buffer;
			MakeSync(pPlayer, &Buffer, wdrX, wdrY);
			GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
			SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
		}
		pPacket->X = wdrX;
		pPacket->Y = wdrY;
	}

	pPlayer->dwAction = pPacket->Direction;
	pPlayer->byDirection = pPacket->Direction;

	switch (pPacket->Direction)
	{
	case dfACTION_MOVE_LL:
	case dfACTION_MOVE_LU:
	case dfACTION_MOVE_LD:
		pPlayer->byDirection = dfACTION_MOVE_LL;
		break;
	case dfACTION_MOVE_RU:
	case dfACTION_MOVE_RR:
	case dfACTION_MOVE_RD:
		pPlayer->byDirection = dfACTION_MOVE_RR;
		break;
	}

	pPlayer->woX = pPacket->X;
	pPlayer->woY = pPacket->Y;

	if (PlayerSectorUpdate(pPlayer))
	{
		ReadySendUpdate(pPlayer);
	}

	pPlayer->dwActionTick = GetTickCount64();
	pPlayer->woActionX = pPlayer->woX;
	pPlayer->woActionY = pPlayer->woY;


	CSerializeBuffer Buffer;
	MakeMoveStart(pPlayer, &Buffer);
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
	SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
}

void CharacterMoveStop(st_CLIENT *pClient, stPACKET_CS_MOVE_STOP *pPacket)
{
	_LOG(dfLOG_LEVEL_WARNING, L"# MOVESTOP # Session ID : %d / Direction : %d / X : %d / Y : %d",
		pClient->dwClientNo, pPacket->Direction, pPacket->X, pPacket->Y);

	st_PLAYER *pPlayer = FindPlayer(pClient->dwClientNo);


	if (pPlayer == nullptr)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# MOVESTOP > Session ID %d Player Not Found", pClient->dwClientNo);
		DisconnectClient(pClient->dwClientNo);
		return;
	}

	if (abs(pPlayer->woX - pPacket->X) > dfERRORRANGE || abs(pPlayer->woY - pPacket->Y) > dfERRORRANGE)
	{
		int wdrX, wdrY;

		int iDeadFrame = DeadReckoning(pPlayer->dwAction, pPlayer->dwActionTick, pPlayer->woActionX, pPlayer->woActionY, &wdrX, &wdrY);

		if (abs(wdrX - pPacket->X) > dfERRORRANGE || abs(wdrY - pPacket->Y) > dfERRORRANGE)
		{
			CSerializeBuffer Buffer;
			MakeSync(pPlayer, &Buffer, wdrX, wdrY);
			GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
			SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
		}
		pPacket->X = wdrX;
		pPacket->Y = wdrY;
	}

	pPlayer->dwAction = dfACTION_STAND;
	pPlayer->byDirection = pPacket->Direction;

	pPlayer->woX = pPacket->X;
	pPlayer->woY = pPacket->Y;

	if (PlayerSectorUpdate(pPlayer))
	{
		ReadySendUpdate(pPlayer);
	}

	pPlayer->dwActionTick = GetTickCount64();
	pPlayer->woActionX = pPlayer->woX;
	pPlayer->woActionY = pPlayer->woY;

	CSerializeBuffer Buffer;
	MakeMoveStop(pPlayer, &Buffer);
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
	SendSectorAround(&pPlayer->Around, nullptr, &Buffer);

}


void CharacterAttack1(st_CLIENT *pClient, stPACKET_CS_ATTACK1 *pPacket)
{
	_LOG(dfLOG_LEVEL_WARNING, L"# ATTACK1 # Session ID : %d / Direction : %d / X : %d / Y : %d",
		pClient->dwClientNo, pPacket->Direction, pPacket->X, pPacket->Y);

	st_PLAYER *pPlayer = FindPlayer(pClient->dwClientNo);
	list <st_PLAYER *> DemageList;

	if (pPlayer == nullptr)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# ATTACK1 > Session ID %d Player Not Found", pClient->dwClientNo);
		DisconnectClient(pClient->dwClientNo);
		return;
	}

	if (abs(pPlayer->woX - pPacket->X) > dfERRORRANGE || abs(pPlayer->woY - pPacket->Y) > dfERRORRANGE)
	{
		int wdrX, wdrY;

		int iDeadFrame = DeadReckoning(pPlayer->dwAction, pPlayer->dwActionTick, pPlayer->woActionX, pPlayer->woActionY, &wdrX, &wdrY);

		if (abs(wdrX - pPacket->X) > dfERRORRANGE || abs(wdrY - pPacket->Y) > dfERRORRANGE)
		{
			CSerializeBuffer Buffer;
			MakeSync(pPlayer, &Buffer, wdrX, wdrY);
			GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
			SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
		}
		pPacket->X = wdrX;
		pPacket->Y = wdrY;
	}

	pPlayer->dwAction = dfACTION_STAND;
	pPlayer->byDirection = pPacket->Direction;

	pPlayer->woX = pPacket->X;
	pPlayer->woY = pPacket->Y;

	if (PlayerSectorUpdate(pPlayer))
	{
		ReadySendUpdate(pPlayer);
	}


	pPlayer->dwActionTick = GetTickCount64();
	pPlayer->woActionX = pPlayer->woX;
	pPlayer->woActionY = pPlayer->woY;

	PlayerColisionCheck(pPlayer, dfACTION_ATTACK1, &DemageList);

	CSerializeBuffer Buffer;
	MakeAttack1(pPlayer, &Buffer);
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
	SendSectorAround(&pPlayer->Around, nullptr, &Buffer);

	Buffer.Clear();

	for(auto iter = DemageList.begin(); iter != DemageList.end(); ++iter)
	{
		(*iter)->chHP -= 4;
		MakeDamage(pPlayer, (*iter), &Buffer);
		SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
	}
}

void CharacterAttack2(st_CLIENT *pClient, stPACKET_CS_ATTACK2 *pPacket)
{
	_LOG(dfLOG_LEVEL_WARNING, L"# ATTACK2 # Session ID : %d / Direction : %d / X : %d / Y : %d",
		pClient->dwClientNo, pPacket->Direction, pPacket->X, pPacket->Y);

	st_PLAYER *pPlayer = FindPlayer(pClient->dwClientNo);
	list <st_PLAYER *> DemageList;

	if (pPlayer == nullptr)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# ATTACK2 > Session ID %d Player Not Found", pClient->dwClientNo);
		DisconnectClient(pClient->dwClientNo);
		return;
	}

	if (abs(pPlayer->woX - pPacket->X) > dfERRORRANGE || abs(pPlayer->woY - pPacket->Y) > dfERRORRANGE)
	{
		int wdrX, wdrY;

		int iDeadFrame = DeadReckoning(pPlayer->dwAction, pPlayer->dwActionTick, pPlayer->woActionX, pPlayer->woActionY, &wdrX, &wdrY);

		if (abs(wdrX - pPacket->X) > dfERRORRANGE || abs(wdrY - pPacket->Y) > dfERRORRANGE)
		{
			CSerializeBuffer Buffer;
			MakeSync(pPlayer, &Buffer, wdrX, wdrY);
			GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
			SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
		}
		pPacket->X = wdrX;
		pPacket->Y = wdrY;
	}

	pPlayer->dwAction = dfACTION_STAND;
	pPlayer->byDirection = pPacket->Direction;

	pPlayer->woX = pPacket->X;
	pPlayer->woY = pPacket->Y;

	if (PlayerSectorUpdate(pPlayer))
	{
		ReadySendUpdate(pPlayer);
	}

	PlayerColisionCheck(pPlayer, dfACTION_ATTACK2, &DemageList);

	CSerializeBuffer Buffer;
	MakeAttack2(pPlayer, &Buffer);
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
	SendSectorAround(&pPlayer->Around, nullptr, &Buffer);

	Buffer.Clear();

	for (auto iter = DemageList.begin(); iter != DemageList.end(); ++iter)
	{
		(*iter)->chHP -= 5;
		MakeDamage(pPlayer, (*iter), &Buffer);
		SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
	}

}

void CharacterAttack3(st_CLIENT *pClient, stPACKET_CS_ATTACK3 *pPacket)
{
	_LOG(dfLOG_LEVEL_WARNING, L"# ATTACK3 # Session ID : %d / Direction : %d / X : %d / Y : %d",
		pClient->dwClientNo, pPacket->Direction, pPacket->X, pPacket->Y);

	st_PLAYER *pPlayer = FindPlayer(pClient->dwClientNo);
	list <st_PLAYER *> DemageList;

	if (pPlayer == nullptr)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# ATTACK3 > Session ID %d Player Not Found", pClient->dwClientNo);
		DisconnectClient(pClient->dwClientNo);
		return;
	}

	if (abs(pPlayer->woX - pPacket->X) > dfERRORRANGE || abs(pPlayer->woY - pPacket->Y) > dfERRORRANGE)
	{
		int wdrX, wdrY;

		int iDeadFrame = DeadReckoning(pPlayer->dwAction, pPlayer->dwActionTick, pPlayer->woActionX, pPlayer->woActionY, &wdrX, &wdrY);

		if (abs(wdrX - pPacket->X) > dfERRORRANGE || abs(wdrY - pPacket->Y) > dfERRORRANGE)
		{
			CSerializeBuffer Buffer;
			MakeSync(pPlayer, &Buffer, wdrX, wdrY);
			GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
			SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
		}
		pPacket->X = wdrX;
		pPacket->Y = wdrY;
	}

	pPlayer->dwAction = dfACTION_STAND;
	pPlayer->byDirection = pPacket->Direction;

	pPlayer->woX = pPacket->X;
	pPlayer->woY = pPacket->Y;

	if (PlayerSectorUpdate(pPlayer))
	{
		ReadySendUpdate(pPlayer);
	}

	PlayerColisionCheck(pPlayer, dfACTION_ATTACK3, &DemageList);

	CSerializeBuffer Buffer;
	MakeAttack3(pPlayer, &Buffer);
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &pPlayer->Around);
	SendSectorAround(&pPlayer->Around, nullptr, &Buffer);

	Buffer.Clear();

	for (auto iter = DemageList.begin(); iter != DemageList.end(); ++iter)
	{
		(*iter)->chHP -= 6;
		MakeDamage(pPlayer, (*iter), &Buffer);
		SendSectorAround(&pPlayer->Around, nullptr, &Buffer);
	}
}

void PlayerColisionCheck(st_PLAYER *pAttacker, BYTE AttackMethod, list<st_PLAYER *> *pOutDemageList)
{
	if (pAttacker == nullptr || pOutDemageList == nullptr)
		return;

	// Ŭ���̾�Ʈ �� ��ǥ
	int PosX = pAttacker->woX;
	int PosY = pAttacker->woY;
	int Direction = pAttacker->byDirection;

	RECT AttackRect;
	
//	int IdleLeft = 42;
//	int IdleTop = dfSPRITE_MIDPOINT_CHARETER_y;
//	int IdleRight = 39;
//	int IdleBottom = 0;

	// ���� ���� 
	switch (AttackMethod)
	{
	case dfACTION_ATTACK1:
	{
		if (Direction == dfDIR_LEFT)
		{
			AttackRect.left = PosX - dfSPRITE_MIDPOINT_CHARETER_x + 10;
			AttackRect.top = PosY - 62;
			AttackRect.right = AttackRect.left + 11;
			AttackRect.bottom = AttackRect.top + 10;
		}
		else
		{
			AttackRect.left = PosX + 69;
			AttackRect.top = PosY - 62;
			AttackRect.right = AttackRect.left + 11;
			AttackRect.bottom = AttackRect.top + 10;
		}
	}
		break;
	case dfACTION_ATTACK2:
	{
		if (Direction == dfDIR_LEFT)
		{
			AttackRect.left = PosX - 58;
			AttackRect.top = PosY - 59;
			AttackRect.right = AttackRect.left + 13;
			AttackRect.bottom = AttackRect.top + 10;
		}
		else
		{
			AttackRect.left = PosX + 65;
			AttackRect.top = PosY - 69;
			AttackRect.right = AttackRect.left + 13;
			AttackRect.bottom = AttackRect.top + 10;
		}
	}
		break;
	case dfACTION_ATTACK3:
	{
		if (Direction == dfDIR_LEFT)
		{
			AttackRect.left = PosX - dfSPRITE_MIDPOINT_CHARETER_x + 10;
			AttackRect.top = PosY - 64;
			AttackRect.right = AttackRect.left + 13;
			AttackRect.bottom = AttackRect.top + 18;
		}
		else
		{
			AttackRect.left = PosX + 55;
			AttackRect.top = PosY - 64;
			AttackRect.right = AttackRect.left + 13;
			AttackRect.bottom = AttackRect.top + 18;
		}
	}
		break;
	}
	GetSectorAround(pAttacker->CurPos.iX, pAttacker->CurPos.iY, &pAttacker->Around);
	SearchColisionSectorAround(&pAttacker->Around, &AttackRect, pOutDemageList);

}