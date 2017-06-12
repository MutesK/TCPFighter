#pragma once
#include "BaseObject.h"

class CPlayer : public CBaseObject
{


public:
	CPlayer(int Xpos, int Ypos, int m_SpriteStart, int m_SpriteEnd, int FrameDelay, int ObjectID, int ObjectType, CSprite *sprite, bool);
	virtual ~CPlayer();

	virtual bool Action();
	virtual bool Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

	void ActionProc();

	int GetDirection();
	void SetDirection(int);

	int GetHP();
	void SetHp(int HP);
	bool isPlayer();


protected:
	bool m_bPlayer;

	// 체력
	int m_chHP;
	
	// 액션
	DWORD m_dwActionCur;
	DWORD m_dwActionOld;

	int m_iDirectionCur;
	int m_iDirectionOld;

protected:
	void InputActionProc();
	void SetActionStand();

	void SetActionAttack1();
	void SetActionAttack2();
	void SetActionAttack3();
	void SetActionMove(int iAction);

	
};

