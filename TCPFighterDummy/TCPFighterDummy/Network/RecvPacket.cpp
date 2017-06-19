
#include "RecvPacket.h"
#include "../Content/ContentProc.h"
#include "MakePacket.h"
// �帧�� ���� ��Ŷ�� �������� �Ѱܼ� Update �Լ��� ó���ϰԲ� �ϸ� �Ǵµ�
// �׼��� st_Player�� ����. -> ���� �Լ��� �ѱ�
// ����ȭ ���۸� ����Ͽ��� ���������, ȯ�������� �׽�Ʈ�뵵�� ŭ.

extern UINT g_EchoCount;
extern DWORD g_dwClientTick;
extern DWORD g_dwAvrClintTick;
extern DWORD g_dwMaxClientTick;

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
	return;
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

	// RTT ����.
	ClientTick = (timeGetTime() - ClientTick);

	g_EchoCount++;
	g_dwClientTick += ClientTick;

	if (g_dwMaxClientTick < ClientTick)
		g_dwMaxClientTick = ClientTick;
}