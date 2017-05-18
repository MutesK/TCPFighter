#pragma once

#include "stdafx.h"
#include "PacketDefine.h"
#include "NetworkProc.h"
using namespace std;


bool RecvEvent();
bool RecvPacket_Check();

void netPacket_CreateMyCharacter(char *Packet);
void netPacket_CreateOtherCharacter(char *Packet);
void netPacket_DeleteCharacter(char *Packet);
void netPacket_SC_MoveStart(char *Packet);
void netPacket_SC_MoveStop(char* Packet);
void netPacket_SC_ATTACK1(char *Packet);
void netPacket_SC_ATTACK2(char *Packet);
void netPacket_SC_ATTACK3(char *Packet);
void netPacket_SC_DAMAGE(char *Packet);
void netPacket_SC_SYNC(char *Packet);
bool PacketProc(BYTE byPacketType, char *Packet);