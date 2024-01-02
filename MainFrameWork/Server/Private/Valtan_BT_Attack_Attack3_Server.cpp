#include "stdafx.h"
#include "Valtan_BT_Attack_Attack3_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack3_Server::CValtan_BT_Attack_Attack3_Server()
{
}

void CValtan_BT_Attack_Attack3_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack3_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_vecAnimDesc[m_iCurrAnimation].bIsLoop && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iCurrAnimation].iAnimIndex)
		static_cast<CMonster_Server*>(m_pGameObject)->Move_Dir(static_cast<CMonster_Server*>(m_pGameObject)->Get_Target_Direction(), static_cast<CMonster_Server*>(m_pGameObject)->Get_MoveSpeed() * 0.5f,fTimeDelta );

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack3_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack3_Server* CValtan_BT_Attack_Attack3_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack3_Server* pInstance = new CValtan_BT_Attack_Attack3_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack3_Server::Free()
{
	__super::Free();
}
