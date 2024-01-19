#pragma once
#include "Client_Defines.h"
#include "State.h"
#include "Player_Controller_GN.h"

BEGIN(Client)

class CState_WDR_Grabbed final : public CState
{
public:
	CState_WDR_Grabbed(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual ~CState_WDR_Grabbed() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Enter_State() override;
	virtual void	Tick_State(_float fTimeDelta) override;
	virtual void	Exit_State() override;

public:
	void	Tick_State_Control(_float fTimeDelta);
	void	Tick_State_NoneControl(_float fTimeDelta);

private:
	class CPlayer_Destroyer* m_pPlayer = nullptr;

	std::function<void(CState_WDR_Grabbed&, _float)> m_TickFunc;

	void	To_GrabPos(_float fTimeDelta);
	void	ToNone_GrabPos(_float fTimeDelta);

private:
	//Animation
	_int m_iGrabbed = 0;

	Matrix m_SaveMatrix;

	CGameObject* m_pValtan = { nullptr };

public:
	static CState_WDR_Grabbed* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Destroyer* pOwner);
	virtual void Free() override;
};

END