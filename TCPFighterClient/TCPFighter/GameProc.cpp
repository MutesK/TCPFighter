
#include "stdafx.h"
#include "GameDefine.h"
#include "GameProc.h"
#include "ScrollMap.h"
#include "Sprite.h"
#include "player.h"
#include "CScreenDIB.h"

CScreenDIB g_cScreenDib(640, 480, 32);
CSprite g_cSprite(80, 0xffffffff);
list<CBaseObject *> List;
CPlayer *g_pPlayer = nullptr;

CScrollMap Map;

extern HWND g_hWnd;
extern BOOL g_bActiveApp;
BOOL GameInit()
{
	g_cSprite.LoadDibSprite(sShadow, L"Data\\Shadow.bmp", dfSPRITE_MIDPOINT_SHADOW_x, dfSPRITE_MIDPOINT_SHADOW_y);

	g_cSprite.LoadDibSprite(sTile, L"Data\\Tile_01.bmp", 0, 0);

	g_cSprite.LoadDibSprite(sHPGuage, L"Data\\HPGuage.bmp", dfSPRITE_MIDPOINT_HPGUAGE_x, dfSPRITE_MIDPOINT_HPGUAGE_y);
	g_cSprite.LoadDibSprite(sxSpark_1, L"Data\\xSpark_1.bmp", dfSPRITE_MIDPOINT_EFFECT_x, dfSPRITE_MIDPOINT_EFFECT_y);
	g_cSprite.LoadDibSprite(sxSpark_2, L"Data\\xSpark_2.bmp", dfSPRITE_MIDPOINT_EFFECT_x, dfSPRITE_MIDPOINT_EFFECT_y);
	g_cSprite.LoadDibSprite(sxSpark_3, L"Data\\xSpark_3.bmp", dfSPRITE_MIDPOINT_EFFECT_x, dfSPRITE_MIDPOINT_EFFECT_y);
	g_cSprite.LoadDibSprite(sxSpark_MAX, L"Data\\xSpark_4.bmp", dfSPRITE_MIDPOINT_EFFECT_x, dfSPRITE_MIDPOINT_EFFECT_y);
	// 케릭터 설정
	g_cSprite.LoadDibSprite(sPlayer_Stand_Left1, L"Data\\Stand_L_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Left2, L"Data\\Stand_L_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Left3, L"Data\\Stand_L_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Left4, L"Data\\Stand_L_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Stand_Left_MAX, L"Data\\Stand_L_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Right1, L"Data\\Stand_R_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Right2, L"Data\\Stand_R_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Right3, L"Data\\Stand_R_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Right4, L"Data\\Stand_R_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Stand_Right_MAX, L"Data\\Stand_R_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Attack1_Left1, L"Data\\Attack1_L_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Left2, L"Data\\Attack1_L_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Left3, L"Data\\Attack1_L_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Left4, L"Data\\Attack1_L_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Left_MAX, L"Data\\Attack1_L_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Attack2_Left1, L"Data\\Attack2_L_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Left2, L"Data\\Attack2_L_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Left3, L"Data\\Attack2_L_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Left4, L"Data\\Attack2_L_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Left_MAX, L"Data\\Attack2_L_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Attack3_Left1, L"Data\\Attack3_L_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Left2, L"Data\\Attack3_L_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Left3, L"Data\\Attack3_L_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Left4, L"Data\\Attack3_L_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Left5, L"Data\\Attack3_L_05.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Left6, L"Data\\Attack3_L_06.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Left_MAX, L"Data\\Attack3_L_06.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Move_Left1, L"Data\\Move_L_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left2, L"Data\\Move_L_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left3, L"Data\\Move_L_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left4, L"Data\\Move_L_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left5, L"Data\\Move_L_05.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left6, L"Data\\Move_L_06.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left7, L"Data\\Move_L_07.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left8, L"Data\\Move_L_08.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left9, L"Data\\Move_L_09.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left10, L"Data\\Move_L_10.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left11, L"Data\\Move_L_11.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Left_MAX, L"Data\\Move_L_12.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Attack1_Right1, L"Data\\Attack1_R_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Right2, L"Data\\Attack1_R_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Right3, L"Data\\Attack1_R_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Right4, L"Data\\Attack1_R_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack1_Right_MAX, L"Data\\Attack1_R_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Attack2_Right1, L"Data\\Attack2_R_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Right2, L"Data\\Attack2_R_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Right3, L"Data\\Attack2_R_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Right4, L"Data\\Attack2_R_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack2_Right_MAX, L"Data\\Attack2_R_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Attack3_Right1, L"Data\\Attack3_R_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Right2, L"Data\\Attack3_R_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Right3, L"Data\\Attack3_R_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Right4, L"Data\\Attack3_R_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Right5, L"Data\\Attack3_R_05.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Right6, L"Data\\Attack3_R_06.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Attack3_Right_MAX, L"Data\\Attack3_R_06.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);

	g_cSprite.LoadDibSprite(sPlayer_Move_Right1, L"Data\\Move_R_01.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right2, L"Data\\Move_R_02.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right3, L"Data\\Move_R_03.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right4, L"Data\\Move_R_04.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right5, L"Data\\Move_R_05.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right6, L"Data\\Move_R_06.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right7, L"Data\\Move_R_07.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right8, L"Data\\Move_R_08.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right9, L"Data\\Move_R_09.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right10, L"Data\\Move_R_10.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right11, L"Data\\Move_R_11.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	g_cSprite.LoadDibSprite(sPlayer_Move_Right_MAX, L"Data\\Move_R_12.bmp", dfSPRITE_MIDPOINT_CHARETER_x, dfSPRITE_MIDPOINT_CHARETER_y);
	
	
	return true;

}

void Update()
{
	
	if (g_bActiveApp)
	KeyProcess();

	Action();

	
	if (FrameSkip())
		Draw();
		return;
		
}

void KeyProcess()
{
	if (g_pPlayer == nullptr)
		return;

	int Action = dfACTION_STAND;



	if ((GetAsyncKeyState(VK_UP) & 0x8000) && (GetAsyncKeyState(VK_LEFT) & 0x8000))
	{
		Action = dfACTION_MOVE_LU;
	}

	else if ((GetAsyncKeyState(VK_UP) & 0x8000) && (GetAsyncKeyState(VK_RIGHT) & 0x8000))
	{

		Action = dfACTION_MOVE_RU;
	}

	else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && (GetAsyncKeyState(VK_LEFT) & 0x8000))
	{
		Action = dfACTION_MOVE_LD;
	}

	else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && (GetAsyncKeyState(VK_RIGHT) & 0x8000))
	{

		Action = dfACTION_MOVE_RD;
	}

	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{

		Action = dfACTION_MOVE_LL;
	}

	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{

		Action = dfACTION_MOVE_RR;
	}

	else if (GetAsyncKeyState(VK_UP) & 0x8000)
	{

		Action = dfACTION_MOVE_UU;
	}

	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		Action = dfACTION_MOVE_DD;
	}
	else if (GetAsyncKeyState(0x5A) & 0x8000) // 공격
	{
		Action = dfACTION_ATTACK1;
	}
	else if (GetAsyncKeyState(0x58) & 0x8000)
	{
		Action = dfACTION_ATTACK2;
	}
	else if (GetAsyncKeyState(0x43) & 0x8000)
	{
		Action = dfACTION_ATTACK3;
	}

	g_pPlayer->ActionInput(Action);

}

void Action()
{
	list <CBaseObject *>::iterator iter;

	for (iter = List.begin(); iter != List.end(); iter++)
		(*iter)->Action();

	if (g_pPlayer != nullptr)
		Map.DrawSet(g_pPlayer->GetCurX(), g_pPlayer->GetCurY());

}

void Draw()
{
	BYTE *bypDest = g_cScreenDib.GetDibBuffer();
	int iDestWidth = g_cScreenDib.GetWidth();
	int iDestHeight = g_cScreenDib.GetHeight();
	int iDestPitch = g_cScreenDib.GetPitch();



	YAxisSort();
	
	//g_cSprite.DrawImage(sMap, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);
	Map.DrawTileMap(bypDest, iDestWidth, iDestHeight, iDestPitch);

	list <CBaseObject *>::iterator iter;
	for (iter = List.begin(); iter != List.end(); iter++)
		if (!(*iter)->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch))
		{
			CBaseObject *pObject = (*iter);
			List.erase(iter);
			delete pObject;
		}


	g_cScreenDib.DrawBuffer(g_hWnd);
}

BOOL FrameSkip()
{
	static DWORD dwTick;
	static DWORD iTick;
	static DWORD m_dwSystemTick = 0;
	static DWORD m_iOneFrameTick = 1000 / FPS;
	BYTE flag;


	dwTick = GetTickCount64();


	iTick += dwTick - m_dwSystemTick;
	flag = true;


	if (iTick < m_iOneFrameTick)
	{
		Sleep(m_iOneFrameTick - iTick);

		dwTick = GetTickCount64();
		iTick = m_iOneFrameTick;

		flag = true;
	}
	else if (iTick - m_iOneFrameTick >= m_iOneFrameTick)
	{
		flag = false;
	}


	m_dwSystemTick = GetTickCount64();
	iTick -= m_iOneFrameTick;

	return flag;

}


void YAxisSort()
{
	List.sort(Comp);
}

bool Comp(CBaseObject* first, CBaseObject * sec)
{
	if (first->GetObjectType() == CBaseObject::Effect)
		return false;

	if (sec->GetObjectType() == CBaseObject::Effect)
		return true;

	if (first->GetCurY() < sec->GetCurY())
		return true;
	

		return false;
}
