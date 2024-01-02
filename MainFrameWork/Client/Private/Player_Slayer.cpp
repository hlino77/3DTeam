#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Slayer.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "NavigationMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "Controller_WR.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

/* State */
#include "State_WR_Idle.h"
#include "State_WR_Run.h"
#include "State_WR_Dash.h"
#include "State_WR_Attack_1.h"
#include "State_WR_Attack_2.h"
#include "State_WR_Attack_3.h"
#include "State_WR_Attack_4.h"
#include "State_WR_Identity.h"
#include "State_WR_Identity_Attack_1.h"
#include "State_WR_Identity_Attack_2.h"
#include "State_WR_Identity_Attack_3.h"
#include "State_WR_Identity_Attack_4.h"
#include "State_WR_Iden_Skill.h"

/* State_Skill */
#include "State_WR_FuriousClaw_Start.h"
#include "State_WR_FuriousClaw_Loop.h"
#include "State_WR_FuriousClaw_End.h"
#include "State_WR_SpiningSword_Start.h"
#include "State_WR_SpiningSword_Loop.h"
#include "State_WR_SpiningSword_End.h"
#include "State_WR_VolcanoEruption_Start.h"
#include "State_WR_VolcanoEruption_Start_1.h"
#include "State_WR_VolcanoEruption_Start_2.h"
#include "State_WR_VolcanoEruption_Loop.h"
#include "State_WR_VolcanoEruption_Fail.h"
#include "State_WR_VolcanoEruption_Success.h"
#include "State_WR_Guillotine_Start.h"
#include "State_WR_Guillotine_Loop.h"
#include "State_WR_Guillotine_End.h"
#include "State_WR_BrutalImpact_Start.h"
#include "State_WR_BrutalImpact_Loop.h"
#include "State_WR_BrutalImpact_End.h"
#include "State_WR_BrutalImpact_End_2.h"
#include "State_WR_WildStomp.h"
#include "State_WR_FlashBalde.h"
#include "State_WR_WildRush_Start.h"
#include "State_WR_WildRush_End.h"
#include "State_WR_WildRush_Stop.h"

/* 스킬 */
#include "Skill_WR_FuriousClaw.h"
#include "Skill_WR_SpiningSword.h"
#include "Skill_WR_VolcanoEruption.h"
#include "Skill_WR_Guillotine.h"
#include "Skill_WR_BrutalImpact.h"
#include "Skill_WR_WildStomp.h"
#include "Skill_WR_FlashBlade.h"
#include "Skill_WR_WildRush.h"


CPlayer_Slayer::CPlayer_Slayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer(pDevice, pContext)
{
}

CPlayer_Slayer::CPlayer_Slayer(const CPlayer_Slayer& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Slayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Slayer::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	if (FAILED(Ready_PhysxBoneBranch()))
		return E_FAIL;

	if (m_bControl)
	{
		if (FAILED(Ready_SkillUI()))
			return E_FAIL;
	}


	m_fAttackMoveSpeed = 8.0f;

	m_vHairColor_1 = { 0.78f, 0.78f, 0.78f, 1.f };
	m_vHairColor_2 = { 0.82f, 0.82f, 0.82f, 1.f };

	/* 플레이어 공통 요소 */
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_matTargetWorld = Desc->matWorld;
	m_vTargetPos = Desc->vTargetPos;

	Vec3 vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Set_WorldMatrix(Desc->matWorld);
	m_pTransformCom->Set_Scale(vScale);

	m_pStateMachine->Change_State(Desc->szState);

	CNavigationMgr::GetInstance()->Find_FirstCell(m_iCurrLevel, this);


	return S_OK;
}

void CPlayer_Slayer::Tick(_float fTimeDelta)
{
	m_pStateMachine->Tick_State(fTimeDelta);
	m_pController->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CPlayer_Slayer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pController->LateTick(fTimeDelta);

	Set_Colliders(fTimeDelta);

	if (m_bControl)
	{
		m_fSendInfoTime += fTimeDelta;
		if (m_fSendInfoTime >= 0.05f)
		{
			m_fSendInfoTime = 0.0f;
			Send_PlayerInfo();
		}
	}
}

HRESULT CPlayer_Slayer::Render()
{
	__super::Render();

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pModelPartCom[i]) continue;

		_uint		iNumMeshes = m_pModelPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (i == (_uint)PART::HELMET && j == m_IsHair)
			{
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &m_vHairColor_1, sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_vHairColor_2, sizeof(Vec4)))))
					return E_FAIL;

				if (FAILED(m_pModelPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &Vec4(), sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &Vec4(), sizeof(Vec4)))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pModelPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CPlayer_Slayer::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pModelPartCom[i]) continue;

		_uint		iNumMeshes = m_pModelPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{

			if (FAILED(m_pModelPartCom[i]->Render(m_pShaderCom, j, "ShadowPass")))
				return S_OK;
		}
	}

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

void CPlayer_Slayer::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS&&pOther->Get_Owner()->Get_ObjectType()==OBJ_TYPE::BOSS)		
		cout << "보스의 직접 공격!" << endl;
	else if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS && pOther->Get_Owner()->Get_ObjectType() == OBJ_TYPE::SKILL)
		cout << "보스의 스킬 공격!" << endl;
	else if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)
		cout << "넌 누구야?!" << endl;
	if (TEXT("WR_Identity_Skill") != Get_State() && false == m_pController->Is_Identity())
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				if(-1 != m_pController->Get_IdenGage())
					m_pController->Increase_IdenGage(10);
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				if (-1 != m_pController->Get_IdenGage())
					m_pController->Increase_IdenGage(10);
			}
		}
		else if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(50);
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				m_pController->Increase_IdenGage(50);
			}
		}
	}
	if (TEXT("Skill_WR_WildRush_End") == Get_State())
	{
		if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
			if ((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER == pOther->Get_ColLayer())
			{
				Set_TargetPos(Vec3(0.f, -1.f, 0.f));
			}
			else if ((_uint)LAYER_COLLIDER::LAYER_BODY_BOSS == pOther->Get_ColLayer())
			{
				Set_TargetPos(Vec3(0.f, -1.f, 0.f));
			}
		}
	}
}

void CPlayer_Slayer::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Slayer::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CPlayer_Slayer::OnCollisionEnter_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Slayer::OnCollisionExit_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Slayer::Set_Skill(CGameObject* pGameObject)
{
	WRITE_LOCK
}

void CPlayer_Slayer::Send_PlayerInfo()
{
	Protocol::S_OBJECTINFO pkt;

	auto tPlayerInfo = pkt.add_tobject();

	tPlayerInfo->set_iobjectid(m_iObjectID);
	tPlayerInfo->set_ilevel(m_iCurrLevel);
	tPlayerInfo->set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);


	auto vTargetPos = tPlayerInfo->mutable_vtargetpos();
	vTargetPos->Resize(3, 0.0f);
	Vec3 vPlayerTargetPos = m_vTargetPos.load();
	memcpy(vTargetPos->mutable_data(), &vPlayerTargetPos, sizeof(Vec3));


	auto matWorld = tPlayerInfo->mutable_matworld();
	matWorld->Resize(16, 0.0f);
	Matrix matPlayerWorld = m_pTransformCom->Get_WorldMatrix();
	memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));


	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

void CPlayer_Slayer::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

HRESULT CPlayer_Slayer::Ready_Components()
{
	__super::Ready_Components();

	/* 클래스 컨트롤러 */
	CController_WR::CONTROLL_DESC	Control_Desc;
	Control_Desc.pOwner = this;
	Control_Desc.pOwnerRigidBody = m_pRigidBody;
	Control_Desc.pOwnerTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Controller_WR"),
		TEXT("Com_Controller"), (CComponent**)&m_pController, &Control_Desc)))
		return E_FAIL;

	/* 초기 장비 및 얼굴 설정 */
	wstring strComName = L"Prototype_Component_Model_WR_Head_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET])))
		return E_FAIL;

	m_IsHair = m_pModelPartCom[(_uint)PART::HELMET]->Is_HairTexture();

	strComName = L"Prototype_Component_Model_WR_Body_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY])))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_Parts()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	/* For.Part_Weapon_1 */
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_weapon_rhand"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_WP_WR_Base"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_1, pParts);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_State()
{
	m_pStateMachine->Add_State(TEXT("Idle"), CState_WR_Idle::Create(TEXT("Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Run"), CState_WR_Run::Create(TEXT("Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash"), CState_WR_Dash::Create(TEXT("Dash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_1"), CState_WR_Attack_1::Create(TEXT("Attack_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_2"), CState_WR_Attack_2::Create(TEXT("Attack_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_3"), CState_WR_Attack_3::Create(TEXT("Attack_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_4"), CState_WR_Attack_4::Create(TEXT("Attack_4"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("WR_Identity"), CState_WR_Identity::Create(TEXT("WR_Identity"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_Attack_1"), CState_WR_Identity_Attack_1::Create(TEXT("Identity_Attack_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_Attack_2"), CState_WR_Identity_Attack_2::Create(TEXT("Identity_Attack_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_Attack_3"), CState_WR_Identity_Attack_3::Create(TEXT("Identity_Attack_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_Attack_4"), CState_WR_Identity_Attack_4::Create(TEXT("Identity_Attack_4"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("WR_Identity_Skill"), CState_WR_Iden_Skill::Create(TEXT("WR_Identity_Skill"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));
	
	/* 스킬 상태 */
	m_pStateMachine->Add_State(TEXT("Skill_WR_FuriousClaw_Start"), CState_WR_FuriousClaw_Start::Create(TEXT("Skill_WR_FuriousClaw_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_FuriousClaw_Loop"), CState_WR_FuriousClaw_Loop::Create(TEXT("Skill_WR_FuriousClaw_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_FuriousClaw_End"), CState_WR_FuriousClaw_End::Create(TEXT("Skill_WR_FuriousClaw_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_SpiningSword_Start"), CState_WR_SpiningSword_Start::Create(TEXT("Skill_WR_SpiningSword_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_SpiningSword_Loop"), CState_WR_SpiningSword_Loop::Create(TEXT("Skill_WR_SpiningSword_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_SpiningSword_End"), CState_WR_SpiningSword_End::Create(TEXT("Skill_WR_SpiningSword_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_VolcanoEruption_Start"), CState_WR_VolcanoEruption_Start::Create(TEXT("Skill_WR_VolcanoEruption_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_VolcanoEruption_Start_1"), CState_WR_VolcanoEruption_Start_1::Create(TEXT("Skill_WR_VolcanoEruption_Start_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_VolcanoEruption_Start_2"), CState_WR_VolcanoEruption_Start_2::Create(TEXT("Skill_WR_VolcanoEruption_Start_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_VolcanoEruption_Loop"), CState_WR_VolcanoEruption_Loop::Create(TEXT("Skill_WR_VolcanoEruption_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_VolcanoEruption_Fail"), CState_WR_VolcanoEruption_Fail::Create(TEXT("Skill_WR_VolcanoEruption_Fail"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_VolcanoEruption_Success"), CState_WR_VolcanoEruption_Success::Create(TEXT("Skill_WR_VolcanoEruption_Success"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_Guillotine_Start"), CState_WR_Guillotine_Start::Create(TEXT("Skill_WR_Guillotine_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_Guillotine_Loop"), CState_WR_Guillotine_Loop::Create(TEXT("Skill_WR_Guillotine_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_Guillotine_End"), CState_WR_Guillotine_End::Create(TEXT("Skill_WR_Guillotine_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_BrutalImpact_Start"), CState_WR_BrutalImpact_Start::Create(TEXT("Skill_WR_BrutalImpact_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_BrutalImpact_Loop"), CState_WR_BrutalImpact_Loop::Create(TEXT("Skill_WR_BrutalImpact_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_BrutalImpact_End"), CState_WR_BrutalImpact_End::Create(TEXT("Skill_WR_BrutalImpact_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_BrutalImpact_End_2"), CState_WR_BrutalImpact_End_2::Create(TEXT("Skill_WR_BrutalImpact_End_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_WildStomp"), CState_WR_WildStomp::Create(TEXT("Skill_WR_WildStomp"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_FlashBlade"), CState_WR_FlashBalde::Create(TEXT("Skill_WR_FlashBlade"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_WildRush_Start"), CState_WR_WildRush_Start::Create(TEXT("Skill_WR_WildRush_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_WildRush_End"), CState_WR_WildRush_End::Create(TEXT("Skill_WR_WildRush_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_WR_WildRush_Stop"), CState_WR_WildRush_Stop::Create(TEXT("Skill_WR_WildRush_Stop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_Skill()
{
	CPlayer_Skill* pSkill = nullptr;
	CPlayer_Skill::PLAYERSKILL_DESC SkillDesc;
	
	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_FuriousClaw_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_FuriousClaw_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_FuriousClaw_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_FuriousClaw_End")));
	pSkill = CSkill_WR_FuriousClaw::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::Q, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_SpiningSword_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_SpiningSword_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_SpiningSword_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_SpiningSword_End")));
	pSkill = CSkill_WR_SpiningSword::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::W, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();


	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_VolcanoEruption_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_VolcanoEruption_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_VolcanoEruption_Start_1")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_VolcanoEruption_Start_2")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_VolcanoEruption_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_VolcanoEruption_Fail")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_VolcanoEruption_Success")));
	pSkill = CSkill_WR_VolcanoEruption::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::E, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();


	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_Guillotine_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_Guillotine_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_Guillotine_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_Guillotine_End")));
	pSkill = CSkill_WR_Guillotine::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::R, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_BrutalImpact_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_BrutalImpact_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_BrutalImpact_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_BrutalImpact_End")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_BrutalImpact_End_2")));
	pSkill = CSkill_WR_BrutalImpact::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::A, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_WildStomp");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_WildStomp")));
	pSkill = CSkill_WR_WildStomp::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::S, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_FlashBlade");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_FlashBlade")));
	pSkill = CSkill_WR_FlashBlade::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::D, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_WR_WildRush_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_WildRush_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_WildRush_End")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_WR_WildRush_Stop")));
	pSkill = CSkill_WR_WildRush::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_Skill(CPlayer_Controller::SKILL_KEY::F, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_Coliders()
{
	if (false == m_bControl)
		return S_OK;

	{
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->SetActive(true);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Radius(0.7f);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));


		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Get_Child());
		pChildCollider->Set_Scale(Vec3(0.2f, 0.6f, 0.2f));
		pChildCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));
		pChildCollider->SetActive(true);
	}

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			Collider.second->Update_Collider();
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_PhysxBoneBranch()
{
	

	return S_OK;
}

HRESULT CPlayer_Slayer::Ready_SkillUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CPlayer_Slayer* CPlayer_Slayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Slayer* pInstance = new CPlayer_Slayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Slayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Slayer::Clone(void* pArg)
{
	CPlayer_Slayer* pInstance = new CPlayer_Slayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Slayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Slayer::Free()
{
	__super::Free();
}
