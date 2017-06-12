#pragma once

class CSprite;

class CBaseObject
{
public:
	enum OBJECTTYPE
	{
		Player = 300,
		Effect = 301
	};
protected:
	CSprite *m_Sprite;

	int m_iXpos;
	int m_iYpos;
	
	// =======================================================
	// 스프라이트 애니메이션 처리
	// =======================================================
	int m_iStart;
	int m_iEnd;
	int m_iNow;

	bool m_iFrameEnd;
	int m_iFrameDelay;
	int m_iFrameNow;
	int m_iDelayCount;

	int m_iObjectID;
	int m_iObjectType;

	DWORD m_dwActionInput;
public:
	CBaseObject(int Xpos, int YPos, int SpriteStart, int SpriteEnd, int SpriteNow, int SpriteFrameDelay,
		int ObjectID, int ObjectType, CSprite *sprite);

	virtual ~CBaseObject();

	virtual bool Action() = 0;
	virtual bool Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch) = 0;

	virtual void NextFrame();

	int GetCurX();
	int GetCurY();
	int GetObjectID();
	int GetObjectType();


	int GetSprite();
	bool isEndFrame();

	void ActionInput(DWORD Action);
	DWORD GetAction();

	void SetPositionX(int Xpos);
	void SetPositionY(int Ypos);
	void SetPosition(int Xpos, int Ypos);

	void SetObjectID(int iD);
	void SetObjectType(int Type);

	void SetSprite(int sprite, int end, int delay);
};

