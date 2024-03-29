#include "stdafx.h"
#include "Valtan_BT_Attack_DoubleSwingChopSwing_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>

CValtan_BT_Attack_DoubleSwingChopSwing_Server::CValtan_BT_Attack_DoubleSwingChopSwing_Server()
{
}

void CValtan_BT_Attack_DoubleSwingChopSwing_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_DoubleSwingChopSwing_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() && static_cast<CBoss_Server*>(m_pGameObject)->Get_Phase() != 3)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);		
		return BT_SUCCESS;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_DoubleSwingChopSwing_Server::OnEnd()
{
	__super::OnEnd();
	if(static_cast<CBoss_Server*>(m_pGameObject)->Get_Phase()==3)
		static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
	else
		static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_DoubleSwingChopSwing_Server* CValtan_BT_Attack_DoubleSwingChopSwing_Server::Create(void* pArg)
{
	CValtan_BT_Attack_DoubleSwingChopSwing_Server* pInstance = new CValtan_BT_Attack_DoubleSwingChopSwing_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_DoubleSwingChopSwing_Server::Free()
{
	__super::Free();
}
