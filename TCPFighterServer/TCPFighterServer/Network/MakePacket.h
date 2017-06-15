#pragma once

#include "NetworkProc.h"
#include "../Content/ContentProc.h"

////////////////////////////////////////////////////////////////
// ������ ��Ŷ�� ������ִ� �Լ���.
////////////////////////////////////////////////////////////////


// �ɸ��� �����ϴ� ��Ŷ ���� ( Unicast)
void MakeCreateMyCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// �ɸ��� �����ϴ� ��Ŷ ���� (pPlayer�� ������ ������ ����)
void MakeCreateOtherCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);


// �ɸ��� ���� ��Ŷ(��ε�ĳ��Ʈ)
void MakeDeleteCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

void MakeDeleteCharacterInSector(st_SECTOR_POS *pPos, st_PLAYER* pExcept, CSerializeBuffer *pBuffer);


// �ɸ��� �̵� ���� ��Ŷ(���� ���� 9��)
void MakeMoveStart(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// �ɸ��� �̵� ���� ��Ŷ(���� ���� 9��)
void MakeMoveStop(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// �ɸ��� ����1 (�ڱ� ����)
void MakeAttack1(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// �ɸ��� ����2 (�ڱ� ����)
void MakeAttack2(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// �ɸ��� ����3 (�ڱ� ����)
void MakeAttack3(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// �ɸ��� ������ ��Ŷ(�ڱ� ����)
void MakeDamage(st_PLAYER *pAttacker, st_PLAYER *pDamager, CSerializeBuffer *pBuffer);

// ����ȭ ��Ŷ( ����ĳ��Ʈ)
void MakeSync(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer, WORD wX, WORD wY);


void MakeEcho(CSerializeBuffer *pBuffer, DWORD& ClientTick);