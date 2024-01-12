#include "stdafx.h"
#include "..\Public\State_GN_TargetDown_Start.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Camera_Player.h"

CState_GN_TargetDown_Start::CState_GN_TargetDown_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TargetDown_Start::Initialize()
{
	m_iTargetDown_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_targetdown_01", 1.0f);
	if (m_iTargetDown_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TargetDown_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TargetDown_Start::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_TargetDown_Start::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iTargetDown_Start, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::LONG, m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffect = false;
}

void CState_GN_TargetDown_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TargetDown_Start::Exit_State()
{
}

void CState_GN_TargetDown_Start::Tick_State_Control(_float fTimeDelta)
{
	if (m_bEffect == false)
	{
		Effect_Start();
		m_bEffect = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iTargetDown_Start))
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_Loop"));
	}
		
}

void CState_GN_TargetDown_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_TargetDown_Start::Effect_Start()
{
	m_pPlayer->Get_Camera()->ZoomInOut(12.0f, 3.0f);

	CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_CrossHair");
}

CState_GN_TargetDown_Start* CState_GN_TargetDown_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TargetDown_Start* pInstance = new CState_GN_TargetDown_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TargetDown_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TargetDown_Start::Free()
{
	__super::Free();
}
