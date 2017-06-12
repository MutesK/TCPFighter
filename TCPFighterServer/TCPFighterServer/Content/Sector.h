#pragma once

#include "../Common/common.h"

#define dfSECTOR_WIDTH 180
#define dfSECTOR_HEIGHT 120


#define dfSECTOR_MAX_X  (dfRANGE_MOVE_RIGHT / dfSECTOR_WIDTH) + 1
#define dfSECTOR_MAX_Y	(dfRANGE_MOVE_BOTTOM / dfSECTOR_HEIGHT) + 1


struct st_PLAYER;

////////////////////////////////
// ������ ��ġ
////////////////////////////////
struct st_SECTOR_POS
{
	int iX;
	int iY;
};

////////////////////////////////
// ���� �ֺ� ǥ�ø� ���� ����ü
////////////////////////////////
struct st_SECTOR_AROUND
{
	int iCount;   // ��밹��
	st_SECTOR_POS Around[9];
};


// ���� ����(���ݿ� ���� �浹ó���ϱ�����, �ش缽�Ϳ� X,Y �������� ������ ��.
// ������ �浹ó���Ҷ� �浹 Range ����� ������ �ҷ���, STL List�� Sort Big O��? (N Log N)
bool Comp(st_PLAYER* first, st_PLAYER * sec);

// �ɸ����� ���� ��ǥ�� ���� ��ġ�� ����ϰ� �ش� ���Ϳ� �ִ´�.
void Sector_AddCharacter(st_PLAYER * pPlayer);
// �ɸ����� ���� ��ǥ�� �ش� ���Ϳ��� ����
void Sector_RemoveCharacter(st_PLAYER *pPlayer);

bool PlayerSectorUpdate(st_PLAYER *pPlayer);

// Ư�� ���� ��ǥ ���� �ֺ� ����� ���� ���
void GetSectorAround(int iSectorX, int iSectorY, st_SECTOR_AROUND *pSectorAround);

// ���Ϳ��� ���͸� �̵��Ͽ�����, ���� ����ǿ��� ���� ����, ���� �߰��� ������ ������ ���Ѵ�.
void GetUpdateSectorAround(st_PLAYER *pPlayer, st_SECTOR_AROUND *pRemoveSector, st_SECTOR_AROUND *pAddSector);

void SearchColisionSectorAround(st_SECTOR_AROUND *pAround, RECT *AttackRect, list<st_PLAYER *> *pOutList);
void SearchColisionAreaSector(st_SECTOR_POS *pArea, RECT *pAttackArea, list<st_PLAYER *> *pList);
void DebugDisplay();