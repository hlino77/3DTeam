#pragma once

/* Ŭ���̾�Ʈ�� �����ϴ� ��� Ŭ������ ��Ŭ����ؾ��� ������ ��´�. */
#include <process.h>


namespace Client
{
	enum LEVELID 
	{ 
		LEVEL_STATIC, 
		LEVEL_TOOL, 
		LEVEL_LOGO, 
		LEVEL_SERVERSELECT, 
		LEVEL_LOBBY,
		LEVEL_BERN,
		LEVEL_CHAOS_1,
		LEVEL_CHAOS_2,
		LEVEL_CHAOS_3,
		LEVEL_LOADING,
		LEVEL_END 
	};

	enum OBJ_TYPE { PLAYER, MONSTER, BOSS, SKILL, TERRAIN, PROP, UI, COLMESH, EFFECT, SPAWNER, PART, PROJECTILE, DUNGEAN, OBJ_END };

	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;
	const float				g_fSizeRatio = 1.25f;
	extern float			g_fLoadingSizeX;

	enum class LAYER_TYPE
	{
		LAYER_TERRAIN,
		LAYER_SKYBOX,
		LAYER_CAMERA,
		LAYER_BACKGROUND,
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_BOSS,
		LAYER_SKILL,
		LAYER_EFFECT,
		LAYER_UI,
		LAYER_END
	};

	enum class LAYER_COLLIDER
	{
		LAYER_BODY_PLAYER,
		LAYER_BODY_MONSTER,
		LAYER_BODY_BOSS,
		LAYER_BODY_STATICMODEL,
		LAYER_ATTACK_MONSTER,
		LAYER_ATTACK_PLAYER,
		LAYER_ATTACK_BOSS,
		LAYER_SKILL_PLAYER,
		LAYER_BUFF_PLAYER,
		LAYER_SPAWNER,
		LAYER_CHILD,
		LAYER_END
	};


	enum class COLLIDER_ATTACK
	{
		MIDDLE,
		LEFT,
		RIGHT,
		SPINBLOWUP,
		SPINBLOWDOWN,
		ATTACK_END
	};

	enum LEVELSTATE { INITREADY, INITEND, STATEEND };

	enum class EVENTSTATE { EVENTSTART, READY, EVENT, EVENTEND, SETEND, TYPEEND };

	enum class EVENT
	{
		EVENTEND
	};

	enum CHANNELID {
		CHANNEL_BGM,
		CHANNEL_UI,
		CHANNEL_BACKGROUND,
		CHANNEL_LOOPSTART,
		CHANNEL_END = 29
	};

	enum class CHR_CLASS { GUNSLINGER, SLAYER, DESTROYER, BARD, CLASSEND };

	enum class STATUSEFFECT { COUNTER, GROGGY, EARTHQUAKE, BUG, FEAR, SHOCK, STUN, EFFECTEND };
}

#define EFFECT_START(name, pivotDesc)	CEffect_Manager::GetInstance()->Effect_Start(name, pivotDesc);

extern _float g_fVolume;
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
using namespace Client;

#define SERVER_IP		L"127.0.0.1"
#define SERVER_PORT		7777