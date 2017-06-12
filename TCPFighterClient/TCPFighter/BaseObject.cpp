#include "stdafx.h"
#include "BaseObject.h"
#include "GameDefine.h"


CBaseObject::CBaseObject(int Xpos, int YPos, int SpriteStart, int SpriteEnd, int SpriteNow, int SpriteFrameDelay,
	int ObjectID, int ObjectType, CSprite *sprite)
{
	m_iXpos = Xpos;
	m_iYpos = YPos;


	m_iStart = SpriteStart;
	m_iEnd = SpriteEnd;
	m_iFrameNow = SpriteStart;
	m_iDelayCount = SpriteFrameDelay;

	m_iObjectID = ObjectID;
	m_iObjectType = ObjectType;

	m_iFrameEnd = false;
	m_Sprite = sprite;
}


CBaseObject::~CBaseObject()
{
	m_Sprite = nullptr;
}


void CBaseObject::NextFrame()
{
	if (0 > m_iStart)
		return;

	m_iDelayCount++;

	if (m_iDelayCount >= m_iFrameDelay)
	{
		m_iDelayCount = 0;
		m_iNow++;

		if (m_iNow >= m_iEnd)
		{
			m_iNow = m_iStart;
			m_iFrameEnd = true;
		}
	}
}

int CBaseObject::GetCurX()
{
	return m_iXpos;
}

int CBaseObject::GetCurY()
{
	return m_iYpos;
}

int CBaseObject::GetObjectID()
{
	return m_iObjectID;
}
int CBaseObject::GetObjectType()
{
	return m_iObjectType;
}


int CBaseObject::GetSprite()
{
	return m_iNow;
}
bool CBaseObject::isEndFrame()
{
	return m_iFrameEnd;
}

void CBaseObject::ActionInput(DWORD Action)
{
	m_dwActionInput = Action;
}
DWORD CBaseObject::GetAction()
{
	return m_dwActionInput;
}

void CBaseObject::SetPositionX(int Xpos)
{
	m_iXpos = Xpos;
}
void CBaseObject::SetPositionY(int Ypos)
{

	m_iYpos = Ypos;
}
void CBaseObject::SetPosition(int Xpos, int Ypos)
{
	SetPositionX(Xpos);
	SetPositionY(Ypos);
}

void CBaseObject::SetObjectID(int iD)
{
	m_iObjectID = iD;
}
void CBaseObject::SetObjectType(int Type)
{
	m_iObjectType = Type;
}

void CBaseObject::SetSprite(int spriteNow, int end, int delay)
{

	m_iStart = m_iNow = spriteNow;
	m_iFrameDelay = delay;
	m_iEnd = end; 
	m_iFrameEnd = false;

}