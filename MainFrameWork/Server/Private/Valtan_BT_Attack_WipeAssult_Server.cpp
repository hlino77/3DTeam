#include "stdafx.h"
#include "Valtan_BT_Attack_WipeAssult_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>

CValtan_BT_Attack_WipeAssult_Server::CValtan_BT_Attack_WipeAssult_Server()
{
}

void CValtan_BT_Attack_WipeAssult_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Invincible(true);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_WipeAssult_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_WipeAssult_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Invincible(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_WipeAssult_Server* CValtan_BT_Attack_WipeAssult_Server::Create(void* pArg)
{
	CValtan_BT_Attack_WipeAssult_Server* pInstance = new CValtan_BT_Attack_WipeAssult_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_WipeAssult_Server::Free()
{
	__super::Free();
}
