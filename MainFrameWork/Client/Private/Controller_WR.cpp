#include "stdafx.h"
#include "..\Public\Controller_WR.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Gunslinger.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"

CController_WR::CController_WR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CController_WR::CController_WR(const CController_WR& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CController_WR::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CController_WR::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	PROJECTILE_DESC Proj_Desc;

	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;

	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.7f);
	Proj_Desc.vChildScale = Vec3(0.35f, 0.6f, 1.5f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.7f);
	m_AttackDesc = Proj_Desc;

	return S_OK;
}

void CController_WR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CController_WR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CController_WR::DebugRender()
{
}

void CController_WR::Input(const _float& fTimeDelta)
{

}

void CController_WR::Attack()
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();
	pAttack->InitProjectile(&m_AttackDesc);
}

void CController_WR::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
{
	if (nullptr == m_pSkills[eKey])
		return;

	CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
	if (Vec3() != vPos)
	{
		m_pSkills[eKey]->Set_SkillProjPos(vPos);
	}
	pSkill->InitProjectile(&m_pSkills[eKey]->Get_Skill_Proj_Desc());
}

void CController_WR::Hit(CGameObject* pHitObject)
{
	if (HIT_TYPE::TYPE_END == m_eHitType || nullptr == pHitObject)
		return;
}

void CController_WR::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);
}

CController_WR* CController_WR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CController_WR* pInstance = new CController_WR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CController_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CController_WR::Clone(CGameObject* pGameObject, void* pArg)
{
	CController_WR* pInstance = new CController_WR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CController_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CController_WR::Free()
{
	__super::Free();
}