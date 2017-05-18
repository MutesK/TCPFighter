#pragma once
#include "BaseObject.h"

class CEffect : public CBaseObject
{
public:
	CEffect(int Xpos, int YPos, int SpriteStart, int SpriteEnd, int SpriteNow, int SpriteFrameDelay,
		int ObjectID, int ObjectType, CSprite *sprit, DWORD AttackID);
	virtual ~CEffect();


	virtual bool Action();
	virtual bool Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

protected:
	bool m_bEffectStart;
	DWORD m_dwAttackID;
};

