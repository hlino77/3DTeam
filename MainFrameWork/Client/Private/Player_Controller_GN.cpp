#include "stdafx.h"
#include "..\Public\Player_Controller_GN.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Player_Gunslinger.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Projectile.h"
#include "Pool.h"
#include "Player.h"
#include "Esther.h"

CPlayer_Controller_GN::CPlayer_Controller_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CPlayer_Controller_GN::CPlayer_Controller_GN(const CPlayer_Controller_GN& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CPlayer_Controller_GN::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Initialize(void* pArg)
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
	Proj_Desc.iDamage = 100;
	Proj_Desc.fRepulsion = 1.5f;
	m_HandAttackDesc = Proj_Desc;

	Proj_Desc.fRadius = 1.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 1.7f);
	Proj_Desc.vChildScale = Vec3(0.6f, 0.6f, 1.0f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 1.2f);
	Proj_Desc.iDamage = 150;
	Proj_Desc.fRepulsion = 2.0f;
	m_ShotAttackDesc = Proj_Desc;

	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 2.4f);
	Proj_Desc.vChildScale = Vec3(0.3f, 0.6f, 2.2f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 2.4f);
	Proj_Desc.iDamage = 120;
	Proj_Desc.fRepulsion = 1.8f;
	m_LongAttackDesc = Proj_Desc;

	return S_OK;
}

void CPlayer_Controller_GN::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	for (auto& pSkill : m_pHandSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->Tick(fTimeDelta);
	}
	for (auto& pSkill : m_pShotSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->Tick(fTimeDelta);
	}
	for (auto& pSkill : m_pLongSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->Tick(fTimeDelta);
	}

}

void CPlayer_Controller_GN::LateTick(_float fTimeDelta)
{
	for (auto& pSkill : m_pHandSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->LateTick(fTimeDelta);
	}
	for (auto& pSkill : m_pShotSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->LateTick(fTimeDelta);
	}
	for (auto& pSkill : m_pLongSkills)
	{
		if (nullptr == pSkill || false == pSkill->Is_Active()) continue;

		pSkill->LateTick(fTimeDelta);
	}

	__super::LateTick(fTimeDelta);
}

void CPlayer_Controller_GN::DebugRender()
{
}

_bool CPlayer_Controller_GN::Is_EstherSkill()
{
	if (false == static_cast<CPlayer*>(m_pOwner)->Is_PartyLeader())
		return false;

	if (static_cast<CPlayer*>(m_pOwner)->Get_EstherGage() < static_cast<CPlayer*>(m_pOwner)->Get_EstherMaxGage())
		return false;

	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::Z))
	{
		m_iEstherType = 0;
		return true;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::X))
	{
		m_iEstherType = 1;
		return true;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::C))
	{
		m_iEstherType = 2;
		return true;
	}

	return false;
}

_bool CPlayer_Controller_GN::Is_Idle()
{
	if (false == __super::Is_Idle())
		return false;

	if (0 != Is_GN_Identity())
		return false;

	return true;
}

_uint CPlayer_Controller_GN::Is_GN_Identity()
{
	if (false == m_bKeyActive || false == m_bCtrlActive)
		return 0;

	if (KEY_TAP(KEY::Z))
	{
		return 2;
	}
	else if (KEY_TAP(KEY::X))
	{
		return 1;
	}

	return 0;
}

void CPlayer_Controller_GN::Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos)
{
	if (true == m_IsDead)
		return;

	__super::Get_HitMessage(iDamge, fForce, vPos);

	// 데미지하락 및 밉라이트?

	m_iCalculateDamaged = (CGameInstance::GetInstance()->Random_Int(m_iDamaged, _int((_float)m_iDamaged * 1.2f))) * 100;
	CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
	tPcStat.iCurHp -= m_iCalculateDamaged;
	if (m_pOwner->Is_Control())
		static_cast<CPlayer*>(m_pOwner)->Show_Damage(m_iCalculateDamaged, false);
	if (0 >= tPcStat.iCurHp)
	{
		tPcStat.iCurHp = 0;
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Dead_Start"));
		static_cast<CPlayer*>(m_pOwner)->Send_Hp();
		return;
	}
	else
	{
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		static_cast<CPlayer*>(m_pOwner)->Send_Hp();
	}
	

	if (true == m_bBuffEffect[(_uint)BUFFEFFECT::STIIFIMMUNE])
	{
		m_eHitType = HIT_TYPE::TYPE_END;
		return;
	}

	if (HIT_TYPE::WEAK != m_eHitType && false == static_cast<CPlayer*>(m_pOwner)->Is_SuperiorArmor() ||
		HIT_TYPE::WEAK != m_eHitType && true == static_cast<CPlayer*>(m_pOwner)->Is_SafeZonePierce())
	{
		static_cast<CPlayer*>(m_pOwner)->Off_SafeZonePierce();

		if (HIT_TYPE::DMG == m_eHitType && false == static_cast<CPlayer*>(m_pOwner)->Is_SuperArmor())
		{
			m_IsHitState = true;

			static_cast<CPlayer*>(m_pOwner)->Set_TargetPos(Vec3(m_vHitColiPos.x, m_fForced, m_vHitColiPos.z));
			static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Hit_Common"));
		}
		else if(HIT_TYPE::DMG != m_eHitType)
		{
			m_IsHitState = true;

			_float fCheckHit = 0.0f;
			switch (m_eHitType)
			{
			case Engine::CPlayer_Controller::DOWN:
				fCheckHit = m_fForced;
				break;
			case Engine::CPlayer_Controller::KNOCKDOWN:
				fCheckHit = 10.f + m_fForced;
				break;
			case Engine::CPlayer_Controller::BOUND:
				fCheckHit = 20.f + m_fForced;
				break;
			case Engine::CPlayer_Controller::TWIST:
				fCheckHit = 30.f + m_fForced;
				break;
			}
			static_cast<CPlayer*>(m_pOwner)->Set_TargetPos(Vec3(m_vHitColiPos.x, fCheckHit, m_vHitColiPos.z));
			static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Hit"));
		}
	}
}

void CPlayer_Controller_GN::Get_GrabMessage(CGameObject* pGrabber)
{
	__super::Get_GrabMessage(pGrabber);

	static_cast<CPlayer*>(m_pOwner)->Set_State(TEXT("Grabbed"));

	static_cast<CPlayer*>(m_pOwner)->Set_SuperArmorState(false);
	static_cast<CPlayer*>(m_pOwner)->Set_SuperiorArmorState(false);
	static_cast<CPlayer*>(m_pOwner)->Set_Invincible(false);

	m_bBuffEffect[(_uint)BUFFEFFECT::STIIFIMMUNE] = false;
	m_fBuffDuration[(_uint)BUFFEFFECT::STIIFIMMUNE] = -1;
}

void CPlayer_Controller_GN::Get_GrabEndMessage()
{
	__super::Get_GrabEndMessage();
}

void CPlayer_Controller_GN::Get_EshterGageUseMessage()
{
	static_cast<CPlayer*>(m_pOwner)->Set_EstherGage(0);
	static_cast<CPlayer*>(m_pOwner)->Send_EstherGauge();
}

void CPlayer_Controller_GN::Skill(GN_IDENTITY eIndex, SKILL_KEY eKey)
{
	__super::Skill(eKey);

	m_fGN_CoolTime[eIndex][eKey] = m_pSkills[eKey]->Get_Skill_CoolTime();
}

HRESULT CPlayer_Controller_GN::Bind_HandSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	/*if (nullptr == pSkill)
		return E_FAIL;*/

	if (SKILL_KEY::SPACE == eKey)
		return S_OK;

	m_pHandSkills[eKey] = pSkill;

	if(nullptr != pSkill)
		pSkill->Set_BindKey(eKey);

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Bind_ShotSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	/*if (nullptr == pSkill)
		return E_FAIL;*/

	if (SKILL_KEY::SPACE == eKey || SKILL_KEY::A == eKey || SKILL_KEY::S == eKey || SKILL_KEY::D == eKey || SKILL_KEY::F == eKey)
		return S_OK;

	m_pShotSkills[eKey] = pSkill;

	if (nullptr != pSkill)
		pSkill->Set_BindKey(eKey);

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Bind_LongSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	/*if (nullptr == pSkill)
		return E_FAIL;*/

	if (SKILL_KEY::SPACE == eKey || SKILL_KEY::Q == eKey || SKILL_KEY::W == eKey || SKILL_KEY::E == eKey || SKILL_KEY::R == eKey)
		return S_OK;

	m_pLongSkills[eKey] = pSkill;

	if (nullptr != pSkill)
		pSkill->Set_BindKey(eKey);

	return S_OK;
}

CPlayer_Skill* CPlayer_Controller_GN::Get_PlayerSkill_Weapon(GN_IDENTITY eStance, SKILL_KEY eKey)
{
	switch (eStance)
	{
	case GN_IDENTITY::HAND:
		if(nullptr != m_pHandSkills[eKey])
			return m_pHandSkills[eKey];
		break;

	case GN_IDENTITY::SHOT:
		if (nullptr != m_pShotSkills[eKey])
			return m_pShotSkills[eKey];
		break;

	case GN_IDENTITY::LONG:
		if (nullptr != m_pLongSkills[eKey])
			return m_pLongSkills[eKey];
		break;
	}
	return nullptr;
}

void CPlayer_Controller_GN::Input(const _float& fTimeDelta)
{

}

void CPlayer_Controller_GN::Attack(Vec3 vPos)
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();

	if (Vec3() != vPos)
	{
		m_AttackDesc.vAttackPos = vPos;
	}
	else
	{
		m_AttackDesc.AttackMatrix = m_pOwner->Get_TransformCom()->Get_WorldMatrix();
	}

	pAttack->InitProjectile(&m_AttackDesc);
}

void CPlayer_Controller_GN::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
{
	if (nullptr == m_pSkills[eKey])
		return;

	CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
	if (Vec3() != vPos)
	{
		m_pSkills[eKey]->Set_SkillProjPos(vPos);
	}
	else
	{
		m_pSkills[eKey]->Set_SkillProjMat(m_pOwner->Get_TransformCom()->Get_WorldMatrix());
	}
	pSkill->InitProjectile(&m_pSkills[eKey]->Get_Skill_Proj_Desc());
}

void CPlayer_Controller_GN::Skill_CoolTime(const _float& fTimeDelta)
{
	__super::Skill_CoolTime(fTimeDelta);

	for (size_t i = 0; i < GN_IDENTITY::_END; i++)
	{
		for (size_t j = 0; j < SKILL_KEY::_END; j++)
		{
			if (-1.f == m_fGN_CoolTime[i][j]) continue;

			m_fGN_CoolDownAcc[i][j] += fTimeDelta;

			if (m_fGN_CoolTime[i][j] <= m_fGN_CoolDownAcc[i][j])
			{
				m_fGN_CoolDownAcc[i][j] = 0.f;
				m_fGN_CoolTime[i][j] = -1.f;
			}
		}
	}
}

void CPlayer_Controller_GN::GN_Identity(GN_IDENTITY eIndex)
{
	m_ePreIdentity = m_eIdentity;
	m_eIdentity = eIndex;
	Change_Skill_Iden(eIndex);
	Change_Iden_Collider(eIndex);
}

void CPlayer_Controller_GN::Change_Iden_Collider(GN_IDENTITY eIndex)
{
	switch (eIndex)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_AttackDesc = m_HandAttackDesc;
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_AttackDesc = m_ShotAttackDesc;
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_AttackDesc = m_LongAttackDesc;
		break;
	}
}

void CPlayer_Controller_GN::Esther_Refill(_float fTimeDelta)
{
	if (false == static_cast<CPlayer*>(m_pOwner)->Is_PartyLeader() ||
		m_pOwner->Get_CurrLevel() != LEVELID::LEVEL_VALTANMAIN)
		return;

	m_fEstherAcc += fTimeDelta;
	if (m_fEstherAcc >= m_fEstherFillTime)
	{
		m_fEstherAcc = 0.0f;

		_uint iCurGage = static_cast<CPlayer*>(m_pOwner)->Get_EstherGage();
		_uint iMaxGage = static_cast<CPlayer*>(m_pOwner)->Get_EstherMaxGage();

		iCurGage += m_iEstherFill;
		if (iCurGage >= iMaxGage)
		{
			iCurGage = iMaxGage;
		}
		static_cast<CPlayer*>(m_pOwner)->Set_EstherGage(iCurGage);
		static_cast<CPlayer*>(m_pOwner)->Send_EstherGauge();
	}
}

void CPlayer_Controller_GN::EstherSkill(_uint iIndex)
{
	for (auto& pEsther : m_vecEsther)
	{
		if (static_cast<CEsther*>(pEsther)->Get_EstherType() == iIndex)
		{
			if (false == pEsther->Is_Active())
				static_cast<CEsther*>(pEsther)->Leader_Active_Esther();
		}
	}
}

void CPlayer_Controller_GN::Change_Skill_Iden(GN_IDENTITY eIndex)
{
	if (GN_IDENTITY::HAND == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pHandSkills[i];
		
			m_fCoolDownAcc[i] = m_fGN_CoolDownAcc[GN_IDENTITY::HAND][i];
			m_fCoolTime[i] = m_fGN_CoolTime[GN_IDENTITY::HAND][i];
		}
	}
	else if (GN_IDENTITY::LONG == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pLongSkills[i];

			m_fCoolDownAcc[i] = m_fGN_CoolDownAcc[GN_IDENTITY::LONG][i];
			m_fCoolTime[i] = m_fGN_CoolTime[GN_IDENTITY::LONG][i];
		}
	}
	else if (GN_IDENTITY::SHOT == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pShotSkills[i];

			m_fCoolDownAcc[i] = m_fGN_CoolDownAcc[GN_IDENTITY::SHOT][i];
			m_fCoolTime[i] = m_fGN_CoolTime[GN_IDENTITY::SHOT][i];
		}
	}
}

CPlayer_Controller_GN* CPlayer_Controller_GN::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPlayer_Controller_GN::Clone(CGameObject* pGameObject, void* pArg)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Controller_GN::Free()
{
	__super::Free();
}
