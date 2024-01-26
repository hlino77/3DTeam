#include "stdafx.h"
#include "Valtan_BT_Attack_Attack11_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_Attack11_Server::CValtan_BT_Attack_Attack11_Server()
{
}

void CValtan_BT_Attack_Attack11_Server::OnStart()
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Is_bDummy())
		__super::OnStart(1);
	else
		__super::OnStart(0);
	m_bShoot = true;
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack11_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 55 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex,
			m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame,
			m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	if (static_cast<CBoss_Server*>(m_pGameObject)->Is_bDummy() && m_iCurrAnimation == 1 && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[1].iAnimIndex) - 3)
	{
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Left(false);
		static_cast<CMonster_Server*>(m_pGameObject)->Set_AnimationSpeed(0.f);
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Die();
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[0].iAnimIndex)-3 && m_bShoot)
	{
		m_bShoot = false;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vRight.Normalize();
		vLook.Normalize();
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->BroadCast_Ghost(vPos, (-vLook + vRight) * 0.5f);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack11_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack11_Server* CValtan_BT_Attack_Attack11_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack11_Server* pInstance = new CValtan_BT_Attack_Attack11_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack11_Server::Free()
{
	__super::Free();
}
