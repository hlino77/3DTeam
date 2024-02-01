#pragma once
#include "Client_Defines.h"
#include "State.h"
#include "Controller_WR.h"

BEGIN(Client)

class CState_MG_Fall final : public CState
{
public:
	CState_MG_Fall(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual ~CState_MG_Fall() = default;

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

	std::function<void(CState_MG_Fall&, _float)> m_TickFunc;

private:
	//Animation
	_float m_fStartAcc = { 0.0f };
	_float m_fStartTime = { 0.1f };

	_float m_bStart = { false };

	_float m_fTimeAcc = { 0.0f };
	_float m_fFallTime = { 5.f };
	_float m_fGravityTime = { 0.4f };

	Vec3 m_vFallDir;

public:
	static CState_MG_Fall* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual void Free() override;
};

END