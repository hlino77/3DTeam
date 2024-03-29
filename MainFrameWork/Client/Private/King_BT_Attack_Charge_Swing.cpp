#include "stdafx.h"
#include "King_BT_Attack_Charge_Swing.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
#include "Skill.h"
#include "GameInstance.h"
#include <Boss.h>
#include <Effect_Manager.h>

CKing_BT_Attack_Charge_Swing::CKing_BT_Attack_Charge_Swing()
{
}

void CKing_BT_Attack_Charge_Swing::OnStart()
{
	__super::OnStart(0);
	m_Shoot = true;
}

CBT_Node::BT_RETURN CKing_BT_Attack_Charge_Swing::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) == 11&& m_Shoot)
	{
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"KING_Getsuga_Tenshou", &tDesc);
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_King_ChargeSwing", &ModelDesc);
		if (pSkill != nullptr)
		{
			Add_Sound(L"Effect", L"KING_56");
			Add_Sound(L"Effect", L"KING_48");
			Add_Sound(L"Effect", L"KING_77");
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 0.1f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			m_Shoot = false;
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (!static_cast<CBoss*>(m_pGameObject)->Is_GroggyLock() && m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyLock(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyLock(false);
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	}
	Add_Sound(0, 0, L"Effect", L"KING_69");
	Add_Sound(1, 1, L"Effect", L"KING_77");
	return  __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Charge_Swing::OnEnd()
{
	__super::OnEnd();		
	static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	static_cast<CBoss*>(m_pGameObject)->Set_GroggyLock(false);
}



CKing_BT_Attack_Charge_Swing* CKing_BT_Attack_Charge_Swing::Create(void* pArg)
{
	CKing_BT_Attack_Charge_Swing* pInstance = new CKing_BT_Attack_Charge_Swing;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Charge_Swing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Charge_Swing::Free()
{
	__super::Free();
}
