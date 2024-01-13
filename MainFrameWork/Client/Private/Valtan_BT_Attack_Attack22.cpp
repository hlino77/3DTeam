#include "stdafx.h"
#include "Valtan_BT_Attack_Attack22.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"


CValtan_BT_Attack_Attack22::CValtan_BT_Attack_Attack22()
{
}

void CValtan_BT_Attack_Attack22::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack22::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
	}
	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_fLoopTime > 0.75f && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime  && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[12].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[12].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[12].iAnimIndex) - 3 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation = 8;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		m_fLoopTime = 0;
		return BT_RUNNING;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack22::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack22* CValtan_BT_Attack_Attack22::Create(void* pArg)
{
	CValtan_BT_Attack_Attack22* pInstance = new CValtan_BT_Attack_Attack22;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack22::Free()
{
	__super::Free();
}
