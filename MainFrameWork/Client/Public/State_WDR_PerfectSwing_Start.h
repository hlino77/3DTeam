#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WDR_PerfectSwing_Start final : public CState_Skill
{
public:
	CState_WDR_PerfectSwing_Start(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_PerfectSwing_Start() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Init_Camera();
	void	Reset_Camera();
private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_PerfectSwing_Start&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iPerfectSwing_Start = 0;

	Vec3 m_vCameraOffset;

	// Sound
	_bool   m_EffectSound = false;
	_bool   m_PlayerSound = false;
	_float  m_EffectSoundAccTime = 0.f;
	_float	m_PlayerSoundAccTime = 0.f;


public:
	static CState_WDR_PerfectSwing_Start* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END