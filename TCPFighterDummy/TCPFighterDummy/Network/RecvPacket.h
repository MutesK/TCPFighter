#pragma once

#include "NetworkProc.h"


void netPacket_CreateMyCharacter(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_CreateOtherCharacter(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_DeleteCharacter(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_MoveStart(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_MoveStop(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_ATTACK1(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_ATTACK2(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_ATTACK3(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_DAMAGE(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_SYNC(st_CLIENT *pClient, CSerializeBuffer *Buffer);
void netPacket_SC_ECHO(st_CLIENT *pClient, CSerializeBuffer *Buffer);