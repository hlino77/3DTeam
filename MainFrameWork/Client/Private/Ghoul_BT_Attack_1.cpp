#include "stdafx.h"
#include "Ghoul_BT_Attack_1.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

CGhoul_BT_Attack_1::CGhoul_BT_Attack_1()
{
}

void CGhoul_BT_Attack_1::OnStart()
{
	__super::OnStart(0);
	//Add_Sound(L"Ghoul", L"WWISEDEFAULTBANK_S_MONSTER_GLOBAL_UNDEAD1#1 (13899944)");
}

CBT_Node::BT_RETURN CGhoul_BT_Attack_1::OnUpdate(const _float& fTimeDelta)
{
	if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive()&& m_pGameObject->Get_ModelCom()->Get_CurrAnim()==m_vecAnimDesc[0].iAnimIndex&& m_pGameObject->Get_ModelCom()->IsNext())
	{
		CSphereCollider* pCollider = m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER);
		pCollider->Set_Radius(0.65f);
		pCollider->SetActive(true);
		pCollider->Set_Offset(Vec3(0.0f, 0.95f, 0.8f));;
		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(Vec3(0.1f, 0.1f, 0.6f));
		pChildCollider->Set_Offset(Vec3(0.0f, 0.95f, 0.8f));
	}
	if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim()== m_vecAnimDesc[2].iAnimIndex&&  8 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex))
	{
		CSphereCollider* pCollider = m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER);
		pCollider->Set_Radius(0.65f);
		pCollider->SetActive(false);
		pCollider->Set_Offset(Vec3(0.0f, 0.95f, 0.8f));
		pCollider->Get_Child()->SetActive(false);
	}	


	Add_Sound(0, 0, L"Effect", L"Ghoul_Attack1_Start",15, 0.2f);
	Add_Sound(2, 1, L"Effect", L"Ghoul_Attack1_End", 0, 0.2f);

	return  __super::OnUpdate(fTimeDelta);
}

void CGhoul_BT_Attack_1::OnEnd()
{
	__super::OnEnd();
	CSphereCollider* pCollider = m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER);
	pCollider->Set_Radius(0.65f);
	pCollider->SetActive(false);
	pCollider->Set_Offset(Vec3(0.0f, 0.95f, 0.8f));
	pCollider->Get_Child()->SetActive(false);
}



CGhoul_BT_Attack_1* CGhoul_BT_Attack_1::Create(void* pArg)
{
	CGhoul_BT_Attack_1* pInstance = new CGhoul_BT_Attack_1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_1::Free()
{
	__super::Free();
}
