#include "stdafx.h"
#include "Valtan_BT_Phase2.h"
#include "Monster.h"
#include "Model.h"
#include <Boss.h>
#include "Transform.h"

CValtan_BT_Phase2::CValtan_BT_Phase2()
{
}

void CValtan_BT_Phase2::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CValtan_BT_Phase2::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Phase2::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss*>(m_pGameObject)->Set_Phase(2);
}



CValtan_BT_Phase2* CValtan_BT_Phase2::Create(void* pArg)
{
	CValtan_BT_Phase2* pInstance = new CValtan_BT_Phase2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Phase2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Phase2::Free()
{
	__super::Free();
}