#include "stdafx.h"
#include "Common_BT_Attack1.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"

CCommon_BT_Attack1::CCommon_BT_Attack1()
{
}

void CCommon_BT_Attack1::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CCommon_BT_Attack1::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() ==OBJ_TYPE::MONSTER)
	{
		if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackStartFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, true);
		if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackEndFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	}
	else if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::BOSS)
	{
		if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackStartFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);
		if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackEndFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
	}
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 5.f);
	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Attack1::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
}



CCommon_BT_Attack1* CCommon_BT_Attack1::Create(void* pArg)
{
	CCommon_BT_Attack1* pInstance = new CCommon_BT_Attack1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Attack1::Free()
{
	__super::Free();
}
