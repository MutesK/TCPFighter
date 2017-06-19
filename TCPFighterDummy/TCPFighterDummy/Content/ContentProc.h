#pragma once

#include "../Network/NetworkProc.h"


////////////////////////////////
// �÷��̾� ����ü
////////////////////////////////
struct st_PLAYER
{
	st_CLIENT *pClient;

	DWORD dwClientNo;
	DWORD dwPlayerNo;	// ���� Ŭ���̾�Ʈ ��ȣ

	DWORD dwAction;			// ���� �׼�
	DWORD dwActionTick;				// �׼� ���� �ð�
	BYTE byDirection;		// �����ִ� ����

	int woX;		// ���̴� ��ǥ (�׼� ���۽� ���巹Ŀ�� ���� ����)
	int woY;
};
////////////////////////////////////////////////////////////////
// �Ϲ� ������ ó��
////////////////////////////////////////////////////////////////
st_PLAYER* FindPlayer(DWORD &ClientNo);

////////////////////////////////////////////////////////////////
// �׼� ���� �Լ� 
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// �÷��̾� ����
// ������ ���� ��ġ�� ���� �� �ش缽�͸� ������ 
// �ֺ� ���Ϳ� ��Ŷ���������� ��.
////////////////////////////////////////////////////////////////
void CharacterMoveStart(st_PLAYER *);
void CharacterMoveStop(st_PLAYER *);
void CharacterAttack1(st_PLAYER *);
void CharacterAttack2(st_PLAYER *);
void CharacterAttack3(st_PLAYER *);

void CharcterSync(DWORD &iD, WORD &X, WORD &Y);
void Echo(st_PLAYER *);
////////////////////////////////////////////////////////////////
// Accept �ϸ� �ڵ����� ȣ��Ǵ� �Լ�
// �� �ش� Ŭ���̾�Ʈ���Դ� stPACKET_SC_CREATE_MY_CHARACTER�� �����־�� �ǰ� (UniCast)
// �ٸ� Ŭ���̾�Ʈ�鿡�Դ� stPACKET_SC_CREATE_OTHER_CHARACTER�� �����־�� ��.(BroadCast)
////////////////////////////////////////////////////////////////
void CreateCharacter(st_CLIENT *pClient, DWORD &id, BYTE &Direction, WORD &X, WORD &Y, BYTE &HP);

////////////////////////////////////////////////////////////////
// HeartBeat �Ǵ� ü���� ���� �� �Ҹ��Ͽ����� �Լ� ȣ��
// ��ü Ŭ���̾�Ʈ�鿡�� ����(BroadCast)
////////////////////////////////////////////////////////////////
void DeleteCharacter(DWORD &UserNo, DWORD &PlayerNo);

void Update();

int DeadReckoning(DWORD dwAction, DWORD dwActionTick, int OldPosX,
	int OldPosY, int *pOutX, int *pOutY);

