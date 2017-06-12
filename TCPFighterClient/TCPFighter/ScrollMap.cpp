#include "stdafx.h"
#include "ScrollMap.h"
#include "GameDefine.h"
#include "Sprite.h"
 

 WCHAR g_szLogBuffer[1000];
extern CSprite g_cSprite;

CScrollMap::CScrollMap()
{
}


CScrollMap::~CScrollMap()
{
}


// ViewPoint�� X,Y ����
void CScrollMap::DrawSet(int iX, int iY)
{
	// iX�� �ּ����� WIDTH�� �߰��̴�.

	// iX�� �ִ����� �ִ� X ������ - enVIEW_WIDTH / 2
	if (iX > dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2)
		ViewPointX = dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2;
	else if (iX < enVIEW_WIDTH / 2)
		ViewPointX = 0;
	else
		ViewPointX = max(abs(iX - enVIEW_WIDTH / 2), 0);


	// iY�� �ִ����� �ִ� Y ������ - enVIEW_WIDTH / 2
	 if (iY > dfRANGE_MOVE_BOTTOM - enVIEW_HEIGHT / 2)
		ViewPointY = dfRANGE_MOVE_BOTTOM - enVIEW_HEIGHT / 2;
	 else if (iY < enVIEW_HEIGHT / 2)
		 ViewPointY = 0;
	 else 
		 ViewPointY = max(abs(iY - enVIEW_HEIGHT / 2), 0);
}

void CScrollMap::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen )
{
	// ViewPointX, Y�� ���� ���ְ� ������ �׸����ʴ´�. ����̸� �ִ밪 üũ�Ͽ� �Ѿ�� �׸����ʴ´�.
	int startX = abs(iDrawX - ViewPointX);
	int startY = abs(iDrawY - ViewPointY);

	if (ViewPointX == dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2)
		startX += enVIEW_WIDTH / 2;
	if (ViewPointY == dfRANGE_MOVE_BOTTOM - enVIEW_HEIGHT / 2)
		startY+= enVIEW_HEIGHT / 2;


	if (startX < 0 || startX > enVIEW_WIDTH)
		return;

	if (startY < 0 || startY > enVIEW_HEIGHT)
		return;

	g_cSprite.DrawSprite(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch, iDrawLen);
}
/*
void CScrollMap::DrawSprite_translucence(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen )
{
	int startX = abs(iDrawX - ViewPointX);
	int startY = abs(iDrawY - ViewPointY);

	if (ViewPointX == dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2)
		startX += enVIEW_WIDTH / 2;
	if (ViewPointY == dfRANGE_MOVE_BOTTOM - enVIEW_HEIGHT / 2)
		startY += enVIEW_HEIGHT / 2;

	if (startX < 0 || startX > enVIEW_WIDTH)
		return;

	if (startY < 0 || startY > enVIEW_HEIGHT)
		return;
	
	g_cSprite.DrawSprite_translucence(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch, iDrawLen);
}

void CScrollMap::DrawSprite_Red(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen )
{

	int startX = abs(iDrawX - ViewPointX);
	int startY = abs(iDrawY - ViewPointY);

	if (ViewPointX == dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2)
		startX += enVIEW_WIDTH / 2;
	if (ViewPointY == dfRANGE_MOVE_BOTTOM - enVIEW_HEIGHT / 2)
		startY += enVIEW_HEIGHT / 2;

	if (startX < 0 || startX > enVIEW_WIDTH)
		return;

	if (startY < 0 || startY > enVIEW_HEIGHT)
		return;
	_LOG(L"Player Xpos : %d, Ypos : %d , RealXpos = %d, RealYpos = %d ViewPointX = %d, ViewPointY = %d \n", startX, startY, iDrawX, iDrawY, ViewPointX, ViewPointY);

	g_cSprite.DrawSprite_Red(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch, iDrawLen);
}
*/
void CScrollMap::PlayerDraw(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, bool isPlayer, int HpBar)
{


	int startX = (iDrawX - ViewPointX);
	int startY = (iDrawY - ViewPointY);

	if (ViewPointX == dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2)
		startX += enVIEW_WIDTH / 2;
	if (ViewPointY == dfRANGE_MOVE_BOTTOM - enVIEW_HEIGHT / 2)
		startY += enVIEW_HEIGHT / 2;

	if (startX < 0 || startX > enVIEW_WIDTH)
		return;

	if (startY < 0 || startY > enVIEW_HEIGHT)
		return;

	switch (isPlayer)
	{
	case 0:
		g_cSprite.DrawSprite(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch);
		break;
	case 1:
		g_cSprite.DrawSprite_Red(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch);
		break;
	}
	g_cSprite.DrawSprite_translucence(sShadow, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch);
	g_cSprite.DrawSprite(sHPGuage, startX - 35, startY + 9, bypDest, iDestWidth, iDestHeight, iDestPitch, HpBar);

}
void CScrollMap::DrawTileMap(BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch)
{
	const int SpriteWidth = 64; // Same Height;
	const int &SpriteHeight = SpriteWidth;

	int startX = -(ViewPointX % 64);
	int startY = -(ViewPointY % 64);

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			g_cSprite.DrawSprite(sTile, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch);
			startX += 64;
		}

		startX = -(ViewPointX % 64);
		startY += 64;
	}

}