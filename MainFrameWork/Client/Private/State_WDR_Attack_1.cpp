#include "stdafx.h"
#include "..\Public\State_WDR_Attack_1.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"

CState_WDR_Attack_1::CState_WDR_Attack_1(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_Attack_1::Initialize()
{
	m_Attack_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_01", 1.2f);
	if (m_Attack_1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_Attack_1::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_Attack_1::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(12);
	m_AttackFrames.push_back(-1);


	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_7.wav"))); //  Skill
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_WDR_Attack_1::Enter_State()
{
	m_EffectSound = false;


	if (m_pPlayer->Is_Control())
	{
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);

	}
	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_1, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	static_cast<CController_WDR*>(m_pController)->Set_Attack_Desc(0);
}

void CState_WDR_Attack_1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_Attack_1::Exit_State()
{
	m_IsAttackContinue = false;
}

void CState_WDR_Attack_1::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimIndex = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1);

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= iAnimIndex)
	{
		m_iAttackCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		20 > iAnimIndex &&
		10 <= iAnimIndex)
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_1))
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
	else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
	{
		m_pPlayer->Set_State(TEXT("WDR_Identity"));
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_IsAttackContinue && 20 <= iAnimIndex)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());


		m_pPlayer->Set_State(TEXT("Attack_2"));
	}
	else if (true == m_pController->Is_Run())
	{
		if (20 < iAnimIndex)
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
}

void CState_WDR_Attack_1::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_Attack_1* CState_WDR_Attack_1::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_Attack_1* pInstance = new CState_WDR_Attack_1(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_Attack_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_Attack_1::Free()
{
	__super::Free();
}
