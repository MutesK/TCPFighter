#pragma once

#include "NetworkProc.h"



struct st_SECTOR_AROUND;
struct st_SECTOR_POS;
struct st_PLAYER;

// pAroundSector 
void SendSectorAround(st_SECTOR_AROUND *pAroundSector, st_PLAYER *pExcept ,CSerializeBuffer* Buffer);

void SendSector(st_SECTOR_POS *pSector, st_PLAYER *pExcept,  CSerializeBuffer* Buffer);

void SendUniCast(st_PLAYER *pPlayer, CSerializeBuffer* Buffer);

// Except�� ������ �������� ����  -> �ý��� �޼���
void SendBroadCast(st_PLAYER *pPlayer, CSerializeBuffer* Buffer);