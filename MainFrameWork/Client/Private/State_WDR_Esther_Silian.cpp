#include "stdafx.h"
#include "..\Public\State_WDR_Esther_Silian.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"
#include "Esther.h"

CState_WDR_Esther_Silian::CState_WDR_Esther_Silian(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Esther_Silian::Initialize()
{
	m_iEsther = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"act_estherskill_1", 1.0f);
	if (m_iEsther == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Esther_Silian::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Esther_Silian::Tick_State_NoneControl;

	return S_OK;
}

void CState_WDR_Esther_Silian::Enter_State()
{
	m_bEstherActive = false;

	m_pPlayer->Reserve_Animation(m_iEsther, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_WDR_Controller()->Get_EshterGageUseMessage();

	m_pPlayer->Set_SuperiorArmorState(true);

	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_1, false);
}

void CState_WDR_Esther_Silian::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Esther_Silian::Exit_State()
{
	m_pPlayer->Set_SuperiorArmorState(false);

	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_1, true);
}

void CState_WDR_Esther_Silian::Tick_State_Control(_float fTimeDelta)
{
	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEsther) && false == m_bEstherActive)
	{
		m_pPlayer->Get_WDR_Controller()->Get_EstherMessage((_uint)CEsther::ESTHERTYPE::SA);
		m_bEstherActive = true;
	}
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iEsther))
	{
		if (true == m_pPlayer->Get_WDR_Controller()->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Iden_Idle"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Idle"));
		}
	}
}

void CState_WDR_Esther_Silian::Tick_State_NoneControl(_float fTimeDelta)
{
	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEsther) && false == m_bEstherActive)
	{
		m_pPlayer->Get_WDR_Controller()->Get_EstherMessage((_uint)CEsther::ESTHERTYPE::SA);
		m_bEstherActive = true;
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Esther_Silian* CState_WDR_Esther_Silian::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Esther_Silian* pInstance = new CState_WDR_Esther_Silian(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Esther_Silian");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Esther_Silian::Free()
{
	__super::Free();
}
