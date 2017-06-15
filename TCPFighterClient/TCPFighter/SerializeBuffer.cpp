
#include "SerializeBuffer.h"



CSerializeBuffer::CSerializeBuffer()
{
	m_chpBuffer = new char[eBUFFER_DEFAULT];
	ZeroMemory(m_chpBuffer, eBUFFER_DEFAULT);
	m_iDataSize = 0;
	m_iBufferSize = eBUFFER_DEFAULT - 1;

	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;
}

CSerializeBuffer::CSerializeBuffer(int iBufferSize)
{
	m_chpBuffer = new char[iBufferSize];
	ZeroMemory(m_chpBuffer, iBufferSize);
	m_iDataSize = 0;
	m_iBufferSize = iBufferSize - 1;


	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;
}

CSerializeBuffer::~CSerializeBuffer()
{
	Release();
}


void CSerializeBuffer::Release(void)
{
	Clear();
	m_iBufferSize = 0;

	if (m_chpBuffer != nullptr)
	{
		delete[] m_chpBuffer;
		m_chpBuffer = nullptr;
	}
}
void	CSerializeBuffer::Clear(void)
{
	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;

	m_iDataSize = 0;
}

int	CSerializeBuffer::MoveWritePos(int iSize)
{
	if (iSize < 0)
		return 0;

	int Usage = m_iBufferSize - (m_chpWritePos - m_chpReadPos);
	int CopiedSize = max(min(iSize, Usage), 0);
	m_chpWritePos += CopiedSize;
	m_iDataSize += CopiedSize;

	return CopiedSize;
}

int	CSerializeBuffer::MoveReadPos(int iSize)
{
	if (iSize < 0)
		return 0;

	if (iSize > m_iDataSize)
		return 0;

	m_chpReadPos += iSize;
	m_iDataSize -= iSize;

	return iSize;
}

int	CSerializeBuffer::PeekData(char *chrSrc, int iSrcSize)
{
	int Usage = m_chpWritePos - m_chpReadPos;
	if (Usage == 0)
		return 0;

	int GetDataSize = max(min(iSrcSize, Usage), 0 );

	memcpy_s(chrSrc, GetDataSize, m_chpReadPos, GetDataSize);

	return GetDataSize;
}
int	CSerializeBuffer::GetData(char *chpDest, int iSize)
{
	if (iSize > m_iDataSize)
		return 0;

	memcpy(chpDest, m_chpReadPos, iSize);
	m_chpReadPos += iSize;
	m_iDataSize -= iSize;

	return iSize;
}

int	CSerializeBuffer::PutData(char *chpSrc, int iSrcSize)
{
	if (m_chpWritePos + iSrcSize > m_chpBuffer + m_iBufferSize)
		return 0;

	memcpy(m_chpWritePos, chpSrc , iSrcSize);
	m_chpWritePos += iSrcSize;
	m_iDataSize += iSrcSize;
	
	return iSrcSize;
}

CSerializeBuffer& CSerializeBuffer::operator=(CSerializeBuffer &clSrCSerializeBuffer)
{
	this->m_iDataSize = clSrCSerializeBuffer.m_iDataSize;
	this->m_iBufferSize = clSrCSerializeBuffer.m_iBufferSize;

	this->m_chpBuffer = new char[m_iBufferSize];
	
	int ReadIndex = clSrCSerializeBuffer.m_chpReadPos - clSrCSerializeBuffer.m_chpBuffer;
	this->m_chpReadPos = m_chpBuffer + ReadIndex;

	int WriteIndex = clSrCSerializeBuffer.m_chpWritePos - clSrCSerializeBuffer.m_chpBuffer;
	this->m_chpWritePos = m_chpBuffer + WriteIndex;

	return *this;
}

CSerializeBuffer& CSerializeBuffer::operator<<(BYTE byValue)
{
	PutData((char *)&byValue, 1);
	return *this;
}

CSerializeBuffer& CSerializeBuffer::operator<<(char chValue)
{
	PutData((char *)&chValue, 1);
	return *this;
}

CSerializeBuffer	&CSerializeBuffer::operator << (short shValue)
{
	PutData((char *)&shValue, 2);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator << (WORD wValue)
{
	PutData((char *)&wValue, 2);
	return *this;
}

CSerializeBuffer	&CSerializeBuffer::operator << (int iValue)
{
	PutData((char *)&iValue, 4);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator << (DWORD dwValue)
{
	PutData((char *)&dwValue, 4);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator << (float fValue)
{
	floatdata f;
	f.a= fValue;

	PutData(f.value, 4);
	return *this;
}

CSerializeBuffer	&CSerializeBuffer::operator << (__int64 iValue)
{
	PutData((char *)&iValue, 8);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator << (double dValue)
{
	doubledata d;
	d.a = dValue;

	PutData(d.value, 8);
	return *this;
}

CSerializeBuffer	&CSerializeBuffer::operator >> (BYTE &byValue)
{
	GetData((char *)&byValue, 1);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator >> (char &chValue)
{
	GetData((char *)&chValue, 1);
	return *this;
}

CSerializeBuffer	&CSerializeBuffer::operator >> (short &shValue)
{
	GetData((char *)&shValue, 2);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator >> (WORD &wValue)
{
	GetData((char *)&wValue, 2);
	return *this;
}

CSerializeBuffer	&CSerializeBuffer::operator >> (int &iValue)
{
	GetData((char *)&iValue, 4);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator >> (DWORD &dwValue)
{
	GetData((char *)&dwValue, 4);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator >> (float &fValue)
{
	floatdata f;
	GetData(f.value, 4);

	fValue = f.a;
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator >> (UINT64 &iValue)
{
	GetData((char *)&iValue, 8);
	return *this;
}

CSerializeBuffer	&CSerializeBuffer::operator >> (__int64 &iValue)
{
	GetData((char *)&iValue, 8);
	return *this;
}
CSerializeBuffer	&CSerializeBuffer::operator >> (double &dValue)
{
	doubledata d;
	GetData(d.value, 8);

	dValue = d.a;

	return *this;
}