#include "stdafx.h"
#include "Valtan_BT_Attack_Attack10.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack10::CValtan_BT_Attack_Attack10()
{
}

void CValtan_BT_Attack_Attack10::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack10::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[2].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->Move_Dir(static_cast<CMonster*>(m_pGameObject)->Get_Target_Direction(),
			1.5f* static_cast<CMonster*>(m_pGameObject)->Get_MoveSpeed(),fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) < 10 || m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack10::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack10* CValtan_BT_Attack_Attack10::Create(void* pArg)
{
	CValtan_BT_Attack_Attack10* pInstance = new CValtan_BT_Attack_Attack10;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack10::Free()
{
	__super::Free();
}