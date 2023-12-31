#include "stdafx.h"
#include "Valtan_BT_Attack_Attack17.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack17::CValtan_BT_Attack_Attack17()
{
}

void CValtan_BT_Attack_Attack17::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack17::OnUpdate(const _float& fTimeDelta)
{

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack17::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack17* CValtan_BT_Attack_Attack17::Create(void* pArg)
{
	CValtan_BT_Attack_Attack17* pInstance = new CValtan_BT_Attack_Attack17;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack17::Free()
{
	__super::Free();
}
