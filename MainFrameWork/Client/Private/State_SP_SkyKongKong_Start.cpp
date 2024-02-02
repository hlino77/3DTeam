#include "stdafx.h"
#include "..\Public\State_SP_SkyKongKong_Start.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"

CState_SP_SkyKongKong_Start::CState_SP_SkyKongKong_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_SkyKongKong_Start::Initialize()
{
	m_iSkyKongKong_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_skykongkong_01", 1.0f);
	if (m_iSkyKongKong_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_SkyKongKong_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_SkyKongKong_Start::Tick_State_NoneControl;

	m_SkillFrames.push_back(19);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_SP_SkyKongKong_Start::Enter_State()
{
	m_iSkillCnt = 0;
	m_bComboContinue = false;

	m_pPlayer->Reserve_Animation(m_iSkyKongKong_Start, 0.1f, 0, 0, 1.f);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());


	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bTrail = false;
}

void CState_SP_SkyKongKong_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_SkyKongKong_Start::Exit_State()
{

	if (true == m_pController->Is_HitState())
	{
		TrailEnd();

		if (20 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSkyKongKong_Start))
		{
			m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
		}
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);


}

void CState_SP_SkyKongKong_Start::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSkyKongKong_Start);

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	if (m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		Effect_Shot();
		m_iSkillCnt++;
		static_cast<CController_SP*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSkyKongKong_Start))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (true == m_pPlayer->Get_SP_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		10 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSkyKongKong_Start) &&
		20 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSkyKongKong_Start))
	{
		m_bComboContinue = true;
	}

	if (20 == iAnimFrame && true == m_bComboContinue)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Skill_SP_SkyKongKong_Loop"));
	}
	else if (30 <= iAnimFrame)
	{
		TrailEnd();

		m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);

		_uint iIdentity = static_cast<CController_SP*>(m_pController)->Is_SP_Identity();

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
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (0 != iIdentity)
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_Moon"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_Sun"));
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

void CState_SP_SkyKongKong_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_SP_SkyKongKong_Start::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"SkyKongKongShot", &tDesc);
}

void CState_SP_SkyKongKong_Start::Effect_Trail()
{
	vector<CEffect*> Effects;
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("SkyKongKongTrail"), func, Effects);
	m_pPlayer->Add_Effect(L"SkyKongKongTrail", Effects.front());
}

void CState_SP_SkyKongKong_Start::TrailEnd()
{
	m_pPlayer->Delete_Effect_Trail(L"SkyKongKongTrail", 5.0f);
}

CState_SP_SkyKongKong_Start* CState_SP_SkyKongKong_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_SkyKongKong_Start* pInstance = new CState_SP_SkyKongKong_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_SkyKongKong_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_SkyKongKong_Start::Free()
{
	__super::Free();
}
