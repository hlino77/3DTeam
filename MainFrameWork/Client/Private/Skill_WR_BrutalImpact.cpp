#include "stdafx.h"
#include "..\Public\Skill_WR_BrutalImpact.h"
#include "Player_Slayer.h"
#include "Projectile.h"

CSkill_WR_BrutalImpact::CSkill_WR_BrutalImpact(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WR_BrutalImpact"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WR_BrutalImpact::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WR_BrutalImpact");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;
	m_eCtrlType = SKILL_CTRLTYPE::HOLD;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 2.f);
	Proj_Desc.vChildScale = Vec3(1.f, 0.6f, 1.8f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 2.f);
	Proj_Desc.iDamage = 200.f;
	Proj_Desc.fRepulsion = 25.f;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

HRESULT CSkill_WR_BrutalImpact::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Brutal_Impact"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WR_BrutalImpact::Check_ColliderState()
{
	if (m_IsSkillSuccess)
	{
		m_SkillProjDesc.iDamage = 400.f;
		m_SkillProjDesc.fRepulsion = 25.f;
	}
	else
	{
		m_SkillProjDesc.iDamage = 200.f;
		m_SkillProjDesc.fRepulsion = 5.f;
	}
}

CSkill_WR_BrutalImpact* CSkill_WR_BrutalImpact::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg)
{
	CSkill_WR_BrutalImpact* pInstance = new CSkill_WR_BrutalImpact(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WR_BrutalImpact");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WR_BrutalImpact::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WR_BrutalImpact::Free()
{
	__super::Free();
}

