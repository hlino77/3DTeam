#include "stdafx.h"
#include "..\Public\State_SP_Fall.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"
#include "Renderer.h"

CState_SP_Fall::CState_SP_Fall(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Fall::Initialize()
{
	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Fall::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Fall::Tick_State_NoneControl;

	return S_OK;
}

void CState_SP_Fall::Enter_State()
{
	m_pController->Get_DeadMessage();
	m_pController->Get_HitEndMessage();

	m_pPlayer->Set_AnimationSpeed(0.0f);
	m_pPlayer->Set_SuperiorArmorState(true);
	m_pPlayer->Set_Navi(false);

	if (TEXT("Hit_Common") == m_pPlayer->Get_PreState())
	{
		m_vFallDir = m_pPlayer->Get_TargetPos();
		m_vFallDir.Normalize();
	}
	else
	{
		m_vFallDir = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		m_vFallDir.Normalize();
	}
	

	m_pPlayer->Get_RigidBody()->ClearForce(ForceMode::FORCE);
	m_pPlayer->Get_RigidBody()->ClearForce(ForceMode::VELOCITY_CHANGE);

	m_fTimeAcc = 0.0f;
	m_fStartAcc = 0.0f;

	m_bStart = false;

	CGameObject::STATDESC tPcDesc = m_pPlayer->Get_PlayerStat_Desc();
	tPcDesc.iCurHp = 0;
	m_pPlayer->Set_PlayerStat_Desc(tPcDesc);

	if (true == m_pPlayer->Is_Control())
		m_pPlayer->Get_RendererCom()->Set_DeadScene(true);
}

void CState_SP_Fall::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Fall::Exit_State()
{
	m_pPlayer->Set_AnimationSpeed(1.0f);
	m_pPlayer->Set_SuperiorArmorState(false);
	m_pPlayer->Get_RigidBody()->Set_Gravity(false);

	Vec3 vCellPos = CNavigationMgr::GetInstance()->Find_CloseCell_Middle(m_pPlayer->Get_CurrLevel(), m_pPlayer);
	vCellPos.y = 0;
	Vec3 vCenter = Vec3(100.f, 0.f, 100.f);
	Vec3 vDir = vCenter - vCellPos;
	vDir.Normalize();

	vCellPos += vDir * 0.5f;
	m_pPlayer->Set_TargetPos(vCellPos);
}

void CState_SP_Fall::Tick_State_Control(_float fTimeDelta)
{
	if (false == m_bStart)
	{
		m_fStartAcc += fTimeDelta;
		if (m_fStartTime < m_fStartAcc)
		{
			m_pPlayer->Get_RigidBody()->AddForce(m_vFallDir * -3.f, ForceMode::FORCE);
			m_pPlayer->Get_RigidBody()->Set_Gravity(true);
			m_bStart = true;
		}
	}
	if (true == m_bStart)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fFallTime <= m_fTimeAcc)
		{
			m_pPlayer->Set_State(TEXT("Dead_End"));
		}
	}
}

void CState_SP_Fall::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Fall* CState_SP_Fall::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Fall* pInstance = new CState_SP_Fall(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Fall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Fall::Free()
{
	__super::Free();
}
