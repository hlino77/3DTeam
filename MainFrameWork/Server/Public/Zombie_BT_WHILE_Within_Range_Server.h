#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Monster_Server.h"
BEGIN(Server)
class CZombie_BT_WHILE_Within_Range_Server :
    public CBT_Decorator
{
private:
	CZombie_BT_WHILE_Within_Range_Server() = default;
	CZombie_BT_WHILE_Within_Range_Server(const CZombie_BT_WHILE_Within_Range_Server& rhs) = delete;
	virtual ~CZombie_BT_WHILE_Within_Range_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_Within_Range();
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction(fTimeDelta);
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Within_Range()
	{
		if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance() < 10.f&& !static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
			return true;

		return false;
	}

public:
	static	CZombie_BT_WHILE_Within_Range_Server* Create(void* pArg)
	{
		CZombie_BT_WHILE_Within_Range_Server* pInstance = new CZombie_BT_WHILE_Within_Range_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CZombie_BT_WHILE_Within_Range_Server");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual void Free() override
	{
		__super::Free();
	}
};

END