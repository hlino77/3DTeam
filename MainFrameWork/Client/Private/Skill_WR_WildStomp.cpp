#include "stdafx.h"
#include "..\Public\Skill_WR_WildStomp.h"
#include "Player_Slayer.h"
#include "Projectile.h"
#include "Item.h"

CSkill_WR_WildStomp::CSkill_WR_WildStomp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WR_WildStomp"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WR_WildStomp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WR_WildStomp");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::COUNTER;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = false;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;

	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.3f);
	Proj_Desc.iDamage = 200;
	Proj_Desc.iStagger = 10;
	Proj_Desc.iStatusEffect = (_uint)STATUSEFFECT::EARTHQUAKE;
	Proj_Desc.fStatusDuration = 3.f;
	Proj_Desc.IsCounter =true;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	m_iUseMana = 67;

	return S_OK;
}

void CSkill_WR_WildStomp::Tick(_float fTimeDelta)
{
}

void CSkill_WR_WildStomp::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_WR_WildStomp::Render()
{
	return S_OK;
}

HRESULT CSkill_WR_WildStomp::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Wild_Stomp"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CSkill_WR_WildStomp::Check_ColliderState()
{
	if (nullptr != static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON))
	{
		_uint iItemLevel = static_cast<CPlayer*>(m_pOwner)->Get_EquipItem((_uint)CPlayer::PART::WEAPON)->Get_UpgradeLevel();
		if (iItemLevel <= 10)
		{
			_uint iDamage = m_vecSkillProjDesces[0].iDamage;
			m_SkillProjDesc.iDamage = iDamage * m_iDefaultItem;
		}
		else
		{
			_uint iDamage = m_vecSkillProjDesces[0].iDamage;
			m_SkillProjDesc.iDamage = iDamage * m_iUpgradedItem;
		}
	}
}

CSkill_WR_WildStomp* CSkill_WR_WildStomp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Slayer* pPlayer, void* pArg)
{
	CSkill_WR_WildStomp* pInstance = new CSkill_WR_WildStomp(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WR_WildStomp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WR_WildStomp::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WR_WildStomp::Free()
{
	__super::Free();
}

