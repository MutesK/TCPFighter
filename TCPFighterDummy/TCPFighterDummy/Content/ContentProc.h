#pragma once

#include "../Network/NetworkProc.h"


////////////////////////////////
// 플레이어 구조체
////////////////////////////////
struct st_PLAYER
{
	st_CLIENT *pClient;

	DWORD dwClientNo;
	DWORD dwPlayerNo;	// 고유 클라이언트 번호

	DWORD dwAction;			// 현재 액션
	DWORD dwActionTick;				// 액션 시작 시간
	BYTE byDirection;		// 보고있는 방향

	int woX;		// 보이는 좌표 (액션 시작시 데드레커닝 계산용 변수)
	int woY;
};
////////////////////////////////////////////////////////////////
// 일반 컨텐츠 처리
////////////////////////////////////////////////////////////////
st_PLAYER* FindPlayer(DWORD &ClientNo);

////////////////////////////////////////////////////////////////
// 액션 대응 함수 
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// 플레이어 생성
// 생성후 섹터 위치에 따라 그 해당섹터를 포함한 
// 주변 섹터에 패킷전송해줘어야 함.
////////////////////////////////////////////////////////////////
void CharacterMoveStart(st_PLAYER *);
void CharacterMoveStop(st_PLAYER *);
void CharacterAttack1(st_PLAYER *);
void CharacterAttack2(st_PLAYER *);
void CharacterAttack3(st_PLAYER *);

void CharcterSync(DWORD &iD, WORD &X, WORD &Y);
void Echo(st_PLAYER *);
////////////////////////////////////////////////////////////////
// Accept 하면 자동으로 호출되는 함수
// 즉 해당 클라이언트에게는 stPACKET_SC_CREATE_MY_CHARACTER를 보내주어야 되고 (UniCast)
// 다른 클라이언트들에게는 stPACKET_SC_CREATE_OTHER_CHARACTER를 보내주어야 됨.(BroadCast)
////////////////////////////////////////////////////////////////
void CreateCharacter(st_CLIENT *pClient, DWORD &id, BYTE &Direction, WORD &X, WORD &Y, BYTE &HP);

////////////////////////////////////////////////////////////////
// HeartBeat 또는 체력이 전부 다 소모하였을때 함수 호출
// 전체 클라이언트들에게 전송(BroadCast)
////////////////////////////////////////////////////////////////
void DeleteCharacter(DWORD &UserNo, DWORD &PlayerNo);

void Update();

int DeadReckoning(DWORD dwAction, DWORD dwActionTick, int OldPosX,
	int OldPosY, int *pOutX, int *pOutY);

