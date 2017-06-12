#pragma once

/*
	DIB를 사용한 GDI용 스크린 버퍼
	윈도우의 HDC 에 DIB 사용하여 그림을 찍어준다.
*/
class CScreenDIB
{
public:
	CScreenDIB(int iWidth, int iHeight, int iColorBit);
	virtual ~CScreenDIB();

protected:
	void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);
	void ReleaseDibBuffer(void);

public:
	void DrawBuffer(HWND hWnd, int iX = 0, int iY = 0);

	// 실제 이미지의 시작점 리턴
	BYTE* GetDibBuffer(void);	
	int GetWidth(void);
	int GetHeight(void);
	int GetPitch(void);

protected:
	BITMAPINFO m_stDibinfo;
	BYTE* m_bypBuffer;			// 이미지 크기
	
	int m_iWidth;
	int m_iHeight;
	int m_iPitch;
	int m_iColorBit;
	int m_iBufferSize;
};

