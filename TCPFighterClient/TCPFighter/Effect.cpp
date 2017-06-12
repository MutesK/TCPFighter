#include "stdafx.h"
#include "Effect.h"
#include "player.h"
#include "Sprite.h"
#include "GameDefine.h"
#include "ScrollMap.h"
#include <list>
using namespace std;

extern list<CBaseObject *> List;
extern CScrollMap Map;

// 맞는사람의 Xpos와 Ypos
CEffect::CEffect(int Xpos, int YPos, int SpriteStart, int SpriteEnd, int SpriteNow, int SpriteFrameDelay,
	int ObjectID, int ObjectType, CSprite *sprite, DWORD AttackID)
 : 	CBaseObject(Xpos, YPos, SpriteStart, SpriteEnd, SpriteNow, SpriteFrameDelay, ObjectID, ObjectType, sprite)
{
	m_dwAttackID = AttackID;

	m_iNow = SpriteStart;

	m_iFrameDelay = dfDELAY_EFFECT;
}

CEffect::~CEffect()
{
}


bool CEffect::Action()
{
	if (!m_bEffectStart)
	{
		list<CBaseObject *>::iterator iter;
		for (iter = List.begin(); iter != List.end(); iter++)
		{
			if ((*iter)->GetObjectID() == m_dwAttackID)
			{
				int SpriteIndex = (*iter)->GetSprite();

				printf("CEffect : Attacker's Sprite Index = %d \n", SpriteIndex);

				switch (SpriteIndex)
				{
				case sPlayer_Attack1_Left2:
				case sPlayer_Attack2_Left2:
				case sPlayer_Attack3_Left4:
				case sPlayer_Attack1_Right2:
				case sPlayer_Attack2_Right2:
				case sPlayer_Attack3_Right4:
					m_bEffectStart = true;
				}
				break;
			}
		}
		
	}
	else
	{
		if (!isEndFrame())
			NextFrame();
		else
			return false;
	}

	return true;
}

bool CEffect::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	if (isEndFrame())
		return false;

	if (m_bEffectStart)
	{
		Map.DrawSprite(m_iNow, m_iXpos, m_iYpos, bypDest, iDestWidth, iDestHeight, iDestPitch);

		return true;
	}
	return true;
}