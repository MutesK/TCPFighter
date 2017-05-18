#include "stdafx.h"
#include "ScrollMap.h"
#include "GameDefine.h"
#include "Sprite.h"
 


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
	 else 
		 ViewPointX = max(iX - enVIEW_WIDTH / 2, 0);



	// iY�� �ִ����� �ִ� Y ������ - enVIEW_WIDTH / 2
	 if (iY > dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2)
		ViewPointY = dfRANGE_MOVE_RIGHT - enVIEW_WIDTH / 2;
	 else 
		 ViewPointY = max(iY - enVIEW_WIDTH / 2, 0);
}

void CScrollMap::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen )
{

	// ViewPointX, Y�� ���� ���ְ� ������ �׸����ʴ´�. ����̸� �ִ밪 üũ�Ͽ� �Ѿ�� �׸����ʴ´�.

	int startX = iDrawX - ViewPointX;
	int startY = iDrawY - ViewPointY;


	if (startX < 0 || startX > enVIEW_WIDTH)
		return;

	if (startY < 0 || startY > enVIEW_HEIGHT)
		return;

	

	g_cSprite.DrawSprite(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch, iDrawLen);
	
}

void CScrollMap::DrawSprite_translucence(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen )
{
	int startX = iDrawX - ViewPointX;
	int startY = iDrawY - ViewPointY;

	if (startX < 0 || startX > enVIEW_WIDTH)
		return;

	if (startY < 0 || startY > enVIEW_HEIGHT)
		return;
	
	g_cSprite.DrawSprite_translucence(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch, iDrawLen);
}

void CScrollMap::DrawSprite_Red(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen )
{

	int startX = iDrawX - ViewPointX;
	int startY = iDrawY - ViewPointY;

	if (startX < 0 || startX > enVIEW_WIDTH)
		return;

	if (startY < 0 || startY > enVIEW_HEIGHT)
		return;

	g_cSprite.DrawSprite_Red(iSpriteIndex, startX, startY, bypDest, iDestWidth, iDestHeight, iDestPitch, iDrawLen);
}

void CScrollMap::DrawTileMap(BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch)
{
	const int SpriteWidth = 64; // Same Height;
	const int &SpriteHeight = SpriteWidth;


	int startX = -(ViewPointX % 64) ;
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