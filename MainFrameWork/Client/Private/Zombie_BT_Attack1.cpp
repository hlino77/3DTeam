#include "stdafx.h"
#include "Zombie_BT_Attack1.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Attack1::CZombie_BT_Attack1()
{
}

void CZombie_BT_Attack1::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Attack1::OnUpdate(const _float& fTimeDelta)
{

	if (static_cast<CMonster*>(m_pGameObject)->Is_Hit()|| static_cast<CMonster*>(m_pGameObject)->Get_NearTarget() == nullptr)
		return BT_FAIL;
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CZombie_BT_Attack1::OnEnd()
{
	__super::OnEnd();

}



CZombie_BT_Attack1* CZombie_BT_Attack1::Create(void* pArg)
{
	CZombie_BT_Attack1* pInstance = new CZombie_BT_Attack1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Attack1::Free()
{
	__super::Free();
}
