#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>

namespace Server
{
	enum LEVELID { LEVEL_STATIC, LEVEL_TOOL, LEVEL_LOGO, LEVEL_SERVERSELECT, LEVEL_LOBBY, LEVEL_BERN, LEVEL_LOADING, LEVEL_END };

	enum OBJ_TYPE { PLAYER, MONSTER, BOSS, SKILL, TERRAIN, PROP, UI, COLMESH, EFFECT, SPAWNER, PART, PROJECTILE, OBJ_END };

	enum class LAYER_TYPE
	{
		LAYER_COLMESH,
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
		ARENASTART,
		PLAYERSTART,
		BATTLESTART,
		MADARAMETEOR,
		KONOHASTART,
		EVENTEND
	};


	enum class CHR_CLASS { GUNSLINGER, SLAYER, CLASSEND };


	enum class STATUSEFFECT { STUN, EFFECTEND };

}

extern atomic<_uint> g_iObjectID;

using namespace Server;

