#include "stdafx.h"
#include "..\Public\State_WR_Iden_Skill.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Iden_Skill::CState_WR_Iden_Skill(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Iden_Skill::Initialize()
{
	m_iIdentity_Skill = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_bloodyrush", 1.4f);
	if (m_iIdentity_Skill == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Iden_Skill::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Iden_Skill::Tick_State_NoneControl;

	m_AttackFrames.push_back(24);
	m_AttackFrames.push_back(28);
	m_AttackFrames.push_back(34);
	m_AttackFrames.push_back(38);
	m_AttackFrames.push_back(44);
	m_AttackFrames.push_back(48);
	m_AttackFrames.push_back(66);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_WR_Iden_Skill::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_iIdentity_Skill, 0.2f, 0, 0, 1.f);

	m_pController->Get_StopMessage();
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	static_cast<CController_WR*>(m_pController)->Get_WR_IdenSkillMessage(10.f);

	static_cast<CController_WR*>(m_pController)->Set_Attack_Desc(4);
}

void CState_WR_Iden_Skill::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Iden_Skill::Exit_State()
{
}

void CState_WR_Iden_Skill::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iIdentity_Skill))
	{
		m_iAttackCnt++;

		static_cast<CController_WR*>(m_pController)->Get_AttackMessage();
	}
	

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity_Skill))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_WR_Iden_Skill::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Iden_Skill* CState_WR_Iden_Skill::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Iden_Skill* pInstance = new CState_WR_Iden_Skill(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Iden_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Iden_Skill::Free()
{
	__super::Free();
}
