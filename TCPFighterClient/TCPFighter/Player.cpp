#include "stdafx.h"
#include "GameDefine.h"
#include "Player.h"
#include "Sprite.h"

#include "PacketDefine.h"
#include "PacketSend.h"
#include "ScrollMap.h"
 

extern CScrollMap Map;



////////////////////////////////////////////////////////////////////
// ������, �ı���.
//
////////////////////////////////////////////////////////////////////
CPlayer::CPlayer(int Xpos, int Ypos, int m_SpriteStart, int m_SpriteEnd, int FrameDelay, int ObjectID, int ObjectType, CSprite *sprite, bool bPlayerCharacter) 
	: CBaseObject(Xpos, Ypos, m_SpriteStart, m_SpriteEnd, m_SpriteStart, FrameDelay, ObjectID, ObjectType, sprite)
{
	m_dwActionCur = dfACTION_STAND;
	m_dwActionOld = dfACTION_STAND;

	m_iDirectionCur = dfDIR_LEFT;
	m_iDirectionOld = dfDIR_LEFT;

	m_iNow = sPlayer_Stand_Left1;
	m_iStart = m_iNow;
	m_iEnd = sPlayer_Stand_Left_MAX;

	m_iFrameDelay = dfDELAY_STAND;

	m_chHP = 100;

	m_bPlayer = bPlayerCharacter;
}


CPlayer::~CPlayer()
{

}


bool CPlayer::Action(void)
{
	//------------------------------------------------------------
	// ���� ���������� ���ϸ��̼�.
	//------------------------------------------------------------
	NextFrame();
	//------------------------------------------------------------
	// m_dwActionPram �� ���� ������ �����Ѵ�.
	//------------------------------------------------------------
	ActionProc();

	return true;

}

////////////////////////////////////////////////////////////////////
// ������Ʈ �׸��� �����Լ�.
//
// Parameters: (BYTE *)DestPtr. (int)DestWidth. (int)DestHeight. (int)DestPitch.
// Return: (DWORD)Value.
////////////////////////////////////////////////////////////////////
bool CPlayer::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{

	Map.PlayerDraw(GetSprite(), m_iXpos, m_iYpos, bypDest, iDestWidth, iDestHeight, iDestPitch, isPlayer(), GetHP());

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// ������Ʈ �׼� ���� �Լ�. 
// ���̽� Ŭ������ m_dwActionParam ���� ���� ������ �������ش�.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayer::ActionProc(void)
{
	//------------------------------------------------------------
	// ��� ���۽�(����)�� ��� ���������� �ش� ���� ó���� �Ϸ��ؾƸ� �Ѵ�.
	//------------------------------------------------------------
	switch (m_dwActionCur)
	{
		//------------------------------------------------------------
		// ���ݵ��� �Ǵ� ������ ������ ���ϸ��̼��� ���������� ���������� ���ϸ��̼���
		// �Ǿ�߸� �ϸ�, ���ϸ��̼��� ���� �� �⺻�������� �ڵ����� ���ư��� �Ѵ�.
		//------------------------------------------------------------
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:

		if (isEndFrame())
		{
			SetActionStand();
			//------------------------------------------------------------
			// ������ �����������, �׼��� �ٲ��༭ �������� ������ �Ҷ�
			// �� ������ �����ϵ��� �Ѵ�.
			//------------------------------------------------------------
			m_dwActionInput = dfACTION_STAND;

		}
		break;


	default:
		//------------------------------------------------------------
		// �̿��� ��쿡�� ����� �Է� ó���� ���ش�.
		//------------------------------------------------------------
		InputActionProc();
		break;
	}
}



////////////////////////////////////////////////////////////////////
// ����� �Է� �׼ǿ� ���� ������ �������ش�.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayer::InputActionProc(void)
{
	//------------------------------------------------------------
	// ���� �׼ǰ�, ���� �׼��� ���Ͽ� � ������ ���Ӱ�
	// ����Ǿ����� Ȯ���Ͽ� ��Ŷ�� �������� ������.
	//------------------------------------------------------------
	m_dwActionOld = m_dwActionCur;



	//------------------------------------------------------------
	// � ���۵� ������� �⺻���� ���ĵ�� �ٲ��ش�.
	//------------------------------------------------------------
	if (dfACTION_STAND == m_dwActionInput)
	{
		SetActionStand();
	}


	//------------------------------------------------------------
	// ���� �Է��� ���� ��� ���� �������� �ٲ��ش�.
	//------------------------------------------------------------
	if (dfACTION_ATTACK1 == m_dwActionInput)
	{
		SetActionAttack1();
	}
	else if (dfACTION_ATTACK2 == m_dwActionInput)
	{
		SetActionAttack2();
	}
	else if (dfACTION_ATTACK3 == m_dwActionInput)
	{
		SetActionAttack3();
	}


	//------------------------------------------------------------
	// 8���⿡ ���� üũ�� �ؼ� �̵���Ű���� �Ѵ�.
	// ��Ʈ��ũ ���� 8�������� �̵��ؾ� �ϱ� ������...
	//
	// �̵����� ���� üũ�� �Ͽ� ó���Ѵ�.
	//------------------------------------------------------------
	//------------------------------------------------------------
	// �̵� ���� üũ �� �̵� ó��
	//------------------------------------------------------------
	switch (m_dwActionInput)
	{
	case dfACTION_MOVE_LL:
	{
		if (dfRANGE_MOVE_LEFT < m_iXpos - dfSPEED_PLAYER_X)
			SetPosition(m_iXpos - dfSPEED_PLAYER_X, m_iYpos);

		SetDirection(dfDIR_LEFT);
		SetActionMove(m_dwActionInput);
	}
	break;


	case dfACTION_MOVE_LU:
	{
		if (dfRANGE_MOVE_TOP < m_iYpos - dfSPEED_PLAYER_Y &&
			dfRANGE_MOVE_LEFT < m_iXpos - dfSPEED_PLAYER_X)

			SetPosition(m_iXpos - dfSPEED_PLAYER_X, m_iYpos - dfSPEED_PLAYER_Y);

		SetDirection(dfDIR_LEFT);
		SetActionMove(m_dwActionInput);
	}
	break;


	case dfACTION_MOVE_UU:
	{
		if (dfRANGE_MOVE_TOP < m_iYpos - dfSPEED_PLAYER_Y)
			SetPosition(m_iXpos, m_iYpos - dfSPEED_PLAYER_Y);

		SetActionMove(m_dwActionInput);
	}
	break;


	case dfACTION_MOVE_RU:
	{
		if (dfRANGE_MOVE_TOP < m_iYpos - dfSPEED_PLAYER_Y &&
			dfRANGE_MOVE_RIGHT > m_iXpos + dfSPEED_PLAYER_X)
			SetPosition(m_iXpos + dfSPEED_PLAYER_X, m_iYpos - dfSPEED_PLAYER_Y);

		SetDirection(dfDIR_RIGHT);
		SetActionMove(m_dwActionInput);
	}
	break;


	case dfACTION_MOVE_RR:
	{
		if (dfRANGE_MOVE_RIGHT > m_iXpos + dfSPEED_PLAYER_X)
			SetPosition(m_iXpos + dfSPEED_PLAYER_X, m_iYpos);

		SetDirection(dfDIR_RIGHT);
		SetActionMove(m_dwActionInput);
	}
	break;


	case dfACTION_MOVE_RD:
	{
		if (dfRANGE_MOVE_BOTTOM > m_iYpos + dfSPEED_PLAYER_Y &&
			dfRANGE_MOVE_RIGHT > m_iXpos + dfSPEED_PLAYER_X)
			SetPosition(m_iXpos + dfSPEED_PLAYER_X, m_iYpos + dfSPEED_PLAYER_Y);

		SetDirection(dfDIR_RIGHT);
		SetActionMove(m_dwActionInput);
	}
	break;


	case dfACTION_MOVE_DD:
	{
		if (dfRANGE_MOVE_BOTTOM > m_iYpos + dfSPEED_PLAYER_Y)
			SetPosition(m_iXpos, m_iYpos + dfSPEED_PLAYER_Y);

		SetActionMove(m_dwActionInput);
	}
	break;


	case dfACTION_MOVE_LD:
	{
		if (dfRANGE_MOVE_BOTTOM > m_iYpos + dfSPEED_PLAYER_Y &&
			dfRANGE_MOVE_LEFT < m_iXpos - dfSPEED_PLAYER_X)
			SetPosition(m_iXpos - dfSPEED_PLAYER_X, m_iYpos + dfSPEED_PLAYER_Y);

		SetDirection(dfDIR_LEFT);
		SetActionMove(m_dwActionInput);
	}
	break;
	}





	//------------------------------------------------------------
	// ���� �������� ���۰�, ���� �������� ������ ���Ͽ�
	// � ������ ��ȭ�� �־����� Ȯ���� ��, ��ȭ�� ������ ��Ŷ���� ������.
	//
	// ��, �÷��̾� ĳ������ ��쿡�� ���� ��ȭ�� ���� ��Ŷ�� ������ �Ѵ�.
	//------------------------------------------------------------
	if (m_dwActionCur == m_dwActionOld || !m_bPlayer)
		return;

	//------------------------------------------------------------
	// �̵��߿� �ƹ��� ���۵� �Է��� ���� ����ȰŶ�� 
	// �̵����� ��Ŷ�� ������ �Ѵ�.
	//------------------------------------------------------------
	if ((dfACTION_MOVE_LL <= m_dwActionOld && dfACTION_MOVE_LD >= m_dwActionOld)
		&& dfACTION_STAND == m_dwActionCur)
	{
		SendStop( GetDirection(), m_iXpos, m_iYpos); //Make Packet
	}
	else
	{
		//------------------------------------------------------------
		// ���� ������ ���� ��Ŷ�� �����.
		//------------------------------------------------------------
		switch (m_dwActionCur)
		{
		case dfACTION_ATTACK1:
			SendAttack1( GetDirection(), m_iXpos, m_iYpos);
			break;

		case dfACTION_ATTACK2:
			SendAttack2(GetDirection(), m_iXpos, m_iYpos);
			break;

		case dfACTION_ATTACK3:
			SendAttack3(GetDirection(), m_iXpos, m_iYpos);
			break;

		case dfACTION_MOVE_UU:
		case dfACTION_MOVE_RU:
		case dfACTION_MOVE_RR:
		case dfACTION_MOVE_RD:
		case dfACTION_MOVE_DD:
		case dfACTION_MOVE_LD:
		case dfACTION_MOVE_LL:
		case dfACTION_MOVE_LU:
			SendMove((BYTE)m_dwActionCur, m_iXpos, m_iYpos);
			break;
		}
	}

}

////////////////////////////////////////////////////////////////////
// HP ����.
//
// Parameters: (char)HP.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayer::SetHp(int chHP)
{
	m_chHP = chHP;
}

////////////////////////////////////////////////////////////////////
// HP ���
//
// Parameters: ����.
// Return: (int)HP.
////////////////////////////////////////////////////////////////////
int CPlayer::GetHP(void)
{
	return m_chHP;
}



// =================================================================
// ���� ����, ��������Ʈ ������ �ٲ��ִ� �Լ�.
// �� �Լ����� �����Լ���, SetAction() �Լ����� ȣ��ȴ�.
// =================================================================
////////////////////////////////////////////////////////////////////
// ������ �������� ����Ÿ�԰�, ��������Ʈ�� �������ش�.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayer::SetActionStand(void)
{
	//------------------------------------------------------------
	// ���� �⺻ ������ �ƴϾ��ų�, ������ �ٲ� �����
	// �⺻ ��������Ʈ�� ���� �ٲ��ش�.
	//------------------------------------------------------------
	if (dfACTION_STAND != m_dwActionCur || m_iDirectionOld != m_iDirectionCur)
	{
		SetDirection(m_iDirectionCur);
		//------------------------------------------------------------
		// ���⿡ ���� ���� ���� ����
		//------------------------------------------------------------
		if (dfDIR_LEFT == GetDirection())
		{
			SetSprite(sPlayer_Stand_Left1, sPlayer_Stand_Left_MAX, dfDELAY_STAND);
		}
		else if (dfDIR_RIGHT == GetDirection())
		{
			SetSprite(sPlayer_Stand_Right1, sPlayer_Stand_Right_MAX, dfDELAY_STAND);
		}
	}
	m_dwActionCur = dfACTION_STAND;
}



void CPlayer::SetActionMove(int iAction)
{
	//------------------------------------------------------------
	// ���� �ȱ� ������ �ƴϾ��ų�, ������ �ٲ� �����
	// �̵� ��������Ʈ�� ���� �ٲ��ش�.
	//------------------------------------------------------------
	if (!(dfACTION_MOVE_LL <= m_dwActionCur && dfACTION_MOVE_LD >= m_dwActionCur) || m_iDirectionOld != m_iDirectionCur)
	{
		SetDirection(m_iDirectionCur);

		if (dfDIR_LEFT == GetDirection())
		{
			SetSprite(sPlayer_Move_Left1, sPlayer_Move_Left_MAX, dfDELAY_MOVE);
		}
		else
		{
			SetSprite(sPlayer_Move_Right1, sPlayer_Move_Right_MAX, dfDELAY_MOVE);
		}
	}
	m_dwActionCur = iAction;
}



void CPlayer::SetActionAttack1(void)
{
	//------------------------------------------------------------
	// ���� �������� �ƴϾ��ٸ� �ٲ��ش�. �����߿� �� ������ �ȵǹǷ�...
	//------------------------------------------------------------
	if (dfACTION_ATTACK1 != m_dwActionCur)
	{
		if (dfDIR_LEFT == GetDirection())
		{
			SetSprite(sPlayer_Attack1_Left1, sPlayer_Attack1_Left_MAX, dfDELAY_ATTACK1);
		}
		else
		{
			SetSprite(sPlayer_Attack1_Right1, sPlayer_Attack1_Right_MAX, dfDELAY_ATTACK1);
		}
	}
	m_dwActionCur = dfACTION_ATTACK1;
}



void CPlayer::SetActionAttack2(void)
{
	//------------------------------------------------------------
	// ���� �������� �ƴϾ��ٸ� �ٲ��ش�. �����߿� �� ������ �ȵǹǷ�...
	//------------------------------------------------------------
	if (dfACTION_ATTACK2 != m_dwActionCur)
	{
		if (dfDIR_LEFT == GetDirection())
		{
			SetSprite(sPlayer_Attack2_Left1, sPlayer_Attack2_Left_MAX, dfDELAY_ATTACK2);
		}
		else
		{
			SetSprite(sPlayer_Attack2_Right1, sPlayer_Attack2_Right_MAX, dfDELAY_ATTACK2);
		}
	}

	m_dwActionCur = dfACTION_ATTACK2;

}

void CPlayer::SetActionAttack3(void)
{
	//------------------------------------------------------------
	// ���� �������� �ƴϾ��ٸ� �ٲ��ش�. �����߿� �� ������ �ȵǹǷ�...
	//------------------------------------------------------------
	if (dfACTION_ATTACK3 != m_dwActionCur)
	{
		if (dfDIR_LEFT == GetDirection())
		{
			SetSprite(sPlayer_Attack3_Left1, sPlayer_Attack3_Left_MAX, dfDELAY_ATTACK3);
		}
		else
		{
			SetSprite(sPlayer_Attack3_Right1, sPlayer_Attack3_Right_MAX, dfDELAY_ATTACK3);
		}
	}
	m_dwActionCur = dfACTION_ATTACK3;

}



////////////////////////////////////////////////////////////////////
// ������Ʈ ���� ����.
//
// Parameters: (int)Direction.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayer::SetDirection(int iDir)
{
	m_iDirectionOld = m_iDirectionCur;
	m_iDirectionCur = iDir;
}

int CPlayer::GetDirection(void)
{
	return m_iDirectionCur;
}


////////////////////////////////////////////////////////////////////
// �÷��̾� ��ü Ȯ��.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
bool CPlayer::isPlayer(void)
{
	return m_bPlayer;
}

