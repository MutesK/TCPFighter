#pragma once

#include "NetworkProc.h"
#include "../Content/ContentProc.h"

////////////////////////////////////////////////////////////////
// ������ ��Ŷ�� ������ִ� �Լ���.
////////////////////////////////////////////////////////////////

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


void MakeEcho(CSerializeBuffer *pBuffer, DWORD& ClientTick);