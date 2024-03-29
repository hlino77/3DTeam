#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;
class CUI_HoldingFrame;
class CState_WR_BrutalImpact_Loop final : public CState_Skill
{
public:
	CState_WR_BrutalImpact_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_BrutalImpact_Loop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_BrutalImpact_Loop_Aura_Impact2();
	void	Effect_BrutalImpact_Loop_Impact3_NonControl();
	void	Effect_BrutalImpact_Loop_Impact3_Control();

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;
	CUI_HoldingFrame* m_pHoldingUI = { nullptr };
	std::function<void(CState_WR_BrutalImpact_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iBrutalImpact_Loop = 0;

	//Effect
	_bool m_bEffectStart[3] = { false, false, false };

public:
	static CState_WR_BrutalImpact_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END