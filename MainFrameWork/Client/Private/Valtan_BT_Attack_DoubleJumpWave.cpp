#include "stdafx.h"
#include "Valtan_BT_Attack_DoubleJumpWave.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include <Skill.h>
#include "Effect_Manager.h"
#include "Effect.h"

CValtan_BT_Attack_DoubleJumpWave::CValtan_BT_Attack_DoubleJumpWave()
{
}

void CValtan_BT_Attack_DoubleJumpWave::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;

	m_bEffect[0] = false;
	m_bEffect[1] = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_DoubleJumpWave::OnUpdate(const _float& fTimeDelta)
{
	if (m_bEffect[0] == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 93)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_DJW_Trail", &tDesc);

		m_bEffect[0] = true;
	}

	if (m_bEffect[1] == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 22)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_DJW_Trail", &tDesc);

		m_bEffect[1] = true;
	}

	if (m_bShoot[0] && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 95)
	{
		Add_Sound(L"Effect", L"Valtan#64 (963708198)");
		m_bShoot[0] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.05f, 50.0f, 0.1f, 10.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(30.f, -30.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(0.f, 30.f);
		}
		if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		{
			static_cast<CMonster*>(m_pGameObject)->Set_Die(0.5f);
			static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.3f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_DJW_Chop", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 24 && m_bShoot[1])
	{
		Add_Sound(L"Effect", L"Valtan#64 (963708198)");
		m_bShoot[1] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;		
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.05f, 50.0f, 0.1f, 10.0f);

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(30.f, -30.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(0.f, 30.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_DJW_Chop", &tDesc);
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
	{
		Add_Sound(0, 0, L"Effect", L"Valtan#17 (437813884)");
		Add_Sound(1, 1, L"Effect", L"Valtan#110 (618835666)");
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_DoubleJumpWave::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_DoubleJumpWave* CValtan_BT_Attack_DoubleJumpWave::Create(void* pArg)
{
	CValtan_BT_Attack_DoubleJumpWave* pInstance = new CValtan_BT_Attack_DoubleJumpWave;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_DoubleJumpWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_DoubleJumpWave::Free()
{
	__super::Free();
}
