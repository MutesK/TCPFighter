#pragma once

#include "../Network/NetworkProc.h"
#include "Sector.h"

#define dfERRORRANGE 500
#define dfNETWORK_PACKET_RECV_TIMEOUT 1000000000



////////////////////////////////
// �÷��̾� ����ü
////////////////////////////////
struct st_PLAYER
{
	st_CLIENT *pClient;
	DWORD dwClientNo;	// ���� Ŭ���̾�Ʈ ��ȣ

	DWORD dwAction;			// ���� �׼�
	DWORD dwActionTick;				// �׼� ���� �ð�
	BYTE byDirection;		// �����ִ� ����

	int woX;		// ���̴� ��ǥ (�׼� ���۽� ���巹Ŀ�� ���� ����)
	int woY;

	int woActionX;		// Update ��ǥ
	int woActionY;
	
	char chHP;		// ü��

	st_SECTOR_POS CurPos;		// ���� �÷��̾ �Ҽӵ� ����
	st_SECTOR_POS OldPos;		// ���� �÷��̾ ���ߴ� ����

	st_SECTOR_AROUND Around;
};
////////////////////////////////////////////////////////////////
// �Ϲ� ������ ó��
////////////////////////////////////////////////////////////////
st_PLAYER* FindPlayer(DWORD &ClientNo);



////////////////////////////////////////////////////////////////
// �׼� ���� �Լ� 
////////////////////////////////////////////////////////////////
// ####  Recv ��Ŷ
////////////////////////////////////////////////////////////////
// �÷��̾� ����
// ������ ���� ��ġ�� ���� �� �ش缽�͸� ������ 
// �ֺ� ���Ϳ� ��Ŷ���������� ��.
////////////////////////////////////////////////////////////////
void CharacterMoveStart(st_CLIENT *pClient, stPACKET_CS_MOVE_START *pPacket);
void CharacterMoveStop(st_CLIENT *pClient, stPACKET_CS_MOVE_STOP *pPacket);
void CharacterAttack1(st_CLIENT *pClient, stPACKET_CS_ATTACK1 *pPacket);
void CharacterAttack2(st_CLIENT *pClient, stPACKET_CS_ATTACK2 *pPacket);
void CharacterAttack3(st_CLIENT *pClient, stPACKET_CS_ATTACK3 *pPacket);


////////////////////////////////////////////////////////////////
// Accept �ϸ� �ڵ����� ȣ��Ǵ� �Լ�
// �� �ش� Ŭ���̾�Ʈ���Դ� stPACKET_SC_CREATE_MY_CHARACTER�� �����־�� �ǰ� (UniCast)
// �ٸ� Ŭ���̾�Ʈ�鿡�Դ� stPACKET_SC_CREATE_OTHER_CHARACTER�� �����־�� ��.(BroadCast)
////////////////////////////////////////////////////////////////
void CreateCharacter(st_CLIENT *pClient);


////////////////////////////////////////////////////////////////
// HeartBeat �Ǵ� ü���� ���� �� �Ҹ��Ͽ����� �Լ� ȣ��
// ��ü Ŭ���̾�Ʈ�鿡�� ����(BroadCast)
////////////////////////////////////////////////////////////////
void DeleteCharacter(DWORD &UserNo);

////////////////////////////////////////////////////////////////
// ������ ������ ó�� �Լ�
////////////////////////////////////////////////////////////////
void Update();
////////////////////////////////////////////////////////////////
// ####  Update ���� �Լ�
////////////////////////////////////////////////////////////////
void UpdateAction(st_PLAYER *pPlayer);

void UpdateMove(st_PLAYER *pPlayer);

int DeadReckoning(DWORD dwAction, DWORD dwActionTick, int OldPosX,
	int OldPosY, int *pOutX, int *pOutY);


void ReadySendUpdate(st_PLAYER *pPlayer);


void PlayerColisionCheck(st_PLAYER *pAttacker, BYTE AttackMethod, list<st_PLAYER *> *pOutDemageList);
