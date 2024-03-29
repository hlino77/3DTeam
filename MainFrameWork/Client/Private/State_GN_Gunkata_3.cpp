#include "stdafx.h"
#include "..\Public\State_GN_Gunkata_3.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"

CState_GN_Gunkata_3::CState_GN_Gunkata_3(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Gunkata_3::Initialize()
{
	m_iGunkata_3 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_gunkata_03", 1.2f);
	if (m_iGunkata_3 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Gunkata_3::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Gunkata_3::Tick_State_NoneControl;

	m_SkillFrames.push_back(5);
	m_SkillFrames.push_back(12);
	m_SkillFrames.push_back(15);
	m_SkillFrames.push_back(18);
	m_SkillFrames.push_back(21);
	m_SkillFrames.push_back(24);
	m_SkillFrames.push_back(-1);


	m_EffectFrames.push_back(EFFECTFRAMEDESC(5, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(12, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(15, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(18, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(21, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(24, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC());

	m_SoundFrames.push_back(SOUNDDESC(3, TEXT("Effect"), TEXT("GN_Gunkata_149.wav")));
	m_SoundFrames.push_back(SOUNDDESC(10, TEXT("Effect"), TEXT("GN_Gunkata_150.wav")));
	m_SoundFrames.push_back(SOUNDDESC(22, TEXT("Effect"), TEXT("GN_Laugh_630.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_GN_Gunkata_3::Enter_State()
{
	m_iSkillCnt = 0;
	m_iEffectCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iGunkata_3, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_Gunkata_3::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Gunkata_3::Exit_State()
{
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_GN_Gunkata_3::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGunkata_3);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (-1 != m_EffectFrames[m_iEffectCnt].iFrame && m_EffectFrames[m_iEffectCnt].iFrame <= (_int)iAnimFrame)
	{
		if (m_EffectFrames[m_iEffectCnt].iFrame == 5)
		{
			Effect_Trail();
		}
		else
			Effect_Shot();

		m_iEffectCnt++;
	}

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

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGunkata_3))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (50 <= iAnimFrame)
	{
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
}

void CState_GN_Gunkata_3::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGunkata_3);

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

	if (-1 != m_EffectFrames[m_iEffectCnt].iFrame && m_EffectFrames[m_iEffectCnt].iFrame <= (_int)iAnimFrame)
	{
		if (m_EffectFrames[m_iEffectCnt].iFrame == 5)
		{
			Effect_Trail();
		}
		else
			Effect_Shot();

		m_iEffectCnt++;
	}
}

void CState_GN_Gunkata_3::Effect_Trail()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;	
	EFFECT_START(TEXT("GunkataTrail1"), &desc)

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.7f, 300.f, 0.2f, 18.f);
}

void CState_GN_Gunkata_3::Effect_Shot()
{
	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.05f, 500.f, 0.1f, 14.f);

	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	matWorld.Translation(static_cast<CPartObject*>(m_pPlayer->Get_Parts((CPartObject::PARTS)m_EffectFrames[m_iEffectCnt].iWeapon))->Get_Part_WorldMatrix().Translation());

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("GunkataBullet"), &desc)
}

CState_GN_Gunkata_3* CState_GN_Gunkata_3::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Gunkata_3* pInstance = new CState_GN_Gunkata_3(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Gunkata_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Gunkata_3::Free()
{
	__super::Free();
}
