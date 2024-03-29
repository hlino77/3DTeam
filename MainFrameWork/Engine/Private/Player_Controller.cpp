#include "Player_Controller.h"
#include "GameInstance.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Player_Skill.h"

CPlayer_Controller::CPlayer_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CPlayer_Controller::CPlayer_Controller(const CPlayer_Controller& rhs)
	: CComponent(rhs)
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		m_fCoolDownAcc[i] = rhs.m_fCoolDownAcc[i];
		m_fCoolTime[i] = rhs.m_fCoolTime[i];
		m_fChangeStatAcc[i] = rhs.m_fChangeStatAcc[i];
		m_fChangeStatTime[i] = rhs.m_fChangeStatTime[i];
	}

	for (size_t i = 0; i < (_uint)STATUSEFFECT::_END; i++)
	{
		m_bStatusEffect[i] = rhs.m_bStatusEffect[i];
		m_fStatusDuration[i] = rhs.m_fStatusDuration[i];
	}

	for (size_t i = 0; i < (_uint)BUFFEFFECT::_END; i++)
	{
		m_bBuffEffect[i] = rhs.m_bBuffEffect[i];
		m_fBuffDuration[i] = rhs.m_fBuffDuration[i];
		m_fBuffAmount[i] = rhs.m_fBuffAmount[i];
	}
}

HRESULT CPlayer_Controller::Initialize_Prototype()
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		m_fCoolDownAcc[i] = 0.f;
		m_fCoolTime[i] = -1.f;

		m_fChangeStatAcc[i] = 0.f;
		m_fChangeStatTime[i] = -1.f;
	}

	for (size_t i = 0; i < (_uint)STATUSEFFECT::_END; i++)
	{
		m_bStatusEffect[i] = false;
		m_fStatusDuration[i] = -1.f;
	}

	for (size_t i = 0; i < (_uint)BUFFEFFECT::_END; i++)
	{
		m_bBuffEffect[i] = false;
		m_fBuffDuration[i] = -1.f;
		m_fBuffAmount[i] = 0.0f;
	}

	return S_OK;
}

HRESULT CPlayer_Controller::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		CONTROLL_DESC* pCtrlDesc = (CONTROLL_DESC*)pArg;
		m_pOwner = pCtrlDesc->pOwner;

		m_pOwnerTransform = pCtrlDesc->pOwnerTransform;

		m_pOwnerRigidBody = pCtrlDesc->pOwnerRigidBody;
	}

	return S_OK;
}

void CPlayer_Controller::Tick(_float fTimeDelta)
{
	if (false == m_bStop)
		Move(fTimeDelta);
	if (true == m_bStop)
		Look_Lerp(fTimeDelta);

	/* Refill */
	Refill_Mana(fTimeDelta);
	Esther_Refill(fTimeDelta);

	/* CoolTime */
	Skill_CoolTime(fTimeDelta);
	Skill_ChangeStat_CoolTime(fTimeDelta);
	StatusEffect_Duration(fTimeDelta);
	BuffEffect_Duration(fTimeDelta);

	/* Skill Collider */
	Skill_Check_Collider();
}

void CPlayer_Controller::LateTick(_float fTimeDelta)
{
	m_vPrePos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
}

void CPlayer_Controller::DebugRender()
{
}

_bool CPlayer_Controller::Is_Idle()
{
	if (false == Is_Run() && false == Is_Skill() && false == Is_Attack() && false == Is_Dash())
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Tap(KEY eKey)
{
	if (false == m_bCtrlActive)
		return false;

	if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && true == m_bMouseActive)
	{
		return KEY_TAP(eKey);
	}
	else if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && false == m_bMouseActive)
	{
		return false;
	}

	if (false == m_bKeyActive)
	{
		return false;
	}


	return KEY_TAP(eKey);
}

_bool CPlayer_Controller::Is_Hold(KEY eKey)
{
	if (false == m_bCtrlActive)
		return false;

	if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && true == m_bMouseActive)
	{
		return KEY_HOLD(eKey);
	}
	else if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && false == m_bMouseActive)
	{
		return false;
	}

	if (false == m_bKeyActive)
	{
		return false;
	}

	return KEY_HOLD(eKey);
}

_bool CPlayer_Controller::Is_HoldorTap(KEY eKey)
{
	if (false == m_bCtrlActive)
		return false;

	if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && true == m_bMouseActive)
	{
		return KEY_HOLD(eKey) || KEY_TAP(eKey);
	}
	else if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && false == m_bMouseActive)
	{
		return false;
	}

	if (false == m_bKeyActive)
	{
		return false;
	}

	return KEY_HOLD(eKey) || KEY_TAP(eKey);
}

_bool CPlayer_Controller::Is_Away(KEY eKey)
{
	if (false == m_bCtrlActive)
		return false;

	if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && true == m_bMouseActive)
	{
		return KEY_AWAY(eKey) || KEY_NONE(eKey);
	}
	else if ((KEY::LBTN == eKey || KEY::RBTN == eKey) && false == m_bMouseActive)
	{
		return false;
	}

	if (false == m_bKeyActive)
	{
		return false;
	}

	return KEY_AWAY(eKey) || KEY_NONE(eKey);
}

_bool CPlayer_Controller::Is_Run()
{
	if (false == m_bMouseActive || false == m_bCtrlActive)
		return false;

	if (KEY_HOLD(KEY::CTRL))
		return false;

	if (KEY_HOLD(KEY::RBTN) || KEY_TAP(KEY::RBTN))
	{
		return true;
	}
		
	return false;
}

_bool CPlayer_Controller::Is_Skill()
{
	if (false == m_bSkillKeyActive || false == m_bKeyActive || false == m_bCtrlActive)
		return false;

	if (-1.f == m_fCoolTime[SKILL_KEY::Q] && nullptr != m_pSkills[SKILL_KEY::Q] && (KEY_HOLD(KEY::Q) || KEY_TAP(KEY::Q)))
	{
		m_eSelectedSkill = SKILL_KEY::Q;

		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::W] && nullptr != m_pSkills[SKILL_KEY::W] && (KEY_HOLD(KEY::W) || KEY_TAP(KEY::W)))
	{
		m_eSelectedSkill = SKILL_KEY::W;
		
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::E] && nullptr != m_pSkills[SKILL_KEY::E] && (KEY_HOLD(KEY::E) || KEY_TAP(KEY::E)))
	{
		m_eSelectedSkill = SKILL_KEY::E;
		
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::R] && nullptr != m_pSkills[SKILL_KEY::R] && (KEY_HOLD(KEY::R) || KEY_TAP(KEY::R)))
	{
		m_eSelectedSkill = SKILL_KEY::R;
		
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::A] && nullptr != m_pSkills[SKILL_KEY::A] && (KEY_HOLD(KEY::A) || KEY_TAP(KEY::A)))
	{
		m_eSelectedSkill = SKILL_KEY::A;
		
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::S] && nullptr != m_pSkills[SKILL_KEY::S] && (KEY_HOLD(KEY::S) || KEY_TAP(KEY::S)))
	{
		m_eSelectedSkill = SKILL_KEY::S;
		
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::D] && nullptr != m_pSkills[SKILL_KEY::D] && (KEY_HOLD(KEY::D) || KEY_TAP(KEY::D)))
	{
		m_eSelectedSkill = SKILL_KEY::D;
		
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}
	else if (-1.f == m_fCoolTime[SKILL_KEY::F] && nullptr != m_pSkills[SKILL_KEY::F] && (KEY_HOLD(KEY::F) || KEY_TAP(KEY::F)))
	{
		m_eSelectedSkill = SKILL_KEY::F;
		
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp -= m_pSkills[m_eSelectedSkill]->Get_UseMana();
		if (0 > tPcStat.iCurMp)
		{
			return false;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
		m_fRefillManaAcc = -2.f;
		return true;
	}

	m_eSelectedSkill = SKILL_KEY::_END;
	return false;
}

_bool CPlayer_Controller::Is_Interect()
{
	if (false == m_bKeyActive || false == m_bCtrlActive)
		return false;

	if (KEY_TAP(KEY::G))
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Dash()
{
	if (false == m_bKeyActive || false == m_bCtrlActive)
		return false;

	if ( KEY_HOLD(KEY::SPACE) ||KEY_TAP(KEY::SPACE))
	{
		if (-1.f == m_fCoolTime[SKILL_KEY::SPACE])
			return true;
		else
			return false;
	}

	return false;
}

_bool CPlayer_Controller::Is_Attack()
{
	if (false == m_bMouseActive || false == m_bCtrlActive)
		return false;

	if (KEY_HOLD(KEY::LBTN) || KEY_TAP(KEY::LBTN))
		return true;

	return false;
}

void CPlayer_Controller::Get_SkillStartMessage(_uint iBindKey)
{
	if (nullptr == m_pSkills[iBindKey])
		return;

	m_pSkills[iBindKey]->Enter();
}

void CPlayer_Controller::Get_RootMessage()
{
	Vec3 vTargetPos = m_pOwner->Get_TargetPos();
	Vec3 vPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vDir = vTargetPos - vPos;
	_float fDist = vDir.Length();
	if (0.05f >= fDist)
	{
		m_pOwner->Get_ModelCom()->Set_RootDist(0.f);
	}
}

void CPlayer_Controller::Get_RootZeroMessage()
{
	m_pOwner->Get_ModelCom()->Set_RootDist(0.f);
	m_pOwnerTransform->Set_State(CTransform::STATE_POSITION, m_vPrePos);
}

void CPlayer_Controller::Get_GrabMessage(CGameObject* pGrab)
{
	m_pGrabber = pGrab;
	m_IsGrabState = true;
}

void CPlayer_Controller::Get_CheckLengthMessage(_float fCheckLength, CGameObject* pOther)
{
	Vec3 vSavePos;
	Vec3 vOwnerPos = vSavePos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vOwnerPos.y = 0.0f;

	Vec3 vOtherPos = pOther->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vOtherPos.y = 0.0f;

	Vec3 vDir = vOwnerPos - vOtherPos;
	_float fLength = vDir.Length();

	if (fLength <= fCheckLength)
	{
		vDir.Normalize();
		Vec3 vTargetPos = vOtherPos + vDir * fCheckLength;
		vSavePos = Vec3::Lerp(vSavePos, vTargetPos, 0.2f);

		Get_StopMessage();
		m_pOwner->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vSavePos);
	}
}

void CPlayer_Controller::Get_GrabEndMessage()
{
	m_pGrabber = nullptr;
	m_IsGrabState = false;
}

void CPlayer_Controller::Get_HitMessage(_uint iDamage, _float fForce, Vec3 vPos)
{
	m_iDamaged = iDamage;
	if (true == m_bBuffEffect[(_uint)BUFFEFFECT::HALFDAMAGE])
	{
		m_iDamaged *= m_fBuffAmount[(_uint)BUFFEFFECT::HALFDAMAGE];
		if (m_iDamaged >= 100)
			m_iDamaged = 100;
	}

	m_vHitColiPos = vPos;

	_float fCheckType = fabs(fForce);

	if (10.f <= fCheckType && 20.f > fCheckType)
	{
		m_eHitType = HIT_TYPE::DMG;

		if(fForce < 0)
			m_fForced = fForce + 10.f;
		else
			m_fForced = fForce - 10.f;
	}
	else if (20.f <= fCheckType && 30.f > fCheckType)
	{
		m_eHitType = HIT_TYPE::DOWN;
		
		if (fForce < 0)
			m_fForced = fForce + 20.f;
		else
			m_fForced = fForce - 20.f;
	}
	else if (30.f <= fCheckType && 40.f > fCheckType)
	{
		m_eHitType = HIT_TYPE::KNOCKDOWN;
		
		if (fForce < 0)
			m_fForced = fForce + 30.f;
		else
			m_fForced = fForce - 30.f;
	}
	else if (40.f <= fCheckType && 50.f > fCheckType)
	{
		m_eHitType = HIT_TYPE::BOUND;
	
		if (fForce < 0)
			m_fForced = fForce + 40.f;
		else
			m_fForced = fForce - 40.f;
	}
	else if (50.f <= fCheckType)
	{
		m_eHitType = HIT_TYPE::TWIST;

		if (fForce < 0)
			m_fForced = fForce + 50.f;
		else
			m_fForced = fForce - 50.f;
	}
	else
	{
		m_eHitType = HIT_TYPE::WEAK;
	}
}

void CPlayer_Controller::Get_MoveMessage(Vec3 vPos)
{
	if (Vec3() == vPos)
	{
		Get_StopMessage();
		return;
	}

	m_vNextMove = vPos; 
	m_bMoveStop = false;
	m_bStop = false; 
	m_IsDir = false; 
}

void CPlayer_Controller::Get_DirMessage(Vec3 vPos)
{
	if (Vec3() == vPos)
	{
		Get_StopMessage();
		return;
	}

	m_vNextMove = vPos; 
	m_bStop = false; 
	m_IsDir = true; 
}

void CPlayer_Controller::Get_LerpLookMessage(Vec3 vAt, _float fSpeed)
{
	if (Vec3() == vAt)
	{
		Get_StopMessage();
		return;
	}

	vAt.y = 0.f;

	m_vNextMove = vAt; 
	m_fLerpLook_Speed = fSpeed;
	m_bStop = true; 
	m_IsDir = false; 
}

void CPlayer_Controller::Get_LerpDirLookMessage(Vec3 vAt, _float fSpeed)
{
	if (Vec3() == vAt)
	{
		Get_StopMessage();
		return;
	}

	vAt.y = 0.f;

	Vec3 vPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vDir = vAt - vPos;
	vDir.Normalize();

	m_vNextMove = vDir;
	m_fLerpLook_Speed = fSpeed;
	m_bStop = true; 
	m_IsDir = true;
}

void CPlayer_Controller::Get_LookMessage(Vec3 vAt)
{
	if (Vec3() == vAt)
	{
		Get_StopMessage();
		return;
	}

	Vec3 vPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vPos.y = 0.0f;
	Vec3 vTarget = vAt;
	vTarget.y = 0.0f;

	_float fLength = Vec3(vTarget - vPos).Length();

	if(fLength >= FLT_EPSILON)
		Look(vAt);
}

void CPlayer_Controller::Get_DashMessage(Vec3 vPos)
{
	if (Vec3() == vPos)
	{
		Get_StopMessage();
		return;
	}

	Vec3 vPlayerPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vPlayerPos.y = 0.0f;
	Vec3 vTarget = vPos;
	vTarget.y = 0.0f;

	_float fLength = Vec3(vTarget - vPlayerPos).Length();

	if (fLength >= FLT_EPSILON)
		Look(vPos);
}

void CPlayer_Controller::Get_DashEndMessage(_float fCoolTime)
{
	m_fCoolTime[SKILL_KEY::SPACE] = fCoolTime;
}

void CPlayer_Controller::Get_StatusEffectMessage(_uint iStatus, _float fDurtaion)
{
	if (iStatus == (_uint)STATUSEFFECT::_END)
		return;

	/* 시작시 이미 이펙트상태면 초기화 */
	if (true == m_bStatusEffect[iStatus])
	{
		m_bStatusEffect[iStatus] = false;
		m_fStatusDuration[iStatus] = -1.f;
		switch (iStatus)
		{
		case (_uint)STATUSEFFECT::BUG:
			Bug();
			break;
		case (_uint)STATUSEFFECT::FEAR:
			Fear();
			break;
		case (_uint)STATUSEFFECT::EARTHQUAKE:
			EarthQuake();
			break;
		case (_uint)STATUSEFFECT::SHOCK:
			Shock();
			break;
		case (_uint)STATUSEFFECT::STUN:
			Stun();
			break;
		case (_uint)STATUSEFFECT::SILENCE:
			Silence();
			break;
		default:
			break;
		}
	}

	switch (iStatus)
	{
	case (_uint)STATUSEFFECT::BUG:
		m_bStatusEffect[iStatus] = true;
		m_fStatusDuration[iStatus] = fDurtaion;
		Bug();
		break;
	case (_uint)STATUSEFFECT::FEAR:
		m_bStatusEffect[iStatus] = true;
		m_fStatusDuration[iStatus] = fDurtaion;
		Fear();
		break;
	case (_uint)STATUSEFFECT::EARTHQUAKE:
		m_bStatusEffect[iStatus] = true;
		m_fStatusDuration[iStatus] = fDurtaion;
		EarthQuake();
		break;
	case (_uint)STATUSEFFECT::SHOCK:
		m_bStatusEffect[iStatus] = true;
		m_fStatusDuration[iStatus] = fDurtaion;
		Shock();
		break;
	case (_uint)STATUSEFFECT::STUN:
		m_bStatusEffect[iStatus] = true;
		m_fStatusDuration[iStatus] = fDurtaion;
		Stun();
		break;
	case (_uint)STATUSEFFECT::SILENCE:
		m_bStatusEffect[iStatus] = true;
		m_fStatusDuration[iStatus] = fDurtaion;
		Silence();
		break;
	default:
		break;
	}
}

HRESULT CPlayer_Controller::Bind_Skill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	m_pSkills[eKey] = pSkill;

	return S_OK;
}

_bool CPlayer_Controller::Pick(_uint screenX, _uint screenY, Vec3& pickPos, _float& distance)
{
	return true;
}

void CPlayer_Controller::Input(const _float& fTimeDelta)
{
}

void CPlayer_Controller::Move(const _float& fTimeDelta)
{
	Vec3 vNext = m_vNextMove;
	Vec3 vCur = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	vNext.y = 0.0f; vCur.y = 0.0f;

	if (Vec3(vNext - vCur).Length() <= 0.05f)
	{
		m_bMoveStop = true;
		return;
	}
	else
	{
		m_bMoveStop = false;
	}

	if (false == m_IsDir)
	{
		Vec3 vNextPos = m_vNextMove;
		vNextPos.y = 0.f;

		Vec3 vDir = vNextPos - vCur;
		m_pOwnerTransform->Move_ToPos(vDir, 12.f, m_fMoveSpeed, fTimeDelta);
	}
	else if (true == m_IsDir)
	{
		Vec3 vNextPos = m_vNextMove;
		vNextPos.y = 0.f;

		Vec3 vDir = vNextPos - vCur;
		m_pOwnerTransform->Move_Dir(vDir, fTimeDelta, m_fMoveSpeed);
	}
}

void CPlayer_Controller::Look_Lerp(const _float& fTimeDelta)
{
	if (Vec3() == m_vNextMove)
		return;

	if (false == m_IsDir)
	{
		Vec3 vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
		Vec3 vDir = m_vNextMove - vPos;
		m_pOwnerTransform->LookAt_Lerp_ForLand(vDir, m_fLerpLook_Speed, fTimeDelta);
	}
	else if (true == m_IsDir)
	{
		m_pOwnerTransform->LookAt_Lerp_ForLand(m_vNextMove, m_fLerpLook_Speed, fTimeDelta);
	}
}

void CPlayer_Controller::Look(Vec3 vAt)
{
	m_pOwnerTransform->LookAt_ForLandObject(vAt);
}

void CPlayer_Controller::Attack(Vec3 vPos)
{
}

HRESULT CPlayer_Controller::Set_SkillSuccess(SKILL_KEY eKey, _bool IsSuccess)
{
	if (nullptr == m_pSkills[eKey])
		return E_FAIL;

	m_pSkills[eKey]->Set_SkillSuccess(IsSuccess);
	return S_OK;
}

_bool CPlayer_Controller::Is_SkillSuccess(SKILL_KEY eKey)
{
	if (nullptr == m_pSkills[eKey])
		return false;

	return m_pSkills[eKey]->Is_SkillSuccess();
}

const wstring& CPlayer_Controller::Get_SkillStartName(SKILL_KEY eKey)
{
	return m_pSkills[eKey]->Get_Skill_StartName();
}

void CPlayer_Controller::Skill(SKILL_KEY eKey)
{
	m_fCoolTime[eKey] = m_pSkills[eKey]->Get_Skill_CoolTime();
}

void CPlayer_Controller::SkillAttack(SKILL_KEY eKey, Vec3 vPos)
{
}

void CPlayer_Controller::ChangeStat(SKILL_KEY eKey)
{
	if (nullptr == m_pSkills[eKey] || eKey == SKILL_KEY::_END)
		return;

	m_fChangeStatTime[eKey] = m_pSkills[eKey]->Change_Player_Status();
	m_fChangeStatAcc[eKey] = 0.f;
}

void CPlayer_Controller::Skill_CoolTime(const _float& fTimeDelta)
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		if (-1.f == m_fCoolTime[i]) continue;

		m_fCoolDownAcc[i] += fTimeDelta;

		if (m_fCoolTime[i] <= m_fCoolDownAcc[i])
		{
			m_fCoolDownAcc[i] = 0.f;
			m_fCoolTime[i] = -1.f;
		}
	}
}

void CPlayer_Controller::Skill_ChangeStat_CoolTime(const _float& fTimeDelta)
{
	for (size_t i = 0; i < SKILL_KEY::_END; i++)
	{
		if (-1.f == m_fChangeStatTime[i]) continue;

		m_fChangeStatAcc[i] += fTimeDelta;

		if (m_fChangeStatTime[i] <= m_fChangeStatAcc[i])
		{
			m_fChangeStatTime[i] = m_pSkills[i]->Restore_Player_Status();
			m_fChangeStatAcc[i] = 0.f;
		}
	}
}

void CPlayer_Controller::Skill_Check_Collider()
{
	for (auto& pSkill : m_pSkills)
	{
		if (nullptr == pSkill) continue;

		pSkill->Check_ColliderState();
	}
}

void CPlayer_Controller::StatusEffect_Duration(const _float& fTimeDelta)
{
	for (size_t i = 0; i < (_uint)STATUSEFFECT::_END; i++)
	{
		if (false == m_bStatusEffect[i]) continue;

		m_fStatusDuration[i] -= fTimeDelta;

		if (0.0f >= m_fStatusDuration[i])
		{
			m_bStatusEffect[i] = false;
			m_fStatusDuration[i] = -1.f;

			switch (i)
			{
			case (_uint)STATUSEFFECT::BUG:
				Bug();
				break;
			case (_uint)STATUSEFFECT::FEAR:
				Fear();
				break;
			case (_uint)STATUSEFFECT::EARTHQUAKE:
				EarthQuake();
				break;
			case (_uint)STATUSEFFECT::SHOCK:
				Shock();
				break;
			case (_uint)STATUSEFFECT::STUN:
				Stun();
				break;
			case (_uint)STATUSEFFECT::SILENCE:
				Silence();
				break;
			}
		}
	}
}

void CPlayer_Controller::Bug()
{
	if (-1.f == m_fStatusDuration[(_uint)STATUSEFFECT::BUG])
	{

	}
	else
	{

	}
}

void CPlayer_Controller::Fear()
{
	if (-1.f == m_fStatusDuration[(_uint)STATUSEFFECT::FEAR])
	{

	}
	else
	{

	}
}

void CPlayer_Controller::EarthQuake()
{
	if (-1.f == m_fStatusDuration[(_uint)STATUSEFFECT::EARTHQUAKE])
	{

	}
	else
	{

	}
}

void CPlayer_Controller::Shock()
{
	if (-1.f == m_fStatusDuration[(_uint)STATUSEFFECT::SHOCK])
	{

	}
	else
	{

	}
}

void CPlayer_Controller::Stun()
{
	if (-1.f == m_fStatusDuration[(_uint)STATUSEFFECT::STUN])
	{

	}
	else
	{

	}
}

void CPlayer_Controller::Silence()
{
	if (-1.f == m_fStatusDuration[(_uint)STATUSEFFECT::SILENCE])
	{
		m_bSkillKeyActive = true;
	}
	else
	{
		m_bSkillKeyActive = false;
	}
}

void CPlayer_Controller::Get_BuffMessage(_uint iBuffStatus, _float fAmount, _float fDurtaion)
{
	if (iBuffStatus == (_uint)BUFFEFFECT::_END)
		return;

	/* 시작시 이미 이펙트상태면 초기화 */
	if (true == m_bBuffEffect[iBuffStatus])
	{
		m_bBuffEffect[iBuffStatus] = false;
		m_fBuffDuration[iBuffStatus] = -1.f;
		switch (iBuffStatus)
		{
		case (_uint)BUFFEFFECT::HALFDAMAGE:
			HalfDamage(0.0f);
			break;
		case (_uint)BUFFEFFECT::MANAREFILL:
			ManaRefill(0.0f);
			break;
		case (_uint)BUFFEFFECT::HPREFILL:
			HPRefill(0.0f);
			break;
		case (_uint)BUFFEFFECT::STIIFIMMUNE:
			StiffImmune(0.0f);
			break;
		}
	}

	switch (iBuffStatus)
	{
	case (_uint)BUFFEFFECT::HALFDAMAGE:
		m_bBuffEffect[iBuffStatus] = true;
		m_fBuffDuration[iBuffStatus] = fDurtaion;
		HalfDamage(fAmount);
		break;
	case (_uint)BUFFEFFECT::MANAREFILL:
		m_bBuffEffect[iBuffStatus] = true;
		m_fBuffDuration[iBuffStatus] = fDurtaion;
		ManaRefill(fAmount);
		break;
	case (_uint)BUFFEFFECT::HPREFILL:
		m_bBuffEffect[iBuffStatus] = true;
		m_fBuffDuration[iBuffStatus] = fDurtaion;
		HPRefill(fAmount);
		break;
	case (_uint)BUFFEFFECT::STIIFIMMUNE:
		m_bBuffEffect[iBuffStatus] = true;
		m_fBuffDuration[iBuffStatus] = fDurtaion;
		StiffImmune(fAmount);
		break;
	default:
		break;
	}
}

void CPlayer_Controller::BuffEffect_Duration(const _float& fTimeDelta)
{
	for (size_t i = 0; i < (_uint)BUFFEFFECT::_END; i++)
	{
		if (false == m_bBuffEffect[i]) continue;

		m_fBuffDuration[i] -= fTimeDelta;

		if (0.0f >= m_fBuffDuration[i])
		{
			m_bBuffEffect[i] = false;
			m_fBuffDuration[i] = -1.f;

			switch (i)
			{
			case (_uint)BUFFEFFECT::HALFDAMAGE:
				HalfDamage(0.0f);
				break;
			case (_uint)BUFFEFFECT::MANAREFILL:
				ManaRefill(0.0f);
				break;
			case (_uint)BUFFEFFECT::HPREFILL:
				HPRefill(0.0f);
				break;
			case (_uint)BUFFEFFECT::STIIFIMMUNE:
				StiffImmune(0.0f);
				break;
			}
		}
	}
}

void CPlayer_Controller::HalfDamage(_float fAmount)
{
	if (-1.f == m_fBuffDuration[(_uint)BUFFEFFECT::HALFDAMAGE])
	{
		m_fBuffAmount[(_uint)BUFFEFFECT::HALFDAMAGE] = 0;
	}
	else
	{
		m_fBuffAmount[(_uint)BUFFEFFECT::HALFDAMAGE] = fAmount;
	}
}

void CPlayer_Controller::ManaRefill(_float fAmount)
{
	if (-1.f == m_fBuffDuration[(_uint)BUFFEFFECT::MANAREFILL])
	{
		m_fBuffAmount[(_uint)BUFFEFFECT::MANAREFILL] = 0;
	}
	else
	{
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurMp += tPcStat.iMaxMp * fAmount;
		if (tPcStat.iCurMp >= tPcStat.iMaxMp)
		{
			tPcStat.iCurMp = tPcStat.iMaxMp;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
	}
}

void CPlayer_Controller::HPRefill(_float fAmount)
{
	if (-1.f == m_fBuffDuration[(_uint)BUFFEFFECT::HPREFILL])
	{
		m_fBuffAmount[(_uint)BUFFEFFECT::HPREFILL] = 0;
	}
	else
	{
		CGameObject::STATDESC tPcStat = m_pOwner->Get_PlayerStat_Desc();
		tPcStat.iCurHp += tPcStat.iMaxHp * fAmount;
		if (tPcStat.iCurHp >= tPcStat.iMaxHp)
		{
			tPcStat.iCurHp = tPcStat.iMaxHp;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcStat);
	}
}

void CPlayer_Controller::StiffImmune(_float fAmount)
{
	if (-1.f == m_fBuffDuration[(_uint)BUFFEFFECT::STIIFIMMUNE])
	{
		m_fBuffAmount[(_uint)BUFFEFFECT::STIIFIMMUNE] = 0;
	}
	else
	{
		m_fBuffAmount[(_uint)BUFFEFFECT::STIIFIMMUNE] = fAmount;
	}
}

void CPlayer_Controller::Refill_Mana(_float fTimeDelta)
{
	CGameObject::STATDESC tPcDesc = m_pOwner->Get_PlayerStat_Desc();
	if (0 >= tPcDesc.iCurMp)
		tPcDesc.iCurMp = 0;

	if (tPcDesc.iCurMp >= tPcDesc.iMaxMp)
		return;

	m_fRefillManaAcc += fTimeDelta;
	if (1.f <= m_fRefillManaAcc)
	{
		m_fRefillManaAcc = 0.0f;

		tPcDesc.iCurMp += tPcDesc.iMaxMp * 0.1f;
		if (tPcDesc.iCurMp >= tPcDesc.iMaxMp)
		{
			tPcDesc.iCurMp = tPcDesc.iMaxMp;
		}
		m_pOwner->Set_PlayerStat_Desc(tPcDesc);
	}
}

void CPlayer_Controller::Free()
{
	__super::Free();

	for (auto& iter : m_Skills)
	{
		Safe_Release(iter.second);
	}
	m_Skills.clear();
}
