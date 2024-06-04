#include "stdafx.h"
#include "Common_BT_Damage1.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Damage1::CCommon_BT_Damage1()
{
}

void CCommon_BT_Damage1::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Damage1::OnUpdate(_float fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_Damage1::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);

}



CCommon_BT_Damage1* CCommon_BT_Damage1::Create(void* pArg)
{
	CCommon_BT_Damage1* pInstance = new CCommon_BT_Damage1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Damage1::Free()
{
	__super::Free();
}
