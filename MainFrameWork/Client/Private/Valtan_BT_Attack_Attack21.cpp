#include "stdafx.h"
#include "Valtan_BT_Attack_Attack21.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss.h>

CValtan_BT_Attack_Attack21::CValtan_BT_Attack_Attack21()
{
}

void CValtan_BT_Attack_Attack21::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack21::OnUpdate(const _float& fTimeDelta)
{
	if (m_vecAnimDesc[m_iCurrAnimation].bIsLoop && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iCurrAnimation].iAnimIndex && m_fLoopTime > 1.f)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
	if (m_vecAnimDesc[1].bIsLoop && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack21::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack21* CValtan_BT_Attack_Attack21::Create(void* pArg)
{
	CValtan_BT_Attack_Attack21* pInstance = new CValtan_BT_Attack_Attack21;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack21::Free()
{
	__super::Free();
}
