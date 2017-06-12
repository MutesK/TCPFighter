#pragma once

/*
	DIB�� ����� GDI�� ��ũ�� ����
	�������� HDC �� DIB ����Ͽ� �׸��� ����ش�.
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

	// ���� �̹����� ������ ����
	BYTE* GetDibBuffer(void);	
	int GetWidth(void);
	int GetHeight(void);
	int GetPitch(void);

protected:
	BITMAPINFO m_stDibinfo;
	BYTE* m_bypBuffer;			// �̹��� ũ��
	
	int m_iWidth;
	int m_iHeight;
	int m_iPitch;
	int m_iColorBit;
	int m_iBufferSize;
};

