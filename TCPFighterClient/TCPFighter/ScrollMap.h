#pragma once

extern WCHAR g_szLogBuffer[1000];
#define _LOG(fmt, ...)									\
do {													\
		wsprintf(g_szLogBuffer, fmt, ##__VA_ARGS__);	\
		OutputDebugString(g_szLogBuffer);				\
}while(0)												\

class CSprite;

class CScrollMap
{
public:
	enum
	{
		enVIEW_WIDTH = 640,
		enVIEW_HEIGHT = 480
	};

	CScrollMap();
	~CScrollMap();


	void DrawSet(int iX, int iY);


	void DrawTileMap(BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch);
	
	void PlayerDraw(int iSpriteIndex, int iDrawX, int DrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, bool isPlayer, int HpBar = 100);
	///////////////////////////////////////////////////////
	// DrawSprite.
	// Ư�� �޸� ��ġ�� ��������Ʈ�� ����Ѵ�. (Į��Ű, Ŭ���� ó��)
	//
	///////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// DrawSprite_translucence.
	// Ư�� �޸� ��ġ�� �̹����� ����Ѵ�. (Ŭ���� ó��, ������)
	//
	///////////////////////////////////////////////////////
	//void DrawSprite_translucence(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	//	int iDestHeight, int iDestPitch, int iDrawLen = 100);

	///////////////////////////////////////////////////////
	// DrawSprite_Red.
	// Ư�� �޸� ��ġ�� �̹����� ����Ѵ�. (Ŭ���� ó��, �ణ �Ӱ�)
	//
	///////////////////////////////////////////////////////
	//void DrawSprite_Red(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	//	int iDestHeight, int iDestPitch, int iDrawLen = 100);



protected:
	/*
		ViewPointX , ViewPointY �� ���� ������ǥ
	*/
	int ViewPointX;
	int ViewPointY;

};

