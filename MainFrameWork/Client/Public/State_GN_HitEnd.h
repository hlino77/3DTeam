#pragma once
#include "Client_Defines.h"
#include "State.h"
#include "Player_Controller_GN.h"

BEGIN(Client)

class CState_GN_HitEnd final : public CState
{
public:
	CState_GN_HitEnd(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_HitEnd() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_HitEnd&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iHitEnd = 0;

	_int m_iHitEnd_KnockDown = 0;
	_int m_iHitEnd_Bound = 0;
	_int m_iHitEnd_Twist = 0;

	_float m_fTimeAcc = 0.f;
	_float m_fDownTime = 3.f;

	CPlayer_Controller::HIT_TYPE m_eHitType;
	_float m_fForceDist;
	_float m_fHitCheck;

	_bool	m_IsAnimEnd = { false };

public:
	static CState_GN_HitEnd* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END