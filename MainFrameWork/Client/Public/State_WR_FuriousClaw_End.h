#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WR_FuriousClaw_End final : public CState_Skill
{
public:
	CState_WR_FuriousClaw_End(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_FuriousClaw_End() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_FuriousClaw_End();

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;

	std::function<void(CState_WR_FuriousClaw_End&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iFuriousClaw_End = 0;

	_bool m_bEffectStart = false;
	_bool m_bTrail = false;

	vector<class CEffect*> m_Trails;

public:
	static CState_WR_FuriousClaw_End* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END