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
	// 특정 메모리 위치에 스프라이트를 출력한다. (칼라키, 클리핑 처리)
	//
	///////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// DrawSprite_translucence.
	// 특정 메모리 위치에 이미지를 출력한다. (클리핑 처리, 반투명)
	//
	///////////////////////////////////////////////////////
	//void DrawSprite_translucence(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	//	int iDestHeight, int iDestPitch, int iDrawLen = 100);

	///////////////////////////////////////////////////////
	// DrawSprite_Red.
	// 특정 메모리 위치에 이미지를 출력한다. (클리핑 처리, 약간 붉게)
	//
	///////////////////////////////////////////////////////
	//void DrawSprite_Red(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	//	int iDestHeight, int iDestPitch, int iDrawLen = 100);



protected:
	/*
		ViewPointX , ViewPointY 가 뷰의 시작좌표
	*/
	int ViewPointX;
	int ViewPointY;

};

