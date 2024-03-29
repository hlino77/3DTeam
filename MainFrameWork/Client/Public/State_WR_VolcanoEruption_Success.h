#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_WR_VolcanoEruption_Success final : public CState_Skill
{
public:
	CState_WR_VolcanoEruption_Success(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual ~CState_WR_VolcanoEruption_Success() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Effect_VolcanoEruption_Explode_Control();
	void	Effect_VolcanoEruption_Explode_NonControl();

private:
	class CPlayer_Slayer* m_pPlayer = nullptr;

	std::function<void(CState_WR_VolcanoEruption_Success&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iVolcano_Success = 0;

	//Effect
	_bool m_bEffectStart = false;

public:
	static CState_WR_VolcanoEruption_Success* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Slayer* pOwner);
	virtual void Free() override;
};

END