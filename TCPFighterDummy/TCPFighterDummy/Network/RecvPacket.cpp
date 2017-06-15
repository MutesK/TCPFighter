
#include "RecvPacket.h"
#include "../Content/ContentProc.h"
#include "MakePacket.h"
// 흐름상 들어온 패킷을 컨텐츠로 넘겨서 Update 함수로 처리하게끔 하면 되는데
// 액션은 st_Player에 있음. -> 대응 함수로 넘김
// 직렬화 버퍼를 사용하여도 상관없지만, 환형버퍼의 테스트용도가 큼.


void netPacket_CreateMyCharacter(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	DWORD iD;
	BYTE Direction;
	WORD X, Y;
	BYTE HP;

	*Buffer >> iD >> Direction >> X >> Y >> HP;

	CreateCharacter(pClient, iD, Direction, X, Y, HP);
}

void netPacket_CreateOtherCharacter(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	return;
}

void netPacket_DeleteCharacter(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	DWORD dwPlayerNo;

	*Buffer >> dwPlayerNo;
	DeleteCharacter(pClient->dwClientNo, dwPlayerNo);
}

void netPacket_SC_MoveStart(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	return;
}

void netPacket_SC_MoveStop(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	return;
}

void netPacket_SC_ATTACK1(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	return;
}

void netPacket_SC_ATTACK2(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	return;
}

void netPacket_SC_ATTACK3(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	return;
}

void netPacket_SC_DAMAGE(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	return;
}

void netPacket_SC_SYNC(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	DWORD iD;
	WORD X;
	WORD Y;

	*Buffer >> iD;
	*Buffer >> X;
	*Buffer >> Y;

	CharcterSync(iD, X, Y);
}

void netPacket_SC_ECHO(st_CLIENT *pClient, CSerializeBuffer *Buffer)
{
	DWORD ClientTick;

	*Buffer >> ClientTick;

	// RTT 나옴.
	ClientTick = (timeGetTime() - ClientTick) / 1000;

}