#include "stdafx.h"
#include "..\Public\State_GN_Attack_Hand3.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Camera_Player.h"

CState_GN_Attack_Hand3::CState_GN_Attack_Hand3(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Hand3::Initialize()
{
	m_Attack_Hand3 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_03", 1.4f);
	if (m_Attack_Hand3 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Hand3::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Hand3::Tick_State_NoneControl;


	/* �Ϲݰ��� ������ */
	m_AttackFrames.push_back(21);
	m_AttackFrames.push_back(-1);

	m_EffectFrames.push_back(EFFECTFRAMEDESC(22, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC());

	m_SoundFrames.push_back(SOUNDDESC(19, TEXT("Effect"), TEXT("GN_Shout_596")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_GN_Attack_Hand3::Enter_State()
{
	m_iAttackCnt = 0;
	m_iEffectCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_Hand3, 0.1f, 0, 0);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_IsAttackContinue = false;
}

void CState_GN_Attack_Hand3::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Hand3::Exit_State()
{
}

void CState_GN_Attack_Hand3::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand3);

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_AttackMessage();
	}

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);

		m_iSoundCnt++;
	}

	if (-1 != m_EffectFrames[m_iEffectCnt].iFrame && m_EffectFrames[m_iEffectCnt].iFrame <= (_int)iAnimFrame)
	{
		Effect_Shot();

		m_iEffectCnt++;
	}

	if (true == m_pController->Is_Attack() &&
		50 > iAnimFrame &&
		40 <= iAnimFrame)
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Hand3))
		m_pPlayer->Set_State(TEXT("Idle"));

	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (0 != iIdentity)
	{
		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_IsAttackContinue && 50 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Attack_Hand_1"));
	}
	else if (true == m_pController->Is_Run() && 50 < iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_GN_Attack_Hand3::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand3);

	if (m_EffectFrames[m_iEffectCnt].iFrame <= (_int)iAnimFrame)
	{
		Effect_Shot();

		m_iEffectCnt++;
	}
}

void CState_GN_Attack_Hand3::Effect_Shot()
{
	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.05f, 500.f, 0.1f, 18.f);

	CEffect_Manager::EFFECTPIVOTDESC desc;
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts((CPartObject::PARTS)m_EffectFrames[m_iEffectCnt].iWeapon))->Get_Part_WorldMatrix().Translation();

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	matWorld.Translation(vPos);
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("HandBullet"), &desc)

	_uint iCount = rand() % 2 + 1;
	for (_uint i = 0; i < iCount; ++i)
	{
		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);


		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(TEXT("HandBulletParticle"), &desc)
	}
}

CState_GN_Attack_Hand3* CState_GN_Attack_Hand3::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Hand3* pInstance = new CState_GN_Attack_Hand3(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Hand3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Hand3::Free()
{
	__super::Free();
}
