#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>

namespace Server
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
		LEVEL_VALTANENTERANCE,
		LEVEL_VALTANMAIN,
		LEVEL_LOADING,
		LEVEL_TOOL_NPC,
		LEVEL_END
	};

	enum OBJ_TYPE { PLAYER, MONSTER, BOSS, NPC, SKILL, ESTHER, ITEM, TERRAIN, PROP, UI, COLMESH, EFFECT, SPAWNER, PART, PROJECTILE, DUNGEAN, OBJ_END };

	enum class LAYER_TYPE
	{
		LAYER_TERRAIN,
		LAYER_SKYBOX,
		LAYER_CAMERA,
		LAYER_BACKGROUND,
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_BOSS,
		LAYER_NPC,
		LAYER_SKILL,
		LAYER_ESTHER,
		LAYER_ITEM,
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
		LAYER_BODY_NPC,
		LAYER_ATTACK_MONSTER,
		LAYER_ATTACK_PLAYER,
		LAYER_ATTACK_BOSS,
		LAYER_SKILL_PLAYER,
		LAYER_SKILL_BOSS,
		LAYER_GRAB_BOSS,
		LAYER_BUFF_PLAYER,
		LAYER_SKILL_ESTHER,
		LAYER_BUFF_ESTHER,
		LAYER_SPAWNER,
		LAYER_SAFEZONE,
		LAYER_CHILD,
		LAYER_SKILL_PLAYER_BUFF,
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

	enum LEVELSTATE { INITREADY, PLAYERREADY, INITEND, STATEEND };

	enum class EVENTSTATE { EVENTSTART, READY, EVENT, EVENTEND, SETEND, TYPEEND };

	enum class EVENT
	{
		BERNSTART,
		EVENTEND
	};


	enum class CHR_CLASS { GUNSLINGER, SLAYER, DESTROYER, BARD, DOAGA, CLASSEND };


	enum class STATUSEFFECT { COUNTER, GROGGY, EARTHQUAKE, BUG, FEAR, SHOCK, STUN, SILENCE,  EFFECTEND };

	enum class CHAOSDUNGEANLEVEL { LEVEL1, LEVEL2, LEVEL3, LEVELEND };
	enum class CHAOSDUNGEANSTATE { READY, PROCEED, CLEAR, LEVELEND };

	enum class SKILL_TYPE
	{
		SKILL_RISINGSUN,
		SKILL_TELEPORTDOOR,
		SKILLEND
	};

	enum class ITEMPART { FACE, HELMET, SHOULDER, BODY, ARM, LEG, WEAPON, _END };
}

extern atomic<_uint> g_iObjectID;

using namespace Server;

