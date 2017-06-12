#pragma once


#define FPS 50

//-----------------------------------------------------------------
// Sprite 중점 정보
//-----------------------------------------------------------------

#define dfSPRITE_MIDPOINT_CHARETER_x 71
#define dfSPRITE_MIDPOINT_CHARETER_y 90

#define dfSPRITE_MIDPOINT_EFFECT_x 70
#define dfSPRITE_MIDPOINT_EFFECT_y 70

#define dfSPRITE_MIDPOINT_HPGUAGE_x 0
#define dfSPRITE_MIDPOINT_HPGUAGE_y 0

#define dfSPRITE_MIDPOINT_SHADOW_x 32
#define dfSPRITE_MIDPOINT_SHADOW_y 4

//-----------------------------------------------------------------
// 애니메이션 프레임 딜레이
//-----------------------------------------------------------------
#define dfDELAY_STAND	5
#define dfDELAY_MOVE	4
#define dfDELAY_ATTACK1	3
#define dfDELAY_ATTACK2	4
#define dfDELAY_ATTACK3	4
#define dfDELAY_EFFECT	3


//-----------------------------------------------------------------
// 화면 이동 범위.
//-----------------------------------------------------------------
#define dfRANGE_MOVE_TOP	0
#define dfRANGE_MOVE_LEFT	0
#define dfRANGE_MOVE_RIGHT	6400
#define dfRANGE_MOVE_BOTTOM	6400


//-----------------------------------------------------------------
// 캐릭터 이동 속도
//-----------------------------------------------------------------
#define dfSPEED_PLAYER_X	3
#define dfSPEED_PLAYER_Y	2


#define dfACTION_MOVE_LL		0
#define dfACTION_MOVE_LU		1
#define dfACTION_MOVE_UU		2
#define dfACTION_MOVE_RU		3
#define dfACTION_MOVE_RR		4
#define dfACTION_MOVE_RD		5
#define dfACTION_MOVE_DD		6
#define dfACTION_MOVE_LD		7

#define dfDIR_LEFT			0
#define dfDIR_RIGHT			4


#define dfACTION_ATTACK1		11
#define dfACTION_ATTACK2		12
#define dfACTION_ATTACK3		13

#define dfACTION_STAND			255
enum
{
	sMap = 0,

	sHPGuage,
	sShadow,

	sxSpark_1, sxSpark_2,
	sxSpark_3, sxSpark_MAX,

	sPlayer_Attack1_Left1, sPlayer_Attack1_Left2,
	sPlayer_Attack1_Left3, sPlayer_Attack1_Left4, sPlayer_Attack1_Left_MAX,

	sPlayer_Attack2_Left1, sPlayer_Attack2_Left2,
	sPlayer_Attack2_Left3, sPlayer_Attack2_Left4, sPlayer_Attack2_Left_MAX,

	sPlayer_Attack3_Left1, sPlayer_Attack3_Left2,
	sPlayer_Attack3_Left3, sPlayer_Attack3_Left4,
	sPlayer_Attack3_Left5, sPlayer_Attack3_Left6, sPlayer_Attack3_Left_MAX,

	sPlayer_Attack1_Right1, sPlayer_Attack1_Right2,
	sPlayer_Attack1_Right3, sPlayer_Attack1_Right4, sPlayer_Attack1_Right_MAX,

	sPlayer_Attack2_Right1, sPlayer_Attack2_Right2,
	sPlayer_Attack2_Right3, sPlayer_Attack2_Right4, sPlayer_Attack2_Right_MAX,

	sPlayer_Attack3_Right1, sPlayer_Attack3_Right2,
	sPlayer_Attack3_Right3, sPlayer_Attack3_Right4,
	sPlayer_Attack3_Right5, sPlayer_Attack3_Right6, sPlayer_Attack3_Right_MAX,

	sPlayer_Move_Left1, sPlayer_Move_Left2,
	sPlayer_Move_Left3, sPlayer_Move_Left4,
	sPlayer_Move_Left5, sPlayer_Move_Left6,
	sPlayer_Move_Left7, sPlayer_Move_Left8,
	sPlayer_Move_Left9, sPlayer_Move_Left10,
	sPlayer_Move_Left11, sPlayer_Move_Left_MAX,

	sPlayer_Move_Right1, sPlayer_Move_Right2,
	sPlayer_Move_Right3, sPlayer_Move_Right4,
	sPlayer_Move_Right5, sPlayer_Move_Right6,
	sPlayer_Move_Right7, sPlayer_Move_Right8,
	sPlayer_Move_Right9, sPlayer_Move_Right10,
	sPlayer_Move_Right11, sPlayer_Move_Right_MAX,

	sPlayer_Stand_Left1,
	sPlayer_Stand_Left2,
	sPlayer_Stand_Left3,
	sPlayer_Stand_Left4,
	sPlayer_Stand_Left_MAX,

	sPlayer_Stand_Right1,
	sPlayer_Stand_Right2,
	sPlayer_Stand_Right3,
	sPlayer_Stand_Right4,
	sPlayer_Stand_Right_MAX,

	sTile

};