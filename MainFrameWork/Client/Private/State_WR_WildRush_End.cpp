#include "stdafx.h"
#include "..\Public\State_WR_WildRush_End.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Model.h"

CState_WR_WildRush_End::CState_WR_WildRush_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_WildRush_End::Initialize()
{
	m_iWildRush = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_thrust_02", 1.f);
	if (m_iWildRush == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_WildRush_End::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_WildRush_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(5);
	m_SkillFrames.push_back(7);
	m_SkillFrames.push_back(9);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WR_WildRush_End::Enter_State()
{
	m_iSkillCnt = 0;
	m_bEffectStart = false;

	m_pPlayer->Reserve_Animation(m_iWildRush, 0.1f, 0, 0, 2.f);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildRush, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildRush, 1.f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_WildRush_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_WildRush_End::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_WildRush_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iWildRush);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}
	
	Effect_WildRush_End();

	if (-1.f == m_pPlayer->Get_TargetPos().y)
	{
		m_pPlayer->Get_ModelCom()->Save_PreFrame(iAnimFrame);
		m_pPlayer->Set_State(TEXT("Skill_WR_WildRush_Stop"));
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iWildRush))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (20 <= iAnimFrame)
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
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildRush, 1.f);
}

void CState_WR_WildRush_End::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iWildRush, 1.f);

	Effect_WildRush_End();

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WR_WildRush_End::Effect_WildRush_End()
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iWildRush);

	if (false == m_bEffectStart && 6 <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_WildRush"), &desc)

		m_bEffectStart = true;
	}
}

CState_WR_WildRush_End* CState_WR_WildRush_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_WildRush_End* pInstance = new CState_WR_WildRush_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_WildRush_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_WildRush_End::Free()
{
	__super::Free();
}
