#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_MG_Esther_Way final : public CState
{
public:
	CState_MG_Esther_Way(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual ~CState_MG_Esther_Way() = default;

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

	std::function<void(CState_MG_Esther_Way&, _float)> m_TickFunc;

private:
	_int	m_iEsther = 0;
	_bool	m_bEstherActive = false;

public:
	static CState_MG_Esther_Way* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual void Free() override;
};

END