#include "stdafx.h"
#include "..\Public\State_WDR_PowerStrike_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"

CState_WDR_PowerStrike_Loop::CState_WDR_PowerStrike_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PowerStrike_Loop::Initialize()
{
	m_iPowerStrike_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_powerstrike_02", 1.2f);
	if (m_iPowerStrike_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PowerStrike_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PowerStrike_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(26);
	m_SkillFrames.push_back(-1);

	// Sound
	m_SoundFrames.push_back(SOUNDDESC(10, TEXT("Effect"), TEXT("WDR_35.wav"))); // Skill
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WDR_PowerStrike_Loop::Enter_State()
{
	m_EffectSound = false;
	m_PlayerSound = false;


	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPowerStrike_Loop, 0.1f, 0, 0, 1.f);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WDR_PowerStrike_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PowerStrike_Loop::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_WDR_PowerStrike_Loop::Tick_State_Control(_float fTimeDelta)
{
	_int iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerStrike_Loop);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_EffectSound)
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
			m_EffectSound = true;
		}

	}

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
		
		Effect_Shot();
	}

	if (27 <= iAnimFrame)
		m_pPlayer->Set_State(TEXT("Skill_WDR_PowerStrike_End"));

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_WDR_PowerStrike_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	_int iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerStrike_Loop);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_EffectSound)
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
			m_EffectSound = true;
		}

	}


	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)m_pPlayer->Get_ModelCom()->Get_Anim_Frame((_uint)m_iPowerStrike_Loop))
	{
		m_iSkillCnt++;

		Effect_Shot();
	}
}

void CState_WDR_PowerStrike_Loop::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START(L"PowerStrike2", &tDesc);

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 100.0f, 0.5f, 10.0f);
}

CState_WDR_PowerStrike_Loop* CState_WDR_PowerStrike_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PowerStrike_Loop* pInstance = new CState_WDR_PowerStrike_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PowerStrike_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PowerStrike_Loop::Free()
{
	__super::Free();
}
