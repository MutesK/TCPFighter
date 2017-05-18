#include "stdafx.h"
#include "PacketRecv.h"
#include "RingBuffer.h"
#include "GameDefine.h"
#include "Player.h"
#include "Sprite.h"
#include "Effect.h"
#include <list>

CRingBuffer RecvQ(1000);

extern SOCKET g_ClientSocket;
extern CSprite g_cSprite;
extern CPlayer* g_pPlayer;
extern list<CBaseObject *> List;

bool RecvEvent()
{

	WSABUF wsabuf[2];
	int bufcount = 1;

	wsabuf[0].len = RecvQ.GetNotBrokenPutSize();
	wsabuf[0].buf = RecvQ.GetWriteBufferPtr();

	if (wsabuf[0].len < RecvQ.GetFreeSize())
	{
		// 공간 체크
		wsabuf[1].len = RecvQ.GetFreeSize() - wsabuf[0].len;
		wsabuf[1].buf = RecvQ.GetBufferPtr();
		bufcount++;
	}

	DWORD RecvSize = 0;
	DWORD Flag = 0;

	int err = WSARecv(g_ClientSocket, wsabuf, bufcount, &RecvSize, &Flag, NULL, NULL);

	if (err == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			//err_Print(L"WSARecv");
			return false;
		}
	}
	RecvQ.MoveWritePos(RecvSize);
	RecvPacket_Check();


	return true;
}
bool RecvPacket_Check()
{
	int size = 4;
	st_NETWORK_PACKET_HEADER HeaderPacket;

	char tBuffer[100];

	while (1)
	{
		// 1 RecvQ 에 최소한 사이즈가 있는지 확인.
		if (RecvQ.GetUseSize() <= size)
			return false;

		// 2. RecvQ에서 헤더를 Peek
		if (RecvQ.Peek((char *)&HeaderPacket, size) != size)
			return false;


		//printf("HeaderPacket Code = %d Type : %d  \n", HeaderPacket.byCode == dfNETWORK_PACKET_CODE,
			//HeaderPacket.byType);

		// 3. 헤더의 Code 확인
		if (HeaderPacket.byCode != dfNETWORK_PACKET_CODE)
			return false;

		// 4. 헤더의 Len값과 RecvQ의 데이터 사이즈 비교 - 완성패킷 사이즈 Header크기 + Len + 엔드코드(1)
		if (RecvQ.GetUseSize() < size + HeaderPacket.bySize + 1)
			return false;

		// 5. 데이터 Peek 했던 헤더를 RecvQ에서 지운다.
		RecvQ.RemoveData(size);


		// 6. RecvQ에서 Len 만큼 임시 패킷버퍼로 뽑는다.
		int bufSize = RecvQ.Get(tBuffer, HeaderPacket.bySize);
		if (bufSize != HeaderPacket.bySize)
			return false; // 잘못된 패킷


		// 7. RecvQ에서 엔드코드 뽑는다.
		char endCode;
		if (RecvQ.Get(&endCode, 1) != 1)
			return false;

		// 8. 앤드코드 확인
		if (endCode != dfNETWORK_PACKET_END)
			return false;

		PacketProc(HeaderPacket.byType, tBuffer);
	}

	return true;
}

bool PacketProc(BYTE byPacketType, char *Packet)
{
	switch (byPacketType)
	{
	default:
		return false;
		//---------------------------------
		// Server -> Client Packet <Recv Part>
		//---------------------------------
	case dfPACKET_SC_CREATE_MY_CHARACTER:
		netPacket_CreateMyCharacter(Packet);
		break;
	case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		netPacket_CreateOtherCharacter(Packet);
		break;
	case dfPACKET_SC_DELETE_CHARACTER:
		netPacket_DeleteCharacter(Packet);
		break;
	case dfPACKET_SC_MOVE_START:
		netPacket_SC_MoveStart(Packet);
		break;
	case dfPACKET_SC_MOVE_STOP:
		netPacket_SC_MoveStop(Packet);
		break;
	case dfPACKET_SC_ATTACK1:
		netPacket_SC_ATTACK1(Packet);
		break;
	case dfPACKET_SC_ATTACK2:
		netPacket_SC_ATTACK2(Packet);
		break;
	case dfPACKET_SC_ATTACK3:
		netPacket_SC_ATTACK3(Packet);
		break;
	case dfPACKET_SC_DAMAGE:
		netPacket_SC_DAMAGE(Packet);
		break;
	case dfPACKET_SC_SYNC:
		netPacket_SC_SYNC(Packet);
		break;
	}
	return true;
}
void netPacket_CreateMyCharacter(char *Packet)
{
	stPACKET_CREATE_SC_CHARACTER T = { 0 };
	memcpy(&T, Packet, 10);

	switch (T.Direction)
	{
	case dfPACKET_MOVE_DIR_LL:
		g_pPlayer = new CPlayer(T.X, T.Y, sPlayer_Stand_Left1, sPlayer_Stand_Left_MAX, dfDELAY_STAND, T.ID, CBaseObject::Player, &g_cSprite, true);
		break;
	case dfPACKET_MOVE_DIR_RR:
		g_pPlayer = new CPlayer(T.X, T.Y, sPlayer_Stand_Right1, sPlayer_Stand_Right_MAX, dfDELAY_STAND, T.ID, CBaseObject::Player, &g_cSprite, true);
		break;
	}


	g_pPlayer->ActionInput(dfACTION_STAND);
	List.push_back(g_pPlayer);
}
void netPacket_CreateOtherCharacter(char *Packet)
{
	stPACKET_CREATE_SC_CHARACTER T = { 0 };
	memcpy(&T, Packet, 10);
	CPlayer* player = NULL;

	switch (T.Direction)
	{
	case dfPACKET_MOVE_DIR_LL:
		player = new CPlayer(T.X, T.Y, sPlayer_Stand_Left1, sPlayer_Stand_Left_MAX, dfDELAY_STAND, T.ID, CBaseObject::Player, &g_cSprite, false);
		break;
	case dfPACKET_MOVE_DIR_RR:
		player = new CPlayer(T.X, T.Y, sPlayer_Stand_Right1, sPlayer_Stand_Right_MAX, dfDELAY_STAND, T.ID, CBaseObject::Player, &g_cSprite, false);
		((CPlayer *)player)->SetDirection(dfPACKET_MOVE_DIR_RR);
		break;
	}
	player->SetHp(T.HP);
	player->ActionInput(dfACTION_STAND);
	List.push_back(player);

}
void netPacket_DeleteCharacter(char *Packet)
{
	// 만약 본인이라면?
	DWORD iD = (DWORD)*Packet;
	if (g_pPlayer->GetObjectID() == iD)
	{
		// 게임 오버
		return;
	}
	list<CBaseObject *>::iterator iter;
	// 아니라면 List에서 전부 조회한다.
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if ((*iter)->GetObjectID() == iD)
		{
			// 해당 클라이언트는 제거한다.
			List.erase(iter);
			return;
		}
	}
}
void netPacket_SC_MoveStart(char *Packet)
{
	list <CBaseObject *>::iterator iter;

	stPacket_SC_MOVESTART T = { 0 };
	memcpy(&T, Packet, 9);

	// 다른 유저이니 ID 검색한다.
	// 아니라면 List에서 전부 조회한다.
	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if ((*iter)->GetObjectID() == T.ID)
		{
			CPlayer *player = (CPlayer *)(*iter);

			
			player->ActionInput(T.Direction);
			player->SetPosition(T.X, T.Y);
		}
	}
}
void netPacket_SC_MoveStop(char* Packet)
{
	list <CBaseObject *>::iterator iter;

	stPacket_SC_MOVESTOP T = { 0 };
	memcpy(&T, Packet, 9);

	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if ((*iter)->GetObjectID() == T.ID)
		{
			CBaseObject *player = (*iter);

			player->SetPositionX(T.X);
			player->SetPositionY(T.Y);

			player->ActionInput(dfACTION_STAND);
			return;
		}
	}

}
void netPacket_SC_ATTACK1(char *Packet)
{
	stPacket_SC_ATTACK T = { 0 };
	memcpy(&T, Packet, 9);
	list <CBaseObject *>::iterator iter;

	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if ((*iter)->GetObjectID() == T.ID)
		{
			CBaseObject *player = (*iter);

			player->SetPositionX(T.X);
			player->SetPositionY(T.Y);

			player->ActionInput(dfACTION_ATTACK1);
			return;
		}
	}

}
void netPacket_SC_ATTACK2(char *Packet)
{
	stPacket_SC_ATTACK T = { 0 };
	memcpy(&T, Packet, 9);
	list <CBaseObject *>::iterator iter;

	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if ((*iter)->GetObjectID() == T.ID)
		{
			CBaseObject *player = (*iter);

			player->SetPositionX(T.X);
			player->SetPositionY(T.Y);

			player->ActionInput(dfACTION_ATTACK2);
			return;
		}
	}

}
void netPacket_SC_ATTACK3(char *Packet)
{
	stPacket_SC_ATTACK T = { 0 };
	memcpy(&T, Packet, 9);
	list <CBaseObject *>::iterator iter;

	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if ((*iter)->GetObjectID() == T.ID)
		{
			CBaseObject *player = (*iter);

			player->SetPositionX(T.X);
			player->SetPositionY(T.Y);

			player->ActionInput(dfACTION_ATTACK3);
			return;
		}
	}

}
void netPacket_SC_DAMAGE(char *Packet)
{
	stPacket_SC_DAMAGE T = { 0 };
	memcpy(&T, Packet, 9);
	list <CBaseObject *>::iterator iter;
	// 공격자나 피해자는 내 플레이어일수 있다.
	// 공격자 Player 객체에 Cffect 트리거를 넣어야 됨.

	//	1.데미지의 HP를 감소
	//	2.어택ID에서 공격 이펙트 


	// 1. 데미지 ID 찾기
	CBaseObject *damagePlayer = NULL;

	if (g_pPlayer->GetObjectID() == T.DamageID)
		damagePlayer = g_pPlayer;
	else
	{
		for (iter = List.begin(); iter != List.end(); iter++)
		{
			if ((*iter)->GetObjectID() == T.DamageID)
			{
				damagePlayer = (*iter);
				break;
			}
		}
	}

	if (damagePlayer == NULL)
	{
		printf("DamagePlayer 못찾음 \n");
		return;
	}

	// 2. 데미지에 의한 감소
	((CPlayer *)damagePlayer)->SetHp(T.DamageHP);

	CBaseObject *attactPlayer = NULL;

	if (g_pPlayer->GetObjectID() == (T.AttackID))
		attactPlayer = g_pPlayer;
	else
	{
		for (iter = List.begin(); iter != List.end(); iter++)
		{
			if ((*iter)->GetObjectID() == (T.AttackID))
			{
				attactPlayer = (*iter);
				break;
			}
		}
	}

	if (attactPlayer == NULL)
	{
		printf("AttackPlayer 못찾음 \n");
		return;
	}
	CEffect* Peffect = new CEffect(damagePlayer->GetCurX(), damagePlayer->GetCurY() - 50, sxSpark_1, sxSpark_MAX, sxSpark_1,
		dfDELAY_EFFECT, -1, CBaseObject::Effect, &g_cSprite, attactPlayer->GetObjectID());

	List.push_back(Peffect);

}
// 서버로부터 동기화 패킷을 받으면 해당 캐릭터를 찾아서
// 캐릭터 좌표를 보정해준다.
void netPacket_SC_SYNC(char *Packet)
{
	stPACKET_SC_SYNC T;
	memcpy(&T, Packet, sizeof(stPACKET_SC_SYNC));

	list <CBaseObject *>::iterator iter;

	for (iter = List.begin(); iter != List.end(); iter++)
	{
		if ((*iter)->GetObjectID() == T.ID)
		{
			(*iter)->SetPositionX(T.X);
			(*iter)->SetPositionY(T.Y);
		}
	}
}
