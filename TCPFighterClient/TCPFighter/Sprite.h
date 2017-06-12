#pragma once
class CSprite
{
public:

	// ****************************************************************
	// DIB 스프라이트 구조체.
	//
	// 스프라이트 이미지와 사이즈 정보를 가짐.
	// ****************************************************************

	typedef struct st_SPRITE
	{
		BYTE *bypImage;                            // 스프라이트 이미지 포인터.
		int    iWidth;                            // Widht
		int    iHeight;                           // Height
		int    iPitch;                            // Pitch

		int    iCenterPointX;                     // 중점 X
		int    iCenterPointY;                     // 중점 Y
	} st_SPRITE;

	////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters: (int)최대 스프라이트 개수. (DWORD)투명칼라.
	////////////////////////////////////////////////////////////////////
	CSprite(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSprite();

	///////////////////////////////////////////////////////
	// LoadDibSprite.
	// BMP파일을 읽어서 하나의 프레임으로 저장한다.
	//
	///////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, WCHAR *szFileName, int iCenterPointX, int iCenterPointY);
	///////////////////////////////////////////////////////
	// ReleaseSprite.
	// 해당 스프라이트 해제.
	//
	///////////////////////////////////////////////////////
	void ReleaseSprite(int iSpriteIndex);

	///////////////////////////////////////////////////////
	// DrawSprite.
	// 특정 메모리 위치에 스프라이트를 출력한다. (칼라키, 클리핑 처리)
	//
	///////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	///////////////////////////////////////////////////////
	// DrawImage.
	// 특정 메모리 위치에 이미지를 출력한다. (클리핑 처리)
	//
	///////////////////////////////////////////////////////
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	///////////////////////////////////////////////////////
	// DrawSprite_translucence.
	// 특정 메모리 위치에 이미지를 출력한다. (클리핑 처리, 반투명)
	//
	///////////////////////////////////////////////////////
	void DrawSprite_translucence(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);

	///////////////////////////////////////////////////////
	// DrawSprite_Red.
	// 특정 메모리 위치에 이미지를 출력한다. (클리핑 처리, 약간 붉게)
	//
	///////////////////////////////////////////////////////
	void DrawSprite_Red(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);



protected:

	//------------------------------------------------------------------
	// Sprite 배열 정보.
	//------------------------------------------------------------------
	int       m_iMaxSprite;
	st_SPRITE *m_stpSprite;

	//------------------------------------------------------------------
	// 투명 색상으로 사용할 컬러.
	//------------------------------------------------------------------
	DWORD     m_dwColorKey;

};

