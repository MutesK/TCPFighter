#include "Sector.h"
#include "ContentProc.h"

list<st_PLAYER *> g_Sector[dfSECTOR_MAX_Y][dfSECTOR_MAX_X];


void Sector_AddCharacter(st_PLAYER * pPlayer)
{
	if (pPlayer == nullptr)
		return;

	if (pPlayer->CurPos.iX != -1 || pPlayer->CurPos.iY != -1)
		return;

	int iSectorX = pPlayer->woX / dfSECTOR_WIDTH;
	int iSectorY = pPlayer->woY / dfSECTOR_HEIGHT;

	if (iSectorX >= dfSECTOR_MAX_X || iSectorY >= dfSECTOR_MAX_Y)
		return;

	g_Sector[iSectorY][iSectorX].push_back(pPlayer);
	pPlayer->OldPos.iX = pPlayer->CurPos.iX = iSectorX;
	pPlayer->OldPos.iY = pPlayer->CurPos.iY = iSectorY;

}

void Sector_RemoveCharacter(st_PLAYER *pPlayer)
{
	if (pPlayer == nullptr)
		return;

	if (pPlayer->CurPos.iX == -1 || pPlayer->CurPos.iY == -1)
		return;

	list<st_PLAYER *> *pList = &g_Sector[pPlayer->CurPos.iY][pPlayer->CurPos.iX];
	//_OUTPUTDEBUG(L"Sector RemoveCharacter [%d %d]\n", pPlayer->CurPos.iY, pPlayer->CurPos.iX);

	auto iter = pList->begin();

	while (iter != pList->end())
	{
		if (pPlayer == (*iter))
		{
			pList->erase(iter);
			break;
		}
		iter++;
	}

	pPlayer->OldPos.iX = pPlayer->CurPos.iX;
	pPlayer->OldPos.iY = pPlayer->CurPos.iY;
	pPlayer->CurPos.iX = -1;
	pPlayer->CurPos.iY = -1;
}

bool PlayerSectorUpdate(st_PLAYER *pPlayer)
{
	if (pPlayer == nullptr)
		return false;

	int OldSectorX = pPlayer->CurPos.iX;
	int OldSectorY = pPlayer->CurPos.iY;

	int iNewSectorX = pPlayer->woX / dfSECTOR_WIDTH;
	int iNewSectorY = pPlayer->woY / dfSECTOR_HEIGHT;

	if (iNewSectorX == OldSectorX && iNewSectorY == OldSectorY)
		return false;

	if (iNewSectorX < 0 || iNewSectorX > dfSECTOR_MAX_X || iNewSectorY < 0 || iNewSectorY > dfSECTOR_MAX_Y)
		return false;

	Sector_RemoveCharacter(pPlayer);
	Sector_AddCharacter(pPlayer);

	pPlayer->CurPos.iX = iNewSectorX; 
	pPlayer->CurPos.iY = iNewSectorY;
	pPlayer->OldPos.iX = OldSectorX;
	pPlayer->OldPos.iY = OldSectorY;

	return true;

}

void GetSectorAround(int iSectorX, int iSectorY, st_SECTOR_AROUND *pSectorAround)
{
	int iCntX, iCntY;

	iSectorX--;
	iSectorY--;

	pSectorAround->iCount = 0;

	for (iCntY = 0; iCntY < 3; iCntY++)
	{
		if ((iSectorY + iCntY) < 0 || (iSectorY + iCntY) >= dfSECTOR_MAX_Y)
			continue;
		for (iCntX = 0; iCntX < 3; iCntX++)
		{
			if ((iSectorX + iCntX) < 0 || (iSectorX + iCntX) >= dfSECTOR_MAX_X)
				continue;

			pSectorAround->Around[pSectorAround->iCount].iX = iSectorX + iCntX;
			pSectorAround->Around[pSectorAround->iCount].iY = iSectorY + iCntY;
			pSectorAround->iCount++;
		}
	}
}

void GetUpdateSectorAround(st_PLAYER *pPlayer, st_SECTOR_AROUND *pRemoveSector, st_SECTOR_AROUND *pAddSector)
{
	if (pPlayer == nullptr)
		return;

	st_SECTOR_AROUND OldSectorAround, CurSectorAround;

	OldSectorAround.iCount = 0;
	CurSectorAround.iCount = 0;

	pRemoveSector->iCount = 0;
	pAddSector->iCount = 0;

	GetSectorAround(pPlayer->OldPos.iX, pPlayer->OldPos.iY, &OldSectorAround);
	GetSectorAround(pPlayer->CurPos.iX, pPlayer->CurPos.iY, &CurSectorAround);

	// 이전 섹터 중 신규섹터에 없는 정보를 찾은뒤에 RemoveSector에 넣는다.
	for (int iCntOld = 0; iCntOld < OldSectorAround.iCount; iCntOld++)
	{
		bool bFind = false;
		for (int iCntCur = 0; iCntCur < CurSectorAround.iCount; iCntCur++)
		{
			if (OldSectorAround.Around[iCntOld].iX == CurSectorAround.Around[iCntCur].iX &&
				OldSectorAround.Around[iCntOld].iY == CurSectorAround.Around[iCntCur].iY)
			{
				bFind = true;
				break;
			}
		}
		if (bFind == false) 
		{
			pRemoveSector->Around[pRemoveSector->iCount] = OldSectorAround.Around[iCntOld];
			pRemoveSector->iCount++;
		}
	}

	// 현재 섹터정보중 이전 섹터에 없는 정보를 찾아서 AddSector에 넣는다.
	for (int iCntCur = 0; iCntCur < CurSectorAround.iCount; iCntCur++)
	{
		bool bFind = false;
		for (int iCntOld = 0; iCntOld < OldSectorAround.iCount; iCntOld++)
		{
			if (OldSectorAround.Around[iCntOld].iX == CurSectorAround.Around[iCntCur].iX &&
				OldSectorAround.Around[iCntOld].iY == CurSectorAround.Around[iCntCur].iY)
			{
				bFind = true;
				break;
			}
		}
		if (bFind == false)
		{
			pAddSector->Around[pAddSector->iCount] = CurSectorAround.Around[iCntCur];
			pAddSector->iCount++;
		}
	}

}

void SearchColisionSectorAround(st_SECTOR_AROUND *pArea, RECT *pAttackArea, list<st_PLAYER *> *pList)
{
	for (int i = 0; i < pArea->iCount; i++)
	{
		SearchColisionAreaSector(&pArea->Around[i], pAttackArea, pList);
	}
}
void SearchColisionAreaSector(st_SECTOR_POS *pArea, RECT *pAttackArea, list<st_PLAYER *> *pList)
{
	if (pArea == nullptr || pAttackArea == nullptr || pList == nullptr)
		return;

	_OUTPUTDEBUG(L"SearchColisionAreaSector [%d %d]\n", pArea->iY, pArea->iX);
	list <st_PLAYER *> &list = g_Sector[pArea->iY][pArea->iX];


	auto begin = list.begin();
	auto end = list.end();

	list.sort(Comp);

	for (auto iter = begin; iter != end; ++iter)
	{
		st_PLAYER *pPlayer = (*iter);

		RECT PlayerArea;
		PlayerArea.left = pPlayer->woX - 42;
		PlayerArea.top = pPlayer->woY - dfSPRITE_MIDPOINT_CHARETER_y;
		PlayerArea.right = pPlayer->woX + 42;
		PlayerArea.bottom = pPlayer->woY;

		// 이 좌표 하나하나가 포함이 살짝이라도 되어 있다면 추가
		if (PlayerArea.top > pAttackArea->bottom)
			continue;

		if (PlayerArea.bottom < pAttackArea->top)
			continue;

		if (PlayerArea.left > pAttackArea->right)
			continue;

		if (PlayerArea.right < pAttackArea->left)
			continue;

		pList->push_back(pPlayer);

	}
}

void DebugDisplay()
{
	COORD Pos = { 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);

	for (int y = 0; y < dfSECTOR_MAX_Y; y++)
	{
		for (int x = 0; x < dfSECTOR_MAX_X; x++)
		{
			wprintf(L"%d ", g_Sector[y][x].size());
		}
		wprintf(L"\n");
	}
}

bool Comp(st_PLAYER* first, st_PLAYER * sec)
{
	if (first->woY < sec->woY)
		return true;
	else
	{
		if (first->woY == sec->woY)
		{
			if (first->woX <= sec->woX)
				return true;
		}
	}

	return false;
}