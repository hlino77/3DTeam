#include "stdafx.h"
#include "Golem_BT_Attack_Jump_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include "Boss_Golem_Server.h"

CGolem_BT_Attack_Jump_Server::CGolem_BT_Attack_Jump_Server()
{
}

void CGolem_BT_Attack_Jump_Server::OnStart()
{
	__super::OnStart(0);

	static_cast<CBoss_Golem_Server*>(m_pGameObject)->Set_RootTargetDistance(0.f);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Jump_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if(m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CGolem_BT_Attack_Jump_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CBoss_Golem_Server*>(m_pGameObject)->Set_RootTargetDistance(0.5f);
}



CGolem_BT_Attack_Jump_Server* CGolem_BT_Attack_Jump_Server::Create(void* pArg)
{
	CGolem_BT_Attack_Jump_Server* pInstance = new CGolem_BT_Attack_Jump_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Jump_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Jump_Server::Free()
{
	__super::Free();
}
