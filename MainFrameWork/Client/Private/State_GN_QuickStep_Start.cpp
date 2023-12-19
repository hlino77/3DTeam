#include "stdafx.h"
#include "..\Public\State_GN_QuickStep_Start.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_QuickStep_Start::CState_GN_QuickStep_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_QuickStep_Start::Initialize()
{
	m_iQuickStep_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_hallucination_01", 1.2f);
	if (m_iQuickStep_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_QuickStep_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_QuickStep_Start::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_QuickStep_Start::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iQuickStep_Start, 0.1f, 0, 0, 1.8f);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_QuickStep_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_QuickStep_Start::Exit_State()
{
	if (25 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_Start))
	{
		m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);

		if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
			m_pPlayer->Set_SuperArmorState(false);
	}
	m_bComboContinue = false;
}

void CState_GN_QuickStep_Start::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iQuickStep_Start))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (true == m_pPlayer->Get_GN_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		5 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_Start) &&
		10 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_Start))
	{
		m_bComboContinue = true;
	}
	else if (true == m_bComboContinue && 10 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_Start))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_QuickStep_End"));
	}
	else if (25 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_Start))
	{
		if (true == m_pController->Is_Dash())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Run())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
				
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_GN_QuickStep_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_QuickStep_Start* CState_GN_QuickStep_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_QuickStep_Start* pInstance = new CState_GN_QuickStep_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_QuickStep_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_QuickStep_Start::Free()
{
	__super::Free();
}
