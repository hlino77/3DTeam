#include "stdafx.h"
#include "Valtan_BT_Attack_Attack9.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>

CValtan_BT_Attack_Attack9::CValtan_BT_Attack_Attack9()
{
}

void CValtan_BT_Attack_Attack9::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack9::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 95 && m_bShoot[0])
	{
		m_bShoot[0] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		}
		if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		{
			static_cast<CMonster*>(m_pGameObject)->Set_Die(0.5f);
			static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.3f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 24 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		}
	}
	if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy() == false )
		if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) < 10 || m_pGameObject->Get_ModelCom()->IsNext())
			static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 2.f);
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 115 && m_iCurrAnimation == 0)
	{
			m_iCurrAnimation = 1;

			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		
	}
	if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
	{
		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 58 && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
			static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);

		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 95 && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
			static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack9::OnEnd()
{
	__super::OnEnd();
	if (!static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack9* CValtan_BT_Attack_Attack9::Create(void* pArg)
{
	CValtan_BT_Attack_Attack9* pInstance = new CValtan_BT_Attack_Attack9;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack9::Free()
{
	__super::Free();
}
