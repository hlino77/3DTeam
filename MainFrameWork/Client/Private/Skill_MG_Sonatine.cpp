#include "stdafx.h"
#include "..\Public\Skill_MG_Sonatine.h"
#include "Player_Bard.h"
#include "Projectile.h"

CSkill_MG_Sonatine::CSkill_MG_Sonatine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Bard* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_MG_Sonatine"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_MG_Sonatine::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_MG_Sonatine");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.2f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 2.0f);
	Proj_Desc.vChildScale = Vec3(1.2f, 0.6f, 1.9f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 2.0f);
	Proj_Desc.iDamage = 150;
	Proj_Desc.iStagger = 4;
	Proj_Desc.fRepulsion = 12.f;

	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

void CSkill_MG_Sonatine::Tick(_float fTimeDelta)
{
}

void CSkill_MG_Sonatine::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_MG_Sonatine::Render()
{
	return S_OK;
}

HRESULT CSkill_MG_Sonatine::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Sonatina"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_MG_Sonatine::Check_ColliderState()
{
}

CSkill_MG_Sonatine* CSkill_MG_Sonatine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Bard* pPlayer, void* pArg)
{
	CSkill_MG_Sonatine* pInstance = new CSkill_MG_Sonatine(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_MG_Sonatine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_MG_Sonatine::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_MG_Sonatine::Free()
{
	__super::Free();
}

