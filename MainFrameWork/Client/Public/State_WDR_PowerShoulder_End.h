#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WDR_PowerShoulder_End final : public CState_Skill
{
public:
	CState_WDR_PowerShoulder_End(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_PowerShoulder_End() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_End();
	void	PrevEffect_End();

	void	Update_Effect_Circle(_float fTimeDelta);

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_PowerShoulder_End&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iPowerShoulder_End = 0;

	_float m_fCircle1TimeAcc = 0.0f;
	_float m_fCircle2TimeAcc = 0.0f;

	_float m_fCircle1Time = 0.0f;
	_float m_fCircle2Time = 0.0f;

	// Sound
	_bool   m_EffectSound = false;
	_bool   m_PlayerSound = false;
	_float  m_EffectSoundAccTime = 0.f;
	_float	m_PlayerSoundAccTime = 0.f;



public:
	static CState_WDR_PowerShoulder_End* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END
