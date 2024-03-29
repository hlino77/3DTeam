#include "stdafx.h"
#include "..\Public\State_WR_Identity_Attack_1.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"
#include "Effect_Manager.h"

CState_WR_Identity_Attack_1::CState_WR_Identity_Attack_1(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Identity_Attack_1::Initialize()
{
	m_Attack_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_identity1_1_01", 1.2f);
	if (m_Attack_1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Identity_Attack_1::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Identity_Attack_1::Tick_State_NoneControl;

	/* 일반공격 프레임 */
	m_AttackFrames.push_back(9);
	m_AttackFrames.push_back(21);
	m_AttackFrames.push_back(-1);

	m_SoundFrames.push_back(SOUNDDESC(4, TEXT("Effect"), TEXT("GN_Huut_355.wav")));
	m_SoundFrames.push_back(SOUNDDESC(7, TEXT("Effect"), TEXT("WR_Iden_16.wav")));
	m_SoundFrames.push_back(SOUNDDESC());


	for (size_t i = 0; i < 2; i++)
	{
		m_bEffectOn[i] = false;
	}

	return S_OK;
}

void CState_WR_Identity_Attack_1::Enter_State()
{
	m_iAttackCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_1, 0.1f, 0, 0, 1.f);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	static_cast<CController_WR*>(m_pController)->Set_Attack_Desc(2);

	for (size_t i = 0; i < 2; i++)
	{
		m_bEffectOn[i] = false;
	}
}

void CState_WR_Identity_Attack_1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Identity_Attack_1::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

	m_IsAttackContinue = false;
}

void CState_WR_Identity_Attack_1::Tick_State_Control(_float fTimeDelta)
{
	Effect_Identity_Attack_1();

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_SoundFrames[m_iSoundCnt].bAddChannel)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}
		else
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		}

		m_iSoundCnt++;
	}

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= (_int)iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CController_WR*>(m_pController)->Get_AttackMessage();
	}

	if (true == m_pController->Is_Attack() &&
		35 > iAnimFrame &&
		25 <= iAnimFrame)
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
	else if (true == static_cast<CController_WR*>(m_pController)->Is_Identity())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("WR_Identity_Skill"));
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
	else if (true == m_IsAttackContinue && 35 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		{
			m_pPlayer->Set_State(TEXT("Identity_Attack_2"));
		}
		else
		{
			m_pPlayer->Set_State(TEXT("Attack_2"));
		}
	}
	else if (true == m_pController->Is_Run() && 35 < iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_WR_Identity_Attack_1::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	Effect_Identity_Attack_1();
}

void CState_WR_Identity_Attack_1::Effect_Identity_Attack_1()
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_1);

	if (4 <= iAnimFrame && false == m_bEffectOn[0])
	{
		auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
		TRAIL_START(TEXT("Slayer_Rage_Attack_1"), func)

		m_bEffectOn[0] = true;
	}

	if (17 <= iAnimFrame && false == m_bEffectOn[1])
	{
		auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
		TRAIL_START(TEXT("Slayer_Rage_Attack_1"), func)

		m_bEffectOn[1] = true;
	}
}

CState_WR_Identity_Attack_1* CState_WR_Identity_Attack_1::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Identity_Attack_1* pInstance = new CState_WR_Identity_Attack_1(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Identity_Attack_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Identity_Attack_1::Free()
{
	__super::Free();
}
