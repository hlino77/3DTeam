#pragma once
#include "Client_Defines.h"
#include "State_Skill.h"
#include "Player_Controller.h"

BEGIN(Client)

class CState_MG_SongOfLight_Loop final : public CState_Skill
{
public:
	CState_MG_SongOfLight_Loop(const wstring& strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual ~CState_MG_SongOfLight_Loop() = default;

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

	std::function<void(CState_MG_SongOfLight_Loop&, _float)> m_TickFunc;

private:
	//Animation
	_int m_iSongOfLight = 0;

	Vec3 m_vColliPos;

public:
	static CState_MG_SongOfLight_Loop* Create(wstring strStateName, class CStateMachine* pMachine, class CPlayer_Controller* pController, class CPlayer_Bard* pOwner);
	virtual void Free() override;
};

END