
#include "RecvPacket.h"
#include "../Content/ContentProc.h"
// 흐름상 들어온 패킷을 컨텐츠로 넘겨서 Update 함수로 처리하게끔 하면 되는데
// 액션은 st_Player에 있음. -> 대응 함수로 넘김

void RecvPacket_MoveStart(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_MOVE_START *pPacket = new stPACKET_CS_MOVE_START;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_MOVE_START));

	CharacterMoveStart(pClient, pPacket);
}

void RecvPacket_MoveStop(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_MOVE_STOP *pPacket = new stPACKET_CS_MOVE_STOP;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_MOVE_STOP));

	CharacterMoveStop(pClient, pPacket);
}

void RecvPacket_Attack1(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_ATTACK1 *pPacket = new stPACKET_CS_ATTACK1;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_ATTACK1));

	CharacterAttack1(pClient, pPacket);
}

void RecvPacket_Attack2(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_ATTACK2 *pPacket = new stPACKET_CS_ATTACK2;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_ATTACK2));

	CharacterAttack2(pClient, pPacket);
}

void RecvPacket_Attack3(st_CLIENT *pClient, CRingBuffer *Buffer)
{
	stPACKET_CS_ATTACK3 *pPacket = new stPACKET_CS_ATTACK3;
	Buffer->Get(reinterpret_cast<char *>(pPacket), sizeof(stPACKET_CS_ATTACK3));

	CharacterAttack3(pClient, pPacket);
}
