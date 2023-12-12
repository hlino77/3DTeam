#include "stdafx.h"
#include "Zombie_BT_DamageLeft.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_DamageLeft::CZombie_BT_DamageLeft()
{
}

void CZombie_BT_DamageLeft::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_DamageLeft::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_DamageLeft::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);

}



CZombie_BT_DamageLeft* CZombie_BT_DamageLeft::Create(void* pArg)
{
	CZombie_BT_DamageLeft* pInstance = new CZombie_BT_DamageLeft;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_DamageLeft::Free()
{
	__super::Free();
}
