#include "stdafx.h"
#include "Common_BT_Spawn_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Spawn_Server::CCommon_BT_Spawn_Server()
{
}

void CCommon_BT_Spawn_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Find_NearTarget(1.f);
	static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Spawn_Server::OnUpdate(const _float& fTimeDelta)
{
	if (IsOut())
		return BT_FAIL;
	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Spawn_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Spawn(false);
}

_bool CCommon_BT_Spawn_Server::IsOut()
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return true;

	return false;
}

CCommon_BT_Spawn_Server* CCommon_BT_Spawn_Server::Create(void* pArg)
{
	CCommon_BT_Spawn_Server* pInstance = new CCommon_BT_Spawn_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Spawn_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Spawn_Server::Free()
{
	__super::Free();
}
