#include "stdafx.h"
#include "Sprite.h"
#include <stdio.h>


CSprite::CSprite(int iMaxSprite, DWORD dwColorKey)
	:m_iMaxSprite(iMaxSprite), m_dwColorKey(dwColorKey)
{
	m_stpSprite = new st_SPRITE[iMaxSprite];
	
}


CSprite::~CSprite()
{
	delete[] m_stpSprite;
}


BOOL CSprite::LoadDibSprite(int iSpriteIndex, WCHAR *szFileName, int iCenterPointX, int iCenterPointY)
{
	BITMAPINFOHEADER* Bmh = new BITMAPINFOHEADER;
	BITMAPFILEHEADER bmfh;
	BYTE *pDib;
	FILE *pFile;
	_wfopen_s(&pFile, szFileName, L"rb");

	// ���� ũ�� �����´�.
	fseek(pFile, 0, SEEK_END);    // ���� �����͸� ������ ������ �̵���Ŵ
	int fileSize = ftell(pFile) - sizeof(BITMAPFILEHEADER);
	pDib = new BYTE[fileSize];
	fseek(pFile, 0, SEEK_SET);


	// BITMAPFILEHEADER ũ�� ��ŭ �о�´�.
	// ��Ʈ�� ���� ��� �б�. �б⿡ �����ϸ� ���� �����͸� �ݰ� ���α׷� ����
	fseek(pFile, 0, SEEK_SET);
	if (fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, pFile) < 1)
	{
		fclose(pFile);
		return 0;
	}

	// _BITMAPINFOHEADER ������ �о�´�.
	if (fread(pDib, fileSize, 1, pFile) < 1)
	{
		fclose(pFile);
		return 0;
	}

	fclose(pFile);

	Bmh = (BITMAPINFOHEADER *)pDib;
	pDib += sizeof(BITMAPINFOHEADER);

	m_stpSprite[iSpriteIndex].iHeight = Bmh->biHeight;
	m_stpSprite[iSpriteIndex].iWidth = Bmh->biWidth;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
	m_stpSprite[iSpriteIndex].iCenterPointY = iCenterPointY;
	m_stpSprite[iSpriteIndex].iPitch = ((Bmh->biBitCount >> 3) * m_stpSprite[iSpriteIndex].iWidth + 3) & ~3;
	m_stpSprite[iSpriteIndex].bypImage = new BYTE[fileSize];

	pDib +=( (m_stpSprite[iSpriteIndex].iPitch * (m_stpSprite[iSpriteIndex].iHeight - 1)));
	BYTE *p_revDib = new BYTE[fileSize];
	BYTE *revDibIndex = p_revDib;



	for (int y = 0; y < Bmh->biHeight; y++)
	{
		memcpy_s(revDibIndex, m_stpSprite[iSpriteIndex].iPitch ,  pDib, m_stpSprite[iSpriteIndex].iPitch);

		revDibIndex += m_stpSprite[iSpriteIndex].iPitch;
		pDib -= m_stpSprite[iSpriteIndex].iPitch;
		
	}

	memcpy_s(m_stpSprite[iSpriteIndex].bypImage, fileSize, p_revDib, fileSize);



	return 1;
}

void CSprite::ReleaseSprite(int iSpriteIndex)
{
	delete m_stpSprite[iSpriteIndex].bypImage;
}
// iDrawLen ü�¿����� ����Ұ�.  Bar�� ���� ���� ���'
void CSprite::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen)
{
	
	// Ŭ���� ó��
	int imageWidth = m_stpSprite[iSpriteIndex].iWidth * iDrawLen / 100;;
	int imageHeight = m_stpSprite[iSpriteIndex].iHeight;
	int imagePitch = m_stpSprite[iSpriteIndex].iPitch;
	int startX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int startY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	BYTE *pImage = m_stpSprite[iSpriteIndex].bypImage;
	
	// ���� Ŭ���� 
	if (iDestWidth < startX + imageWidth)
	{
		imageWidth -= abs(imageWidth + startX - iDestWidth);
	}
	// ���� Ŭ����
	if (startX < 0)
	{
		imageWidth += startX;
		pImage += abs(startX) * 4/*����Ʈ*/;
		startX = 0;
	}

	// ���� Ŭ����
	if (startY < 0)
	{
		imageHeight += startY;
		pImage += abs(startY) * imagePitch;
		startY = 0;
	}

	// ���� Ŭ����
	if (iDestHeight < startY + imageHeight)
	{
		imageHeight -= abs(startY + imageHeight - iDestHeight);
	}
	
	BYTE *ImageLine;
	BYTE *DestLine;
	int x = 0, y = 0;
	for ( y = 0; y < imageHeight; y++)
	{
		DestLine = bypDest + (((startX) * 4) + ((startY + y) * iDestPitch));
		ImageLine = pImage + (y * imagePitch);
		for ( x = 0; x < imageWidth; x++)
		{
			if (0xffffffff != *(DWORD *)ImageLine)
			{
				*(DWORD *)DestLine = *(DWORD *)ImageLine;
			}

			ImageLine += 4;
			DestLine += 4;
		}
		
	}	
}

void CSprite::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen)
{
	// Ŭ���� ó��
	// Ŭ���� ó��
	int imageWidth = m_stpSprite[iSpriteIndex].iWidth * iDrawLen / 100;
	int imageHeight = m_stpSprite[iSpriteIndex].iHeight;
	int imagePitch = m_stpSprite[iSpriteIndex].iPitch;
	int startX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int startY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	BYTE *pImage = m_stpSprite[iSpriteIndex].bypImage;

	// ���� Ŭ���� 
	if (iDestWidth < startX + imageWidth)
	{
		imageWidth -= abs(imageWidth + startX - iDestWidth);
	}
	// ���� Ŭ����
	if (startX < 0)
	{
		imageWidth += startX;
		pImage += abs(startX) * 4/*����Ʈ*/;
		startX = 0;
	}

	// ���� Ŭ����
	if (startY < 0)
	{
		imageHeight += startY;
		pImage += abs(startY) * imagePitch;
		startY = 0;
	}

	// ���� Ŭ����
	if (iDestHeight < startY + imageHeight)
	{
		imageHeight -= abs(startY + imageHeight - iDestHeight);
	}

	BYTE *ImageLine;
	BYTE *DestLine;
	int x = 0, y = 0;

	for (y = 0; y < imageHeight; y++)
	{
		DestLine = bypDest + (((startX) * 4) + ((startY + y) * iDestPitch));
		ImageLine = pImage + (y * imagePitch);

		memcpy_s(DestLine, imageWidth * 4, ImageLine, imageWidth * 4);
	}
}


void CSprite::DrawSprite_translucence(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen)
{
	// Ŭ���� ó��
	// Ŭ���� ó��
	int imageWidth = m_stpSprite[iSpriteIndex].iWidth;
	int imageHeight = m_stpSprite[iSpriteIndex].iHeight;
	int imagePitch = m_stpSprite[iSpriteIndex].iPitch;
	int startX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int startY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	BYTE *pImage = m_stpSprite[iSpriteIndex].bypImage;

	// ���� Ŭ���� 
	if (iDestWidth < startX + imageWidth)
	{
		imageWidth -= abs(imageWidth + startX - iDestWidth);
	}
	// ���� Ŭ����
	if (startX < 0)
	{
		imageWidth += startX;
		pImage += abs(startX) * 4/*����Ʈ*/;
		startX = 0;
	}

	// ���� Ŭ����
	if (startY < 0)
	{
		imageHeight += startY;
		pImage += abs(startY) * imagePitch;
		startY = 0;
	}

	// ���� Ŭ����
	if (iDestHeight < startY + imageHeight)
	{
		imageHeight -= abs(startY + imageHeight - iDestHeight);
	}

	BYTE *ImageLine;
	BYTE *DestLine;
	int x = 0, y = 0;
	for (y = 0; y < imageHeight; y++) 
	{ 
		DestLine = bypDest + (((startX) * 4) + ((startY + y) * iDestPitch));
		ImageLine = pImage + (y * imagePitch);
		for (x = 0; x < imageWidth; x++)
		{
			if (*(DWORD *)ImageLine == 0x00000000)
			{
				*(DestLine + 0) = *(ImageLine + 0) / 2 + *(DestLine + 0) / 2;
				*(DestLine + 1) = *(ImageLine + 1) / 2 + *(DestLine + 1) / 2;
				*(DestLine + 2) = *(ImageLine + 2) / 2 + *(DestLine + 2) / 2;
			}
			ImageLine += 4;
			DestLine += 4;
		}
	}

}

void CSprite::DrawSprite_Red(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDrawLen)
{
	// Ŭ���� ó��
	// Ŭ���� ó��
	int imageWidth = m_stpSprite[iSpriteIndex].iWidth;
	int imageHeight = m_stpSprite[iSpriteIndex].iHeight;
	int imagePitch = m_stpSprite[iSpriteIndex].iPitch;
	int startX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int startY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	BYTE *pImage = m_stpSprite[iSpriteIndex].bypImage;

	// ���� Ŭ���� 
	if (iDestWidth < startX + imageWidth)
	{
		imageWidth -= abs(imageWidth + startX - iDestWidth);
	}
	// ���� Ŭ����
	if (startX < 0)
	{
		imageWidth += startX;
		pImage += abs(startX) * 4/*����Ʈ*/;
		startX = 0;
	}

	// ���� Ŭ����
	if (startY < 0)
	{
		imageHeight += startY;
		pImage += abs(startY) * imagePitch;
		startY = 0;
	}

	// ���� Ŭ����
	if (iDestHeight < startY + imageHeight)
	{
		imageHeight -= abs(startY + imageHeight - iDestHeight);
	}

	BYTE *ImageLine;
	BYTE *DestLine;
	int x = 0, y = 0;
	for (y = 0; y < imageHeight; y++)
	{
		DestLine = bypDest + (((startX) * 4) + ((startY + y) * iDestPitch));
		ImageLine = pImage + (y * imagePitch);
		for (x = 0; x < imageWidth; x++)
		{
			if (0xffffffff != *(DWORD *)ImageLine)
			{
				*(DestLine + 0) = *(ImageLine + 0) / 2;
				*(DestLine + 1) = *(ImageLine + 1) / 2;
				*(DestLine + 2) = *(ImageLine + 2);
			}
			ImageLine += 4;
			DestLine += 4;
		}
	}

}