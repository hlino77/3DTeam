#include "stdafx.h"
#include "..\Public\State_MG_Esther_Bahuntur.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"
#include "Esther.h"

CState_MG_Esther_Bahuntur::CState_MG_Esther_Bahuntur(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_Esther_Bahuntur::Initialize()
{
	m_iEsther = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"act_estherskill_1", 1.0f);
	if (m_iEsther == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_Esther_Bahuntur::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_Esther_Bahuntur::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_Esther_Bahuntur::Enter_State()
{
	m_bEstherActive = false;

	m_pPlayer->Reserve_Animation(m_iEsther, 0.1f, 0, 0);

	m_pPlayer->Get_MG_Controller()->Get_StopMessage();
	m_pPlayer->Get_MG_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperiorArmorState(true);

	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_1, false);
}

void CState_MG_Esther_Bahuntur::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_Esther_Bahuntur::Exit_State()
{
	m_pPlayer->Set_SuperiorArmorState(false);

	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_1, true);
}

void CState_MG_Esther_Bahuntur::Tick_State_Control(_float fTimeDelta)
{
	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEsther) && false == m_bEstherActive)
	{
		m_pPlayer->Get_MG_Controller()->Get_EstherMessage((_uint)CEsther::ESTHERTYPE::BT);
		m_bEstherActive = true;
	}
	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iEsther))
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_MG_Esther_Bahuntur::Tick_State_NoneControl(_float fTimeDelta)
{
	if (20 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEsther) && false == m_bEstherActive)
	{
		m_pPlayer->Get_MG_Controller()->Get_EstherMessage((_uint)CEsther::ESTHERTYPE::BT);
		m_bEstherActive = true;
	}

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_Esther_Bahuntur* CState_MG_Esther_Bahuntur::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_Esther_Bahuntur* pInstance = new CState_MG_Esther_Bahuntur(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_Esther_Bahuntur");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_Esther_Bahuntur::Free()
{
	__super::Free();
}
