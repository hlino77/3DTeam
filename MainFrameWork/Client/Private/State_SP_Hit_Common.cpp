#include "stdafx.h"
#include "..\Public\State_SP_Hit_Common.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "NavigationMgr.h"
#include "Cell.h"

CState_SP_Hit_Common::CState_SP_Hit_Common(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Hit_Common::Initialize()
{
	m_iHit_Dmg_1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"dmg_idle_1", 1.0f);
	if (m_iHit_Dmg_1 == -1)
		return E_FAIL;

	m_iHit_Dmg_2 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"dmg_idle_2", 1.0f);
	if (m_iHit_Dmg_2 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Hit_Common::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Hit_Common::Tick_State_NoneControl;

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_425.wav"), 0.3f)); //  Skill
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_SP_Hit_Common::Enter_State()
{

	if (m_pPlayer->Is_Control())
	{
		if (false == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt].strName))
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, 0.4f);
	}


	m_fForceDist = m_pPlayer->Get_TargetPos().y;
	Vec3 vHitCenter = m_pPlayer->Get_TargetPos();
	vHitCenter.y = 0.0f;

	Vec3 vPlayerPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vPlayerPos.y = 0.0f;
	Vec3 vDir = vPlayerPos - vHitCenter;
	vDir.Normalize();

	m_pPlayer->Get_RigidBody()->ClearForce(ForceMode::FORCE);
	m_pPlayer->Get_RigidBody()->ClearForce(ForceMode::VELOCITY_CHANGE);
	m_pPlayer->Get_RigidBody()->AddForce(vDir * m_fForceDist, ForceMode::FORCE);
	m_pPlayer->Set_TargetPos(vDir);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (true == pGameInstance->Random_Coin(0.5f))
	{
		m_pPlayer->Reserve_Animation(m_iHit_Dmg_1, 0.1f, 0, 0, m_fForceDist);
		m_iHit_Dmg = m_iHit_Dmg_1;
	}
	else
	{
		m_pPlayer->Reserve_Animation(m_iHit_Dmg_2, 0.1f, 0, 0, m_fForceDist);
		m_iHit_Dmg = m_iHit_Dmg_2;
	}
	RELEASE_INSTANCE(CGameInstance);

	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();

	/* ��Ʈ����*/
	m_pController->Get_HitEndMessage();
}

void CState_SP_Hit_Common::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Hit_Common::Exit_State()
{
	m_fTimeAcc = 0.f;
}

void CState_SP_Hit_Common::Tick_State_Control(_float fTimeDelta)
{
	if (false == CNavigationMgr::GetInstance()->Is_NeighborActive(m_pPlayer->Get_CurrLevel(), m_pPlayer) &&
		2 <= m_pPlayer->Get_ValtanPhase())
	{
		m_pPlayer->Set_State(TEXT("Fall"));
		return;
	}
	else
	{
		Hit_Dmg(fTimeDelta);
	}
}

void CState_SP_Hit_Common::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_SP_Hit_Common::Hit_Dmg(_float fTimeDelta)
{
	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
		return;
	}

	m_fTimeAcc += fTimeDelta;
	if (m_fDmgTime <= m_fTimeAcc)
	{
		m_pPlayer->Set_State(TEXT("Idle"));
		return;
	}
}

CState_SP_Hit_Common* CState_SP_Hit_Common::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Hit_Common* pInstance = new CState_SP_Hit_Common(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Hit_Common");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Hit_Common::Free()
{
	__super::Free();
}
