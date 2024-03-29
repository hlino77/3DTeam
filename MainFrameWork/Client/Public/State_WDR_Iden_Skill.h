#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_WDR_Iden_Skill final : public CState
{
public:
	CState_WDR_Iden_Skill(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_Iden_Skill() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_Shot();

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_Iden_Skill&, _float)> m_TickFunc;

private:
	//Animation
	_int m_Attack = 0;

	_uint m_iAttackCnt = 0;
	vector<_int> m_AttackFrames;


	_bool m_EffectSound  = false;
	_bool m_EffectSound1 = false;
	_bool m_EffectSound2 = false;

public:
	static CState_WDR_Iden_Skill* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END