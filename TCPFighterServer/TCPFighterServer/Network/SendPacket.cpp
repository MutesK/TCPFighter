#include "SendPacket.h"

#include "../Content/ContentProc.h"
#include "../Content/Sector.h"

extern list<st_PLAYER *> g_Sector[][dfSECTOR_MAX_X];

void SendSectorAround(st_SECTOR_AROUND *pAroundSector, st_PLAYER *pExcept, CSerializeBuffer* Buffer)
{
	if (pAroundSector == nullptr)
		return;

//endSector(pAroundSector->pData, pExcept, Buffer);
	for (int i = 0; i < pAroundSector->iCount; i++)
	{
		SendSector(&pAroundSector->Around[i], pExcept, Buffer);
	}
}

void SendSector(st_SECTOR_POS *pSector, st_PLAYER *pExcept, CSerializeBuffer* Buffer)
{
	if (pSector == nullptr)
		return;

	if (pSector->iX <= -1 || pSector->iX > dfSECTOR_MAX_X || pSector->iY <= -1 || pSector->iY > dfSECTOR_MAX_Y)
		return;

	//_OUTPUTDEBUG(L"SendSector [%d %d]\n", pSector->iY, pSector->iX);
	list <st_PLAYER *> *list = &g_Sector[pSector->iY][pSector->iX];

	auto begin = list->begin();
	auto end = list->end();
	//_OUTPUTDEBUG(L"SendSector List Size = %d \n", list->size());

	for(auto iter = begin; iter != end; ++iter)
	{
		st_PLAYER *pPlayer = (*iter);

		if (pPlayer != pExcept)
		{
			pPlayer->pClient->SendQ.PutData(Buffer->GetReadBufferPtr(), Buffer->GetDataSize());
		}
	}
}

void SendUniCast(st_PLAYER *pPlayer, CSerializeBuffer* Buffer)
{
	if (pPlayer == nullptr)
		return;

	pPlayer->pClient->SendQ.PutData(Buffer->GetReadBufferPtr(), Buffer->GetDataSize());
}

void SendBroadCast(st_PLAYER *pPlayer, CSerializeBuffer* Buffer)
{
	if (pPlayer == nullptr)
		return;

	pPlayer->pClient->SendQ.PutData(Buffer->GetReadBufferPtr(), Buffer->GetDataSize());
}