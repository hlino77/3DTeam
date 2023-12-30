#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_MG_Idle final : public CState
{
public:
	CState_MG_Idle(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual ~CState_MG_Idle() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Bard* m_pPlayer = nullptr;

	std::function<void(CState_MG_Idle&, _float)> m_TickFunc;

private:
	void Change_Idle_Anim(_float fTimeDelta);

private:
	//Animation
	_int m_iIdle = 0;

	_int m_iIdle_N = 0;
	_int m_iIdle_SP = 0;

	_float m_fSPTimeAcc = 0.f;
	_float m_fSPTime = 10.f;

public:
	static CState_MG_Idle* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual void Free() override;
};

END