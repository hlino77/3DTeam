#include "stdafx.h"
#include "..\Public\State_GN_Attack_Hand2.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "ColliderSphere.h"
#include "Model.h"

CState_GN_Attack_Hand2::CState_GN_Attack_Hand2(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Hand2::Initialize()
{
	m_Attack_Hand2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_02", 1.4f);
	if (m_Attack_Hand2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Hand2::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Hand2::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Attack_Hand2::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_Attack_Hand2, 0.1f, 0, 0);
	m_pController->Get_LerpLookMessage(m_pPlayer->Get_TargetPos());
}

void CState_GN_Attack_Hand2::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Hand2::Exit_State()
{
}

void CState_GN_Attack_Hand2::Tick_State_Control(_float fTimeDelta)
{
	if ((7 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2) && 9 >= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2)) ||
		(11 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2) && 13 >= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2)) || 
		(15 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2) && 17 >= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2)) ||
		(19 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2) && 21 >= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2)) ||
		(23 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2) && 25 >= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2)))
		m_pPlayer->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)->SetActive(true);
	else
		m_pPlayer->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)->SetActive(false);


	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if ( 0 != iIdentity )
	{
		if (30 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2))
		{
			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
		}
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);


		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		switch (eKey)
		{
		case Engine::CPlayer_Controller::Q:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::Q));
			break;
		case Engine::CPlayer_Controller::W:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::W));
			break;
		case Engine::CPlayer_Controller::E:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::E));
			break;
		case Engine::CPlayer_Controller::R:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::R));
			break;
		case Engine::CPlayer_Controller::A:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::A));
			break;
		case Engine::CPlayer_Controller::S:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::S));
			break;
		case Engine::CPlayer_Controller::D:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::D));
			break;
		case Engine::CPlayer_Controller::F:
			m_pPlayer->Set_State(m_pController->Get_SkillStartName(CPlayer_Controller::F));
			break;
		}
	}
	else if (true == m_pController->Is_Attack())
	{
		if (30 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2))
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Attack_Hand_3"));
		}
	}
	else if (true == m_pController->Is_Run())
	{
		if (30 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand2))
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
		if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Hand2))
			m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_GN_Attack_Hand2::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Attack_Hand2* CState_GN_Attack_Hand2::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Hand2* pInstance = new CState_GN_Attack_Hand2(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Hand2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Hand2::Free()
{
	__super::Free();
}
