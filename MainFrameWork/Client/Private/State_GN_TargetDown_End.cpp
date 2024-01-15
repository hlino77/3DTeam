#include "stdafx.h"
#include "..\Public\State_GN_TargetDown_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Custom_CrossHair.h"
#include "Camera_Player.h"


CState_GN_TargetDown_End::CState_GN_TargetDown_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TargetDown_End::Initialize()
{
	m_iTargetDown_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_targetdown_04", 1.0f);
	if (m_iTargetDown_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TargetDown_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TargetDown_End::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_TargetDown_End::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iTargetDown_End, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();

	m_bEffect = false;
}

void CState_GN_TargetDown_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TargetDown_End::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_TargetDown_End::Tick_State_Control(_float fTimeDelta)
{
	if (m_bEffect == false)
	{
		Effect_End();
		m_bEffect = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iTargetDown_End))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_GN_TargetDown_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_TargetDown_End::Effect_End()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect_Custom_CrossHair* pEffect = dynamic_cast<CEffect_Custom_CrossHair*>(pGameInstance->Find_GameObejct(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"GN_CrossHair"));
	
	pEffect->EffectEnd();

	m_pPlayer->Get_Camera()->DefaultLength(7.0f);


	CEffect* pDecal = dynamic_cast<CEffect*>(pGameInstance->Find_GameObejct(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, L"Effect_TargetDownDecal"));
	pDecal->Set_ObjectTag(L"Effect_TargetDownDecalReady");
	pDecal->EffectEnd();

	Safe_Release(pGameInstance);
}

CState_GN_TargetDown_End* CState_GN_TargetDown_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TargetDown_End* pInstance = new CState_GN_TargetDown_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TargetDown_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TargetDown_End::Free()
{
	__super::Free();
}
