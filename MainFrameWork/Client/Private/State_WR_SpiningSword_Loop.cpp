#include "stdafx.h"
#include "..\Public\State_WR_SpiningSword_Loop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WR_SpiningSword_Loop::CState_WR_SpiningSword_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_SpiningSword_Loop::Initialize()
{
	m_iSpiningSword_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_windblade_02", 1.1f);
	if (m_iSpiningSword_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_SpiningSword_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_SpiningSword_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(5);
	m_SkillFrames.push_back(11);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WR_SpiningSword_Loop::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iSpiningSword_Loop, 0.1f, 0, 0, 1.2f);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_Loop, 1.3f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_Loop, 1.1f);
}

void CState_WR_SpiningSword_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_SpiningSword_Loop::Exit_State()
{
}

void CState_WR_SpiningSword_Loop::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSpiningSword_Loop))
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (25 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSpiningSword_Loop))
		m_pPlayer->Set_State(TEXT("Skill_WR_SpiningSword_End"));

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_Loop, 1.1f);
}

void CState_WR_SpiningSword_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iSpiningSword_Loop, 1.1f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_SpiningSword_Loop* CState_WR_SpiningSword_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_SpiningSword_Loop* pInstance = new CState_WR_SpiningSword_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_SpiningSword_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_SpiningSword_Loop::Free()
{
	__super::Free();
}
