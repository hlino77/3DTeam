#include "stdafx.h"
#include "..\Public\State_WR_FuriousClaw_End.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "Model.h"

CState_WR_FuriousClaw_End::CState_WR_FuriousClaw_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_FuriousClaw_End::Initialize()
{
	m_iFuriousClaw_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_furiousclaw_03", 1.f);
	if (m_iFuriousClaw_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_FuriousClaw_End::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_FuriousClaw_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(3);
	m_SkillFrames.push_back(-1);

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WR_Ta_358.wav")));
	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("WR_Furi_126.wav"), 0.4f));
	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("WR_Furi_129.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_FuriousClaw_End::Enter_State()
{
	m_iSkillCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFuriousClaw_End, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_End, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_End, 1.f);

	m_bEffectStart = false;
	m_bTrail = false;
	m_Trails.clear();

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_FuriousClaw_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_FuriousClaw_End::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	for (auto& pTrail : m_Trails)
		static_cast<CEffect_Trail*>(pTrail)->TrailEnd(1.f);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_WR_FuriousClaw_End::Tick_State_Control(_float fTimeDelta)
{
	Effect_FuriousClaw_End();

	_int iCurrFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFuriousClaw_End);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= iCurrFrame)
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

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFuriousClaw_End))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFuriousClaw_End))
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
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_End, 1.f);
}

void CState_WR_FuriousClaw_End::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_End, 1.f);

	_int iCurrFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFuriousClaw_End);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= iCurrFrame)
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


	Effect_FuriousClaw_End();

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WR_FuriousClaw_End::Effect_FuriousClaw_End()
{
	if (!m_bEffectStart && -1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFuriousClaw_End))
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

		if (static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		{
			if (!m_bTrail)
			{
				m_bTrail = true;

				auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
				TRAIL_START_OUTLIST(TEXT("Slayer_Rage_FuriousClaw_Trail"), func, m_Trails)
			}

			EFFECT_START(TEXT("Slayer_Rage_FuriousClaw3"), &desc)
				EFFECT_START(TEXT("Slayer_Rage_FuriousClaw4"), &desc)
		}
		else
		{
			EFFECT_START(TEXT("Slayer_FuriousClaw3"), &desc)
				EFFECT_START(TEXT("Slayer_FuriousClaw4"), &desc)
		}

		m_bEffectStart = true;

		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}
}

CState_WR_FuriousClaw_End* CState_WR_FuriousClaw_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_FuriousClaw_End* pInstance = new CState_WR_FuriousClaw_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_FuriousClaw_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_FuriousClaw_End::Free()
{
	__super::Free();
}
