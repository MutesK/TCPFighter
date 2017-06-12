#pragma once
class CSprite
{
public:

	// ****************************************************************
	// DIB ��������Ʈ ����ü.
	//
	// ��������Ʈ �̹����� ������ ������ ����.
	// ****************************************************************

	typedef struct st_SPRITE
	{
		BYTE *bypImage;                            // ��������Ʈ �̹��� ������.
		int    iWidth;                            // Widht
		int    iHeight;                           // Height
		int    iPitch;                            // Pitch

		int    iCenterPointX;                     // ���� X
		int    iCenterPointY;                     // ���� Y
	} st_SPRITE;

	////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Parameters: (int)�ִ� ��������Ʈ ����. (DWORD)����Į��.
	////////////////////////////////////////////////////////////////////
	CSprite(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSprite();

	///////////////////////////////////////////////////////
	// LoadDibSprite.
	// BMP������ �о �ϳ��� ���������� �����Ѵ�.
	//
	///////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, WCHAR *szFileName, int iCenterPointX, int iCenterPointY);
	///////////////////////////////////////////////////////
	// ReleaseSprite.
	// �ش� ��������Ʈ ����.
	//
	///////////////////////////////////////////////////////
	void ReleaseSprite(int iSpriteIndex);

	///////////////////////////////////////////////////////
	// DrawSprite.
	// Ư�� �޸� ��ġ�� ��������Ʈ�� ����Ѵ�. (Į��Ű, Ŭ���� ó��)
	//
	///////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	///////////////////////////////////////////////////////
	// DrawImage.
	// Ư�� �޸� ��ġ�� �̹����� ����Ѵ�. (Ŭ���� ó��)
	//
	///////////////////////////////////////////////////////
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);
	///////////////////////////////////////////////////////
	// DrawSprite_translucence.
	// Ư�� �޸� ��ġ�� �̹����� ����Ѵ�. (Ŭ���� ó��, ������)
	//
	///////////////////////////////////////////////////////
	void DrawSprite_translucence(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);

	///////////////////////////////////////////////////////
	// DrawSprite_Red.
	// Ư�� �޸� ��ġ�� �̹����� ����Ѵ�. (Ŭ���� ó��, �ణ �Ӱ�)
	//
	///////////////////////////////////////////////////////
	void DrawSprite_Red(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDrawLen = 100);



protected:

	//------------------------------------------------------------------
	// Sprite �迭 ����.
	//------------------------------------------------------------------
	int       m_iMaxSprite;
	st_SPRITE *m_stpSprite;

	//------------------------------------------------------------------
	// ���� �������� ����� �÷�.
	//------------------------------------------------------------------
	DWORD     m_dwColorKey;

};

