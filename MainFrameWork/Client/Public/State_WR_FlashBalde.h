#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CEffect;
class CState_WR_FlashBalde final : public CState_Skill
{
public:
	CState_WR_FlashBalde(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_FlashBalde() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_FlashBlade_Trail();

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;

	std::function<void(CState_WR_FlashBalde&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iFlashBlade = 0;

	//Effect
	_bool m_bTrailStart = false;
	_bool m_bEffectStart = false;
	vector<CEffect*> m_Trail;

public:
	static CState_WR_FlashBalde* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END