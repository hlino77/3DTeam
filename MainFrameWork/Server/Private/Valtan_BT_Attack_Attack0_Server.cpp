#include "stdafx.h"
#include "Valtan_BT_Attack_Attack0_Server.h"
#include "Boss_Server.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack0_Server::CValtan_BT_Attack_Attack0_Server()
{
}

void CValtan_BT_Attack_Attack0_Server::OnStart()
{
	__super::OnStart(0);
	Vec3 vDirection = Vec3(-2.f, 0.f, -0.25f);
	vDirection.Normalize();
	m_vLandPosition = static_cast<CBoss_Server*>(m_pGameObject)->Get_SpawnPosition() + vDirection * 3.f;
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack0_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vLandPosition);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
		static_cast<CMonster_Server*>(m_pGameObject)->Move_Dir(static_cast<CBoss_Server*>(m_pGameObject)->Get_SpawnPosition()- m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION)
		,0.5f* static_cast<CMonster_Server*>(m_pGameObject)->Get_MoveSpeed(),fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[10].iAnimIndex, m_fLoopTime < 1.5f)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack0_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack0_Server* CValtan_BT_Attack_Attack0_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack0_Server* pInstance = new CValtan_BT_Attack_Attack0_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack0_Server::Free()
{
	__super::Free();
}
