#include "stdafx.h"
#include "Golem_BT_Attack_Jump.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "Boss_Golem.h"
#include <ColliderOBB.h>
#include <Skill.h>
#include "GameInstance.h"
#include <Effect_Manager.h>
CGolem_BT_Attack_Jump::CGolem_BT_Attack_Jump()
{
}

void CGolem_BT_Attack_Jump::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Set_RootTargetDistance(0.f);

	m_Shoot[0] = true;
	m_Shoot[1] = true;
};

CBT_Node::BT_RETURN CGolem_BT_Attack_Jump::OnUpdate(const _float& fTimeDelta)
{
	if (m_Shoot[0] && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex)
	{ 
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		EFFECT_START(TEXT("Chaos_Rook_Cross_Warnning"), &desc)

		m_Shoot[0] = false;
	}

	if ( m_Shoot[1] && 36 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex))
	{
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		EFFECT_START(TEXT("Chaos_Rook_Cross_Effects"), &desc)

		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Golem_Jump", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				vPos.y = 0.5f;
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				m_Shoot[1] = false;
			}
		}
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Golem_Jump", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
				vPos += vRight * 0.3f;
				vRight.Normalize();
				vPos.y = 0.5f;
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vRight);
				m_Shoot[1] = false;
			}
		}
	}

	return __super::OnUpdate(fTimeDelta);
}

void CGolem_BT_Attack_Jump::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_RootTargetDistance(static_cast<CMonster*>(m_pGameObject)->Get_AttackRange() * 0.7f);

}

CGolem_BT_Attack_Jump* CGolem_BT_Attack_Jump::Create(void* pArg)
{
	CGolem_BT_Attack_Jump* pInstance = new CGolem_BT_Attack_Jump;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Jump::Free()
{
	__super::Free();
}
