#include "stdafx.h"
#include "..\Public\State_WR_BrutalImpact_Start.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Trail.h"
#include "Effect_Manager.h"
#include "Model.h"

CState_WR_BrutalImpact_Start::CState_WR_BrutalImpact_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_BrutalImpact_Start::Initialize()
{
	m_iBrutalImpact_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_brutalimpact_01", 1.f);
	if (m_iBrutalImpact_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_BrutalImpact_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_BrutalImpact_Start::Tick_State_NoneControl;

	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("WR_Ch_398.wav")));
	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("WR_Brutal_29.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_BrutalImpact_Start::Enter_State()
{
	m_iSoundCnt = 0;
	m_bTrailStart = false;
	m_bEffectStart = false;

	m_pPlayer->Reserve_Animation(m_iBrutalImpact_Start, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Start, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Start, 1.f);


	m_pPlayer->Get_WR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_WR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_BrutalImpact_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_BrutalImpact_Start::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	for (auto& iter : m_Trails)
	{
		static_cast<CEffect_Trail*>(iter)->TrailEnd(0.5f);
	}

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_WR_BrutalImpact_Start::Tick_State_Control(_float fTimeDelta)
{
	Effect_BrutalImpact_Start();

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iBrutalImpact_Start);

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


	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iBrutalImpact_Start))
		m_pPlayer->Set_State(TEXT("Skill_WR_BrutalImpact_Loop"));

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Start, 1.f);
}

void CState_WR_BrutalImpact_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	Effect_BrutalImpact_Start();

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iBrutalImpact_Start);

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

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iBrutalImpact_Start, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WR_BrutalImpact_Start::Effect_BrutalImpact_Start()
{
	if (false == m_bTrailStart)
	{
		auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
		TRAIL_START_OUTLIST(TEXT("Slayer_BrutalImpact_Start"), func, m_Trails)

		m_bTrailStart = true;
	}

	if (false == m_bEffectStart)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;

		EFFECT_START(TEXT("Slayer_BrutalImpact_Loop_Impact1"), &desc)
		m_bEffectStart = true;
	}
}

CState_WR_BrutalImpact_Start* CState_WR_BrutalImpact_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_BrutalImpact_Start* pInstance = new CState_WR_BrutalImpact_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_BrutalImpact_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_BrutalImpact_Start::Free()
{
	__super::Free();
}
