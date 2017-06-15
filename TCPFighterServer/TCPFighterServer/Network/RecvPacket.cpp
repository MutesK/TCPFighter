
#include "RecvPacket.h"
#include "../Content/ContentProc.h"
#include "MakePacket.h"
#include "SendPacket.h"
// �帧�� ���� ��Ŷ�� �������� �Ѱܼ� Update �Լ��� ó���ϰԲ� �ϸ� �Ǵµ�
// �׼��� st_Player�� ����. -> ���� �Լ��� �ѱ�
// ����ȭ ���۸� ����Ͽ��� ���������, ȯ�������� �׽�Ʈ�뵵�� ŭ.
void RecvPacket_MoveStart(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_MOVE_START *pPacket = new stPACKET_CS_MOVE_START;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_MOVE_START));

	CharacterMoveStart(pClient, pPacket);
	delete pPacket;
}

void RecvPacket_MoveStop(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_MOVE_STOP *pPacket = new stPACKET_CS_MOVE_STOP;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_MOVE_STOP));

	CharacterMoveStop(pClient, pPacket);
	delete pPacket;
}

void RecvPacket_Attack1(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_ATTACK1 *pPacket = new stPACKET_CS_ATTACK1;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_ATTACK1));

	CharacterAttack1(pClient, pPacket);
	delete pPacket;
}

void RecvPacket_Attack2(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_ATTACK2 *pPacket = new stPACKET_CS_ATTACK2;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_ATTACK2));

	CharacterAttack2(pClient, pPacket);
	delete pPacket;
}

void RecvPacket_Attack3(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_ATTACK3 *pPacket = new stPACKET_CS_ATTACK3;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_ATTACK3));

	CharacterAttack3(pClient, pPacket);
	delete pPacket;
}

void RecvPacket_Echo(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	DWORD ClientTick;
	Buffer->Get(reinterpret_cast<char *>(&ClientTick), 4);

	// Echo�̴� ���� �������� ó�������ʰ� ���⼭ ó���Ѵ�.
	CSerializeBuffer Buf;
	MakeEcho(&Buf, ClientTick);

	pClient->SendQ.PutData(Buf.GetReadBufferPtr(), Buf.GetDataSize());
}