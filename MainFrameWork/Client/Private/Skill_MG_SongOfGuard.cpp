#include "stdafx.h"
#include "..\Public\Skill_MG_SongOfGuard.h"
#include "Player_Bard.h"
#include "Projectile.h"

CSkill_MG_SongOfGuard::CSkill_MG_SongOfGuard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Bard* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_MG_SongOfGuard"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_MG_SongOfGuard::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_MG_SongOfGuard");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.2f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 1;
	Proj_Desc.iStagger = 0;
	Proj_Desc.fRepulsion = 0.f;
	Proj_Desc.bUseProjPos = true;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;


	return S_OK;
}

void CSkill_MG_SongOfGuard::Tick(_float fTimeDelta)
{
}

void CSkill_MG_SongOfGuard::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_MG_SongOfGuard::Render()
{
	return S_OK;
}

HRESULT CSkill_MG_SongOfGuard::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Guardian_Tune"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_MG_SongOfGuard::Check_ColliderState()
{
}

CSkill_MG_SongOfGuard* CSkill_MG_SongOfGuard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Bard* pPlayer, void* pArg)
{
	CSkill_MG_SongOfGuard* pInstance = new CSkill_MG_SongOfGuard(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_MG_SongOfGuard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_MG_SongOfGuard::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_MG_SongOfGuard::Free()
{
	__super::Free();
}

