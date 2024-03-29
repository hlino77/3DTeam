#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)
class CEffect;
class CUI_HoldingFrame;
class CState_WR_VolcanoEruption_Loop final : public CState_Skill
{
public:
	CState_WR_VolcanoEruption_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_VolcanoEruption_Loop() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void Effect_VolcanoEruption_Charge_Control(_float fTimeDelta);
	void Effect_VolcanoEruption_Charge_NonControl(_float fTimeDelta);

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;
	CUI_HoldingFrame* m_pHoldingUI = { nullptr };
	std::function<void(CState_WR_VolcanoEruption_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iVolcano_Loop = 0;

	//Effect
	_bool m_bEffectStart = false;
	vector<CEffect*> m_Effect;

public:
	static CState_WR_VolcanoEruption_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END