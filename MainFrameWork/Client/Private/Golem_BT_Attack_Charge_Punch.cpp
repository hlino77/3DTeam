#include "stdafx.h"
#include "Golem_BT_Attack_Charge_Punch.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "GameInstance.h"
#include "Boss_Golem.h"
#include <ColliderOBB.h>
#include <Skill.h>

CGolem_BT_Attack_Charge_Punch::CGolem_BT_Attack_Charge_Punch()
{
}

void CGolem_BT_Attack_Charge_Punch::OnStart()
{
	__super::OnStart(0);
	
	m_Shoot = true;

	m_iMaxSound = 3;
	for (size_t i = 0; i < m_iMaxSound; i++)
	{
		m_bSoundOn[i] = false;
	}
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Charge_Punch::OnUpdate(const _float& fTimeDelta)
{
	Add_Sound(0, 0, TEXT("Effect"), TEXT("Rook_64"), 0, 0.8f);
	Add_Sound_Channel(2, 1, TEXT("Effect"), TEXT("Rook_95"), 0, 0.8f);
	Add_Sound(3, 2, TEXT("Effect"), TEXT("Rook_100"), 8, 1.f);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[0].iAnimIndex)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
 	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && 10 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex)&& m_Shoot)
	{
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;


		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Golem_Charge_Punch", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 1.2f;
			vPos.y = 1.f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			m_Shoot = false;
		}
	}
	if (!static_cast<CBoss*>(m_pGameObject)->Is_GroggyLock() && m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyLock(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyLock(false);
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	}

	return  __super::OnUpdate(fTimeDelta);

}

void CGolem_BT_Attack_Charge_Punch::OnEnd()
{
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	__super::OnEnd();
	static_cast<CBoss*>(m_pGameObject)->Set_GroggyLock(false);
	static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);

	CSound_Manager::GetInstance()->Stop_Channel_Sound(TEXT("Rook_95"));
}



CGolem_BT_Attack_Charge_Punch* CGolem_BT_Attack_Charge_Punch::Create(void* pArg)
{
	CGolem_BT_Attack_Charge_Punch* pInstance = new CGolem_BT_Attack_Charge_Punch;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Charge_Punch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Charge_Punch::Free()
{
	__super::Free();
}
