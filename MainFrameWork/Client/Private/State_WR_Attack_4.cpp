#include "stdafx.h"
#include "..\Public\State_WR_Attack_4.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"

CState_WR_Attack_4::CState_WR_Attack_4(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Attack_4::Initialize()
{
	m_Attack_4 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_2_04", 1.4f);
	if (m_Attack_4 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Attack_4::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Attack_4::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(14);
	m_AttackFrames.push_back(-1);

	return S_OK;
}

void CState_WR_Attack_4::Enter_State()
{
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_4, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
}

void CState_WR_Attack_4::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Attack_4::Exit_State()
{
	m_IsAttackContinue = false;
}

void CState_WR_Attack_4::Tick_State_Control(_float fTimeDelta)
{
	if (m_AttackFrames[m_iAttackCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_4))
	{
		m_iAttackCnt++;
		static_cast<CController_WR*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		40 >= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_4) &&
		30 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_4))
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
	{
		if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("WR_Identity"));
		}
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_IsAttackContinue && 40 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_4))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Attack_1"));
	}
	else if (true == m_pController->Is_Run())
	{
		if (40 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_4))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
	else if (true == m_pController->Is_Idle())
	{
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_4))
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_WR_Attack_4::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Attack_4* CState_WR_Attack_4::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Attack_4* pInstance = new CState_WR_Attack_4(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Attack_4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Attack_4::Free()
{
	__super::Free();
}
