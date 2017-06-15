#pragma once

#include "NetworkProc.h"
#include "../Content/ContentProc.h"

////////////////////////////////////////////////////////////////
// 전송할 패킷을 만들어주는 함수들.
////////////////////////////////////////////////////////////////

// 케릭터 이동 시작 패킷(인접 섹터 9개)
void MakeMoveStart(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// 케릭터 이동 중지 패킷(인접 섹터 9개)
void MakeMoveStop(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// 케릭터 공격1 (자기 섹터)
void MakeAttack1(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// 케릭터 공격2 (자기 섹터)
void MakeAttack2(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// 케릭터 공격3 (자기 섹터)
void MakeAttack3(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);


void MakeEcho(CSerializeBuffer *pBuffer, DWORD& ClientTick);