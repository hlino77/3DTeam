#include "stdafx.h"
#include "..\Public\State_WDR_Iden_Skill.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_WDR_Iden_Skill::CState_WDR_Iden_Skill(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Iden_Skill::Initialize()
{
	m_Attack = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_vortexgravity", 1.f);
	if (m_Attack == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Iden_Skill::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Iden_Skill::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(21);
	m_AttackFrames.push_back(-1);


	// Sound
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_180.wav"))); // Skill
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_181.wav"))); // Skill
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_182.wav"))); // Skill
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_WDR_Iden_Skill::Enter_State()
{
	m_EffectSound = false;
	m_EffectSound1 = false;

	
	// Player Sound
	CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);

	
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_WDR*>(m_pController)->Get_WDR_IdenSkillMessage(5.f);
	static_cast<CController_WDR*>(m_pController)->Set_Attack_Desc(4);

}

void CState_WDR_Iden_Skill::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Iden_Skill::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

}

void CState_WDR_Iden_Skill::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame((_uint)m_Attack);


	if (false == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt + 0].strName))
	{
		if (false == m_EffectSound)
		{
			
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].strGroup, m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].fVolume);

			m_EffectSound = true;

		}
	}


	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= (_int)iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_AttackMessage();

		Effect_Shot();
	}


	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack))
	{
		if (true == static_cast<CController_WDR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Idle"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Idle"));
		}
	}

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_WDR_Iden_Skill::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt + 0].strName))
	{
		if (false == m_EffectSound)
		{

			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].strGroup, m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].fVolume);

			m_EffectSound = true;

		}
	}


	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= (_int)m_pPlayer->Get_ModelCom()->Get_Anim_Frame((_uint)m_Attack))
	{
		m_iAttackCnt++;
		Effect_Shot();
	}
}

void CState_WDR_Iden_Skill::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;

	EFFECT_START(L"VotexGravity", &tDesc);

	if (m_pPlayer->Is_Control())		
		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 100.0f, 0.5f, 10.0f);
}


CState_WDR_Iden_Skill* CState_WDR_Iden_Skill::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Iden_Skill* pInstance = new CState_WDR_Iden_Skill(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Iden_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Iden_Skill::Free()
{
	__super::Free();
}
