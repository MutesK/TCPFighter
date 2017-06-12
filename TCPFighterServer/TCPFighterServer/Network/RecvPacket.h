#pragma once

#include "NetworkProc.h"


void RecvPacket_MoveStart(st_CLIENT *pClient, CRingBuffer *Buffer);
void RecvPacket_MoveStop(st_CLIENT *pClient, CRingBuffer *Buffer);
void RecvPacket_Attack1(st_CLIENT *pClient, CRingBuffer *Buffer);
void RecvPacket_Attack2(st_CLIENT *pClient, CRingBuffer *Buffer);
void RecvPacket_Attack3(st_CLIENT *pClient, CRingBuffer *Buffer);

