#include "stdafx.h"
#include "..\Public\Skill_SP_Inkshot.h"
#include "Player_Doaga.h"
#include "Projectile.h"

CSkill_SP_Inkshot::CSkill_SP_Inkshot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_SP_Inkshot"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_SP_Inkshot::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* ��Ѹ��� */
	m_strSkill_Name = TEXT("Skill_SP_Inkshot");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.6f);
	Proj_Desc.vChildScale = Vec3(1.4f, 0.6f, 1.4f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.6f);
	Proj_Desc.iDamage = 200;
	Proj_Desc.iStagger = 60;
	Proj_Desc.fRepulsion = 10.f;

	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	return S_OK;
}

void CSkill_SP_Inkshot::Tick(_float fTimeDelta)
{
}

void CSkill_SP_Inkshot::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_SP_Inkshot::Render()
{
	return S_OK;
}

HRESULT CSkill_SP_Inkshot::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Yinyanshi_Sprinkle"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SP_Inkshot::Check_ColliderState()
{
}

CSkill_SP_Inkshot* CSkill_SP_Inkshot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer, void* pArg)
{
	CSkill_SP_Inkshot* pInstance = new CSkill_SP_Inkshot(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_SP_Inkshot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_SP_Inkshot::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_SP_Inkshot::Free()
{
	__super::Free();
}
