#pragma once

#include "../Network/NetworkProc.h"
#include "Sector.h"

#define dfERRORRANGE 500
#define dfNETWORK_PACKET_RECV_TIMEOUT 1000000000



////////////////////////////////
// 플레이어 구조체
////////////////////////////////
struct st_PLAYER
{
	st_CLIENT *pClient;
	DWORD dwClientNo;	// 고유 클라이언트 번호

	DWORD dwAction;			// 현재 액션
	DWORD dwActionTick;				// 액션 시작 시간
	BYTE byDirection;		// 보고있는 방향

	int woX;		// 보이는 좌표 (액션 시작시 데드레커닝 계산용 변수)
	int woY;

	int woActionX;		// Update 좌표
	int woActionY;
	
	char chHP;		// 체력

	st_SECTOR_POS CurPos;		// 현재 플레이어에 소속된 섹터
	st_SECTOR_POS OldPos;		// 전에 플레이어가 속했던 섹터

	st_SECTOR_AROUND Around;
};
////////////////////////////////////////////////////////////////
// 일반 컨텐츠 처리
////////////////////////////////////////////////////////////////
st_PLAYER* FindPlayer(DWORD &ClientNo);



////////////////////////////////////////////////////////////////
// 액션 대응 함수 
////////////////////////////////////////////////////////////////
// ####  Recv 패킷
////////////////////////////////////////////////////////////////
// 플레이어 생성
// 생성후 섹터 위치에 따라 그 해당섹터를 포함한 
// 주변 섹터에 패킷전송해줘어야 함.
////////////////////////////////////////////////////////////////
void CharacterMoveStart(st_CLIENT *pClient, stPACKET_CS_MOVE_START *pPacket);
void CharacterMoveStop(st_CLIENT *pClient, stPACKET_CS_MOVE_STOP *pPacket);
void CharacterAttack1(st_CLIENT *pClient, stPACKET_CS_ATTACK1 *pPacket);
void CharacterAttack2(st_CLIENT *pClient, stPACKET_CS_ATTACK2 *pPacket);
void CharacterAttack3(st_CLIENT *pClient, stPACKET_CS_ATTACK3 *pPacket);


////////////////////////////////////////////////////////////////
// Accept 하면 자동으로 호출되는 함수
// 즉 해당 클라이언트에게는 stPACKET_SC_CREATE_MY_CHARACTER를 보내주어야 되고 (UniCast)
// 다른 클라이언트들에게는 stPACKET_SC_CREATE_OTHER_CHARACTER를 보내주어야 됨.(BroadCast)
////////////////////////////////////////////////////////////////
void CreateCharacter(st_CLIENT *pClient);


////////////////////////////////////////////////////////////////
// HeartBeat 또는 체력이 전부 다 소모하였을때 함수 호출
// 전체 클라이언트들에게 전송(BroadCast)
////////////////////////////////////////////////////////////////
void DeleteCharacter(DWORD &UserNo);

////////////////////////////////////////////////////////////////
// 서버상 컨텐츠 처리 함수
////////////////////////////////////////////////////////////////
void Update();
////////////////////////////////////////////////////////////////
// ####  Update 대응 함수
////////////////////////////////////////////////////////////////
void UpdateAction(st_PLAYER *pPlayer);

void UpdateMove(st_PLAYER *pPlayer);

int DeadReckoning(DWORD dwAction, DWORD dwActionTick, int OldPosX,
	int OldPosY, int *pOutX, int *pOutY);


void ReadySendUpdate(st_PLAYER *pPlayer);


void PlayerColisionCheck(st_PLAYER *pAttacker, BYTE AttackMethod, list<st_PLAYER *> *pOutDemageList);
