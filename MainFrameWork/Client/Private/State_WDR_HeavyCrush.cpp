#include "stdafx.h"
#include "..\Public\State_WDR_HeavyCrush.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_WDR_HeavyCrush::CState_WDR_HeavyCrush(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_HeavyCrush::Initialize()
{
	m_iHeavyCrush = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_heavycrush_01", 1.2f);
	if (m_iHeavyCrush == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_HeavyCrush::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_HeavyCrush::Tick_State_NoneControl;

	m_SkillFrames.push_back(40);
	m_SkillFrames.push_back(-1);

	m_fChageEffectDelay = 0.1f;



	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_193.wav"))); //  Player 
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_24.wav"))); //  Skill  
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_100.wav"))); //  Skill  
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_WDR_HeavyCrush::Enter_State()
{
	m_EffectSound = false;


	CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
	CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);



	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iHeavyCrush, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);

	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_AddMarbleMessage(1);

	m_fChageEffectAcc = m_fChageEffectDelay;
}

void CState_WDR_HeavyCrush::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_HeavyCrush::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

}

void CState_WDR_HeavyCrush::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iHeavyCrush);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);

		Effect_Shot();

		if (m_EffectSound == false)
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].strGroup, m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].fVolume);
			m_EffectSound = true;
		}

	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iHeavyCrush))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (iAnimFrame < 40)
	{
		Effect_Charge(fTimeDelta);
	}

	
	if (55 <= iAnimFrame)
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

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
		{
			m_pPlayer->Set_State(TEXT("WDR_Identity"));
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
}

void CState_WDR_HeavyCrush::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iHeavyCrush);



	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		if (m_EffectSound == false)
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].strGroup, m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].fVolume);
			m_EffectSound = true;
		}
	}


	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		m_iSkillCnt++;

		Effect_Shot();
	}

	if (iAnimFrame < 40)
	{
		Effect_Charge(fTimeDelta);
	}
}


void CState_WDR_HeavyCrush::Effect_Charge(_float fTimeDelta)
{
	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iHeavyCrush);

	m_fChageEffectAcc += fTimeDelta;
	if (m_fChageEffectAcc >= m_fChageEffectDelay)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START_OUTLIST(L"PerfectSwingCharge", &tDesc, m_Effects);
		m_fChageEffectAcc = 0.0f;
	}

	for (auto& Effectiter = m_Effects.begin(); Effectiter != m_Effects.end();)
	{
		if ((*Effectiter)->Is_Active())
		{
			(*Effectiter)->Update_Pivot(matWorld);
			++Effectiter;
		}
		else
		{
			Effectiter = m_Effects.erase(Effectiter);
		}
	}
}

void CState_WDR_HeavyCrush::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START_OUTLIST(L"HeavyCrush", &tDesc, m_Effects);

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 100.0f, 0.5f, 10.0f);
}

CState_WDR_HeavyCrush* CState_WDR_HeavyCrush::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_HeavyCrush* pInstance = new CState_WDR_HeavyCrush(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_HeavyCrush");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_HeavyCrush::Free()
{
	__super::Free();
}
