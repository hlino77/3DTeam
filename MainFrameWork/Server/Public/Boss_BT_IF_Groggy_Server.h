#pragma once
#include "Server_Defines.h"
#include "BT_Decorator.h"
#include "Boss_Server.h"

BEGIN(Server)

class CBoss_BT_IF_Groggy_Server :
    public CBT_Decorator
{
private:
	CBoss_BT_IF_Groggy_Server() = default;
	CBoss_BT_IF_Groggy_Server(const CBoss_BT_IF_Groggy_Server& rhs) = delete;
	virtual ~CBoss_BT_IF_Groggy_Server() = default;

	virtual void OnStart() override
	{

	}

	virtual BT_RETURN OnUpdate(const _float & fTimeDelta) override
	{
		m_bCondition = Is_Countered();
		return __super::OnUpdate(fTimeDelta);
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	Is_Countered()
	{
		if (static_cast<CBoss_Server*>(m_pGameObject)->Is_Groggy())
			return true;

		return false;
	}

public:
	static	CBoss_BT_IF_Groggy_Server* Create(void* pArg)
	{
		CBoss_BT_IF_Groggy_Server* pInstance = new CBoss_BT_IF_Groggy_Server;

		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed to Created : CBoss_BT_IF_Groggy_Server");
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