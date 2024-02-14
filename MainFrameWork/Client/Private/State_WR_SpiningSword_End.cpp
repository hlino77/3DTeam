#include "stdafx.h"
#include "..\Public\State_WR_SpiningSword_End.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Model.h"

CState_WR_SpiningSword_End::CState_WR_SpiningSword_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_SpiningSword_End::Initialize()
{
	m_iSpiningSword_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_windblade_02", 1.1f);
	if (m_iSpiningSword_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_SpiningSword_End::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_SpiningSword_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(5);
	m_SkillFrames.push_back(11);
	m_SkillFrames.push_back(-1);

	m_SoundFrames.push_back(SOUNDDESC(3, TEXT("Effect"), TEXT("WR_Spin_100.wav")));
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_WR_SpiningSword_End::Enter_State()
{
	m_iSkillCnt = 0;
	m_iSoundCnt = 0;

	for (_int i = 0; i < 2; ++i)
	{
		m_bEffectStart[i] = false;
	}

	m_pPlayer->Reserve_Animation(m_iSpiningSword_End, 0.1f, 0, 0, 1.2f);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_End, 1.3f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_End, 1.1f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_SpiningSword_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_SpiningSword_End::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_SpiningSword_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSpiningSword_End);

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

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (false == m_bEffectStart[0])
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		tDesc.pPivotMatrix = &matPivot;

		vector<CEffect*> Effects;

		EFFECT_START_OUTLIST(L"Slayer_SpinningSword_Trail", &tDesc, Effects);

		CEffect* pEffect[2] = { Effects[0], Effects[1] };

		m_pPlayer->Add_Effect(L"SpinningSword_Trail_Slash", pEffect[0]);
		m_pPlayer->Add_Effect(L"SpinningSword_Trail_Wind", pEffect[1]);

		pEffect[0]->CB_UpdatePivot += bind(&CPlayer::Load_WorldMatrix, m_pPlayer, placeholders::_1);
		pEffect[0]->Set_Trace(true);

		pEffect[1]->CB_UpdatePivot += bind(&CPlayer::Load_WorldMatrix, m_pPlayer, placeholders::_1);
		pEffect[1]->Set_Trace(true);

		m_bEffectStart[0] = true;
	}

	if (false == m_bEffectStart[1] && 11 <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		tDesc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_SpinningSword_Smoke"), &tDesc)

			m_bEffectStart[1] = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSpiningSword_End))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (25 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pController->Is_Dash())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
				m_pPlayer->Set_State(TEXT("Identity_Attack_1"));
			else
				m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
		{
			if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
			{
				if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
					m_pPlayer->Set_TargetPos(vClickPos);
				else
					m_pPlayer->Set_TargetPos(Vec3());

				m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
			}
			else
			{
				m_pPlayer->Set_State(TEXT("WR_Identity"));
			}
		}
		else if (true == m_pController->Is_Run())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_End, 1.1f);
}

void CState_WR_SpiningSword_End::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_End, 1.1f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_SpiningSword_End* CState_WR_SpiningSword_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_SpiningSword_End* pInstance = new CState_WR_SpiningSword_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_SpiningSword_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_SpiningSword_End::Free()
{
	__super::Free();
}
