#include "stdafx.h"
#include "Valtan_BT_Attack_Attack19.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>

CValtan_BT_Attack_Attack19::CValtan_BT_Attack_Attack19()
{
}

void CValtan_BT_Attack_Attack19::OnStart()
{
	__super::OnStart();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack19::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[2].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext())
		m_pGameObject->Get_TransformCom()->Turn_Speed(m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_UP), XMConvertToRadians(280.f), fTimeDelta);

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack19::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack19* CValtan_BT_Attack_Attack19::Create(void* pArg)
{
	CValtan_BT_Attack_Attack19* pInstance = new CValtan_BT_Attack_Attack19;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack19::Free()
{
	__super::Free();
}
