#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_GN_TargetDown_Start final : public CState_Skill
{
public:
	CState_GN_TargetDown_Start(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual ~CState_GN_TargetDown_Start() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	void	Reset_Camera();
	void	Update_Camera(_float fTimeDelta);

	void	Init_CameraOffset();
private:
	class CPlayer_Gunslinger* m_pPlayer = nullptr;

	std::function<void(CState_GN_TargetDown_Start&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iTargetDown_Start = 0;

	Vec3 m_vCameraOffset;
	Vec3 m_vTargetOffset;

	_bool m_bZoomOut = false;

public:
	static CState_GN_TargetDown_Start* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Gunslinger* pOwner);
	virtual void Free() override;
};

END