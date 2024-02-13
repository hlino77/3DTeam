#include "stdafx.h"
#include "..\Public\State_WR_Guillotine_Loop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "Model.h"

CState_WR_Guillotine_Loop::CState_WR_Guillotine_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_Guillotine_Loop::Initialize()
{
	m_iGuillotine_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_guillotine_02", 1.f);
	if (m_iGuillotine_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_Guillotine_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_Guillotine_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(6);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WR_Guillotine_Loop::Enter_State()
{
	m_iSkillCnt = 0;
	m_bEffectStart = false;

	m_pPlayer->Reserve_Animation(m_iGuillotine_Loop, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.f);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	if (m_pPlayer->Is_Control())
		Init_Camera();
}

void CState_WR_Guillotine_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_Guillotine_Loop::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (m_pPlayer->Is_Control())
		Reset_Camera();
}

void CState_WR_Guillotine_Loop::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGuillotine_Loop);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGuillotine_Loop))
		m_pPlayer->Set_State(TEXT("Skill_WR_Guillotine_End"));

	if (false == m_bEffectStart && 4 <= iAnimFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix& matPivot = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Slayer_Guillotine_Slash"), &desc)

		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 60.0f, 0.8f, 5.0f);

		m_bEffectStart = true;
	}

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.f);

	Update_Camera(iAnimFrame, fTimeDelta);
}

void CState_WR_Guillotine_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iGuillotine_Loop, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WR_Guillotine_Loop::Init_Camera()
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_pPlayer->Get_TargetPos() - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();
	Vec3 vRight = Vec3::Up.Cross(vLook);
	vRight.Normalize();

	m_vCameraTargetPos = vPos + vLook * 1.f;

	Vec3 vTargetPos = vPos + vLook * 1.5f + Vec3::Up * 0.85f;

	Vec3 vOffset = vLook * 5.0f + Vec3::Up * 0.85f;
	vOffset.Normalize();

	pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
	pCamera->Set_TargetPos(vTargetPos);
	pCamera->Set_Offset(vOffset);
	pCamera->Set_CameraLength(1.25f);

	pCamera->ZoomInOut(10.0f, 15.0f);
}

void CState_WR_Guillotine_Loop::Update_Camera(_uint iAnimFrame, _float fTimeDelta)
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_vCameraTargetPos - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();

	pCamera->Set_Offset(vLook * 5.0f + Vec3::Up * 0.85f);
	pCamera->Set_TargetPos(vPos + vLook * 1.5f + Vec3::Up * 0.85f); // ���⼭ �ɾ��ָ� ���� ������ ���۶� �� ��ġ�� �Ǵ� ��
}

void CState_WR_Guillotine_Loop::Reset_Camera()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(7.0f);
}

CState_WR_Guillotine_Loop* CState_WR_Guillotine_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_Guillotine_Loop* pInstance = new CState_WR_Guillotine_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_Guillotine_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_Guillotine_Loop::Free()
{
	__super::Free();
}
