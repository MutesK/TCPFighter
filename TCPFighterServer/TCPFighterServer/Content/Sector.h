#pragma once

#include "../Common/common.h"

#define dfSECTOR_WIDTH 180
#define dfSECTOR_HEIGHT 120


#define dfSECTOR_MAX_X  (dfRANGE_MOVE_RIGHT / dfSECTOR_WIDTH) + 1
#define dfSECTOR_MAX_Y	(dfRANGE_MOVE_BOTTOM / dfSECTOR_HEIGHT) + 1


struct st_PLAYER;

////////////////////////////////
// 섹터의 위치
////////////////////////////////
struct st_SECTOR_POS
{
	int iX;
	int iY;
};

////////////////////////////////
// 섹터 주변 표시를 위한 구조체
////////////////////////////////
struct st_SECTOR_AROUND
{
	int iCount;   // 사용갯수
	st_SECTOR_POS Around[9];
};


// 정렬 기준(공격에 의한 충돌처리하기전에, 해당섹터에 X,Y 오름차순 정렬을 함.
// 이유는 충돌처리할때 충돌 Range 계산을 빠르게 할려고, STL List의 Sort Big O는? (N Log N)
bool Comp(st_PLAYER* first, st_PLAYER * sec);

// 케릭터의 현재 좌표로 섹터 위치를 계산하고 해당 섹터에 넣는다.
void Sector_AddCharacter(st_PLAYER * pPlayer);
// 케릭터의 현재 좌표로 해당 섹터에서 삭제
void Sector_RemoveCharacter(st_PLAYER *pPlayer);

bool PlayerSectorUpdate(st_PLAYER *pPlayer);

// 특정 섹터 좌표 기준 주변 영향권 섹터 얻기
void GetSectorAround(int iSectorX, int iSectorY, st_SECTOR_AROUND *pSectorAround);

// 섹터에서 섹터를 이동하였을때, 섹터 영향권에서 빠진 섹터, 새로 추가된 섹터의 정보를 구한다.
void GetUpdateSectorAround(st_PLAYER *pPlayer, st_SECTOR_AROUND *pRemoveSector, st_SECTOR_AROUND *pAddSector);

void SearchColisionSectorAround(st_SECTOR_AROUND *pAround, RECT *AttackRect, list<st_PLAYER *> *pOutList);
void SearchColisionAreaSector(st_SECTOR_POS *pArea, RECT *pAttackArea, list<st_PLAYER *> *pList);
void DebugDisplay();