#pragma once

#include "stdafx.h"
#include "PacketDefine.h"


bool SendPacket();
bool SendEvent();

void SendStop(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);
void SendMove(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);
void SendAttack1(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);
void SendAttack2(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);
void SendAttack3(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);



void makePacket_MoveStart(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);

void makePacket_MoveStop(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);

void makePacket_ATTACK1(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);

void makePacket_ATTACK2(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);

void makePacket_ATTACK3(BYTE _iDir, unsigned short Xpos, unsigned short Ypos);
