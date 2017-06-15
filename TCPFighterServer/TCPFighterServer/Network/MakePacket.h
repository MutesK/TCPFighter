#pragma once

#include "NetworkProc.h"
#include "../Content/ContentProc.h"

////////////////////////////////////////////////////////////////
// 전송할 패킷을 만들어주는 함수들.
////////////////////////////////////////////////////////////////


// 케릭터 생성하는 패킷 생성 ( Unicast)
void MakeCreateMyCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

// 케릭터 생성하는 패킷 생성 (pPlayer를 제외한 나머지 소켓)
void MakeCreateOtherCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);


// 케릭터 삭제 패킷(브로드캐스트)
void MakeDeleteCharacter(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer);

void MakeDeleteCharacterInSector(st_SECTOR_POS *pPos, st_PLAYER* pExcept, CSerializeBuffer *pBuffer);


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

// 케릭터 데미지 패킷(자기 섹터)
void MakeDamage(st_PLAYER *pAttacker, st_PLAYER *pDamager, CSerializeBuffer *pBuffer);

// 동기화 패킷( 유니캐스트)
void MakeSync(st_PLAYER *pPlayer, CSerializeBuffer *pBuffer, WORD wX, WORD wY);


void MakeEcho(CSerializeBuffer *pBuffer, DWORD& ClientTick);