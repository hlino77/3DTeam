#include "stdafx.h"
#include "..\Public\State_GN_TerminatingShot_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Camera_Player.h"

CState_GN_TerminatingShot_End::CState_GN_TerminatingShot_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TerminatingShot_End::Initialize()
{
	m_iTerminatingShot2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_terminatingshot_02", 1.2f);
	if (m_iTerminatingShot2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TerminatingShot_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TerminatingShot_End::Tick_State_NoneControl;


	m_SkillFrames.push_back(21);

	m_SkillFrames.push_back(-1);


	m_ParticleName.push_back(L"TerminateParticle1");
	m_ParticleName.push_back(L"TerminateParticle2");
	m_ParticleName.push_back(L"TerminateParticle3");
	m_ParticleName.push_back(L"TerminateParticle4");
	m_ParticleName.push_back(L"TerminateParticle5");
	m_ParticleName.push_back(L"TerminateParticle6");
	m_ParticleName.push_back(L"TerminateParticle7");


	m_SoundFrames.push_back(SOUNDDESC(8, TEXT("Effect"), TEXT("GN_Termi_243.wav")));
	m_SoundFrames.push_back(SOUNDDESC(19, TEXT("Effect"), TEXT("GN_Termi_244.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_GN_TerminatingShot_End::Enter_State()
{
	m_iSkillCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iTerminatingShot2, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_TerminatingShot_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TerminatingShot_End::Exit_State()
{
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::SHOT, m_eSkillSelectKey);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_GN_TerminatingShot_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iTerminatingShot2);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_SoundFrames[m_iSoundCnt].bAddChannel)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}
		else
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		}

		m_iSoundCnt++;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd((_uint)m_iTerminatingShot2))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_GN_TerminatingShot_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iTerminatingShot2);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_SoundFrames[m_iSoundCnt].bAddChannel)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}
		else
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		}

		m_iSoundCnt++;
	}

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
	}
}

void CState_GN_TerminatingShot_End::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_4))->Get_Part_WorldMatrix().Translation();
	matWorld.Translation(vPos);

	if (m_pPlayer->Is_Control())
	{
		Vec3 vRadialPos = matWorld.Translation() + matWorld.Backward() * 1.3f;
		vRadialPos.y += 0.7f;
		m_pPlayer->Get_Camera()->Cam_Shake(0.2f, 500.f, 0.1f, 12.f);
		m_pPlayer->Get_Camera()->Set_RadialBlur(0.1f, vRadialPos, 0.2f, 0.1f);
	}

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("TerminateMuzzle1"), &desc);

	for (_uint i = 0; i < 30; ++i)
	{
		//Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 50) * 0.02f);

		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);

		_uint iParticleNameIndex = rand() % 7;

		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
		desc.pPivotMatrix = &matWorld;

		EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
	}
}

CState_GN_TerminatingShot_End* CState_GN_TerminatingShot_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TerminatingShot_End* pInstance = new CState_GN_TerminatingShot_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TerminatingShot_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TerminatingShot_End::Free()
{
	__super::Free();
}
