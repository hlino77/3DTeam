#pragma once

#include "Server_Defines.h"
#include "Level.h"
#include <thread>

BEGIN(Server)

class CLevel_Bern_Server final : public CLevel
{
private:
	CLevel_Bern_Server();
	virtual ~CLevel_Bern_Server() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(const _float& fTimeDelta) override;
	virtual HRESULT LateTick(const _float& fTimeDelta) override;

	virtual HRESULT Exit() override;



private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Player(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_BackGround(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Monster(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_UI(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Effect(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Npc(const LAYER_TYPE eLayerType);


private:
	HRESULT			Broadcast_Character();
	void			Broadcast_LevelState(LEVELSTATE eState);
	void			Wait_ClientLevelState(LEVELSTATE eState);
	HRESULT			Broadcast_PlayerInfo();

	HRESULT			Ready_Events();


	//Test
	void			Enter_ChaosDungean(CHAOSDUNGEANLEVEL eLEVEL);
	void			Enter_ValtanMain();

public:
	static class CLevel_Bern_Server* Create();
	virtual void Free() override;


private:

private:
	_float m_fBroadcastTime = 0.0f;
	_float m_fNextLevelDelay = 0.0f;
	_bool m_bKey_Lock = false;
private:
	
	thread* m_pCollisionThread = nullptr;
};

END