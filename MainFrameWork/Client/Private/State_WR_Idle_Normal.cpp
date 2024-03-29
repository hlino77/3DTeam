#include "stdafx.h"
#include "..\Public\State_WR_Idle_Normal.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"

CState_WR_Idle_Normal::CState_WR_Idle_Normal(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Idle_Normal::Initialize()
{
	m_Idle_Normal = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_normal_1", 1.0f);
	if (m_Idle_Normal == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Idle_Normal::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Idle_Normal::Tick_State_NoneControl;

	return S_OK;
}

void CState_WR_Idle_Normal::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_Idle_Normal, 0.1f, 0, 0);
	m_pPlayer->Store_Part();

	m_pController->Get_StopMessage();
}

void CState_WR_Idle_Normal::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Idle_Normal::Exit_State()
{
}

void CState_WR_Idle_Normal::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pController->Is_Attack())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Mode_Battle"));
	}
	else if (true == m_pController->Is_Run())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run_Normal"));
		}
	}

}

void CState_WR_Idle_Normal::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_Idle_Normal* CState_WR_Idle_Normal::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Idle_Normal* pInstance = new CState_WR_Idle_Normal(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Idle_Normal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Idle_Normal::Free()
{
	__super::Free();
}
