#include "stdafx.h"
#include "CScreenDIB.h"


CScreenDIB::CScreenDIB(int iWidth, int iHeight, int iColorBit)
	:m_iWidth(iWidth), m_iHeight(iHeight), m_iColorBit(iColorBit)
{
	CreateDibBuffer(iWidth, iHeight, iColorBit);
}


CScreenDIB::~CScreenDIB()
{
	ReleaseDibBuffer();
}

// DibBuffer Create
void CScreenDIB::CreateDibBuffer(int iWidth, int iHeight, int iColorBit)
{
		// 입력 받은 인자의 정보로 맴버 변수 정보 셋팅.
		// m_stDibinfo를 셋팅한다.
	m_iPitch = ((iColorBit >> 3) * iWidth + 3) & ~3;
	m_bypBuffer = new BYTE[iWidth * iHeight * (iColorBit / 8)];
	memset(m_bypBuffer, 0xff, iWidth * iHeight * (iColorBit / 8));

	BITMAPINFOHEADER *pH = &m_stDibinfo.bmiHeader;
	memset(pH, 0, sizeof(BITMAPINFOHEADER));

	pH->biSize = sizeof(BITMAPINFOHEADER);
	pH->biWidth = iWidth;
	pH->biHeight = -iHeight;
	pH->biPlanes = 1;
	pH->biBitCount = iColorBit;
	
}

void CScreenDIB::ReleaseDibBuffer(void)
{
	delete[] m_bypBuffer;
}

int CScreenDIB::GetWidth(void)
{
	return m_iWidth;
}
int CScreenDIB::GetHeight(void)
{
	return m_iHeight;
}
int CScreenDIB::GetPitch(void)
{
	return m_iPitch;
}

BYTE* CScreenDIB::GetDibBuffer(void)
{
	return m_bypBuffer;
}

void CScreenDIB::DrawBuffer(HWND hWnd, int iX, int iY)
{
	HDC hdc = GetDC(hWnd);

	StretchDIBits(hdc, iX, iY, m_iWidth, m_iHeight,
		0, 0, m_iWidth, m_iHeight
		, m_bypBuffer, &m_stDibinfo, DIB_RGB_COLORS, SRCCOPY);
}