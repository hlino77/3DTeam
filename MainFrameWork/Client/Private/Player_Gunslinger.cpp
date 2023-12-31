#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Gunslinger.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "Player_Controller_GN.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

/* State */
#include "State_GN_Idle.h"
#include "State_GN_Run.h"
#include "NavigationMgr.h"
#include "State_GN_Dash.h"
#include "State_GN_Dash_End.h"
#include "State_GN_Dash_2.h"
#include "State_GN_Dash_2_End.h"
#include "State_GN_Attack_Hand1.h"
#include "State_GN_Attack_Hand2.h"
#include "State_GN_Attack_Hand3.h"
#include "State_GN_Attack_Shot1.h"
#include "State_GN_Attack_Shot2.h"
#include "State_GN_Attack_Long1.h"
#include "State_GN_Attack_Long2.h"
#include "State_GN_Identity.h"
#include "State_GN_Identity_Back.h"
#include "State_GN_Run_Identity.h"
#include "State_GN_Run_Identity_Back.h"

/* State_Skill */
#include "State_GN_FreeShooter.h"
#include "State_GN_TerminatingShot_Start.h"
#include "State_GN_TerminatingShot_End.h"
#include "State_GN_LastSupper.h"
#include "State_GN_QuickStep_Start.h"
#include "State_GN_QuickStep_End.h"
#include "State_GN_FocusShot_Start.h"
#include "State_GN_FocusShot_Loop.h"
#include "State_GN_FocusShot_End.h"
#include "State_GN_PerfectShot_Start.h"
#include "State_GN_PerfectShot_Loop.h"
#include "State_GN_PerfectShot_End.h"
#include "State_GN_Apocalypse_Start.h"
#include "State_GN_Apocalypse_Loop.h"
#include "State_GN_Apocalypse_Success.h"
#include "State_GN_TargetDown_Start.h"
#include "State_GN_TargetDown_Loop.h"
#include "State_GN_TargetDown_Shot.h"
#include "State_GN_TargetDown_End.h"
#include "State_GN_DeathFire_Start.h"
#include "State_GN_DeathFire_Success.h"
#include "State_GN_RapidFire_Start.h"
#include "State_GN_RapidFire_End.h"
#include "State_GN_Gunkata_1.h"
#include "State_GN_Gunkata_2.h"
#include "State_GN_Gunkata_3.h"
#include "State_GN_Grenade.h"
#include "State_GN_SprialChaser.h"
#include "State_GN_DeadHard_Start.h"
#include "State_GN_DeadHard_Loop.h"
#include "State_GN_DeadHard_End.h"

/* 스킬 */
#include "Skill_GN_Apocalypse.h"
#include "Skill_GN_DeadHard.h"
#include "Skill_GN_DeathFire.h"
#include "Skill_GN_FocusShot.h"
#include "Skill_GN_FreeShooter.h"
#include "Skill_GN_Grenade.h"
#include "Skill_GN_Gunkata.h"
#include "Skill_GN_LastSupper.h"
#include "Skill_GN_PerfectShot.h"
#include "Skill_GN_QuickStep.h"
#include "Skill_GN_RapidFire.h"
#include "Skill_GN_SpiralChaser.h"
#include "Skill_GN_TargetDown.h"
#include "Skill_GN_TerminatingShot.h"

#include "Effect_Manager.h"

CPlayer_Gunslinger::CPlayer_Gunslinger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer(pDevice, pContext)
{
}

CPlayer_Gunslinger::CPlayer_Gunslinger(const CPlayer_Gunslinger& rhs)
	: CPlayer(rhs)
{
}

HRESULT CPlayer_Gunslinger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Gunslinger::Initialize(void* pArg)
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

	m_pController->Get_GN_IdentityMessage((CPlayer_Controller_GN::GN_IDENTITY)m_iWeaponIndex);
	Set_Weapon_RenderState(m_iWeaponIndex);

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

void CPlayer_Gunslinger::Tick(_float fTimeDelta)
{
	m_pStateMachine->Tick_State(fTimeDelta);
	m_pController->Tick(fTimeDelta);

	if (KEY_TAP(KEY::Q))
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotTransform = m_pTransformCom;

		HRESULT hr = EFFECT_START(TEXT("tempHelix0"), &desc)
		if (E_FAIL == hr)
			MSG_BOX("Failed to Create tempHelix0");
	}

	__super::Tick(fTimeDelta);
}

void CPlayer_Gunslinger::LateTick(_float fTimeDelta)
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

HRESULT CPlayer_Gunslinger::Render()
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

HRESULT CPlayer_Gunslinger::Render_ShadowDepth()
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

void CPlayer_Gunslinger::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Gunslinger::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Gunslinger::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CPlayer_Gunslinger::OnCollisionEnter_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Gunslinger::OnCollisionExit_NoneControl(const _uint iColLayer, CCollider* pOther)
{

}

void CPlayer_Gunslinger::Set_Skill(CGameObject* pGameObject)
{
	WRITE_LOCK
}

void CPlayer_Gunslinger::Send_PlayerInfo()
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

void CPlayer_Gunslinger::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

void CPlayer_Gunslinger::Set_Weapon_RenderState(_uint iIndex, _bool Is_Shot2)
{
	switch (iIndex)
	{
	case Client::CPlayer_Controller_GN::HAND:
		m_Parts[CPartObject::PARTS::WEAPON_1]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_2]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_3]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_4]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(false);
		break;
	case Client::CPlayer_Controller_GN::LONG:
		m_Parts[CPartObject::PARTS::WEAPON_1]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_2]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_3]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_4]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(false);
		break;
	case Client::CPlayer_Controller_GN::SHOT:
		m_Parts[CPartObject::PARTS::WEAPON_1]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_2]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_3]->Set_Render(false);
		m_Parts[CPartObject::PARTS::WEAPON_4]->Set_Render(true);
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(false);
		break;
	}

	if (true == Is_Shot2)
		m_Parts[CPartObject::PARTS::WEAPON_5]->Set_Render(true);
}

HRESULT CPlayer_Gunslinger::Ready_Components()
{
	__super::Ready_Components();

	/* 클래스 컨트롤러 */
	CPlayer_Controller_GN::CONTROLL_DESC	Control_Desc;
	Control_Desc.pOwner = this;
	Control_Desc.pOwnerRigidBody = m_pRigidBody;
	Control_Desc.pOwnerTransform = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Controller_GN"),
		TEXT("Com_Controller"), (CComponent**)&m_pController, &Control_Desc)))
		return E_FAIL;

	/* 초기 장비 및 얼굴 설정 */
	/*wstring strComName = L"Prototype_Component_Model_GN_Legend_Helmet";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET])))
		return E_FAIL;

	m_IsHair = m_pModelPartCom[(_uint)PART::HELMET]->Is_HairTexture();

	strComName = L"Prototype_Component_Model_GN_Legend_Body";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_Legend_Leg";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Leg"), (CComponent**)&m_pModelPartCom[(_uint)PART::LEG])))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_Face";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pModelPartCom[(_uint)PART::FACE])))
		return E_FAIL;*/

	CModel::CHANGECOLOR pChangeColor;
	pChangeColor.vColor_R = Vec4(1.f, 1.f, 1.f, 1.f);
	pChangeColor.vColor_G = Vec4(1.f, 0.01f, 0.f, 0.692807f);
	pChangeColor.vColor_B = Vec4(0.01f, 1.f, 0.24f, 0.587838f);

	wstring strComName = L"Prototype_Component_Model_GN_Head_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET], &pChangeColor)))
		return E_FAIL;

	m_IsHair = m_pModelPartCom[(_uint)PART::HELMET]->Is_HairTexture();

	strComName = L"Prototype_Component_Model_GN_Body_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY], &pChangeColor)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_Parts()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	/* For.Part_Weapon_1 */
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Hand"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_1, pParts);

	/* For.Part_Weapon_2 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_2;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_2"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Hand_2"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_2, pParts);

	/* For.Part_Weapon_3 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_3;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Long"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_3, pParts);

	/* For.Part_Weapon_4 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_4;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Shot"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_4, pParts);

	/* For.Part_Weapon_5 */
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_5;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_2"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_GN_WP_Shot"), &PartDesc_Weapon);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CPartObject::PARTS::WEAPON_5, pParts);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_State()
{
	m_pStateMachine->Add_State(TEXT("Idle"), CState_GN_Idle::Create(TEXT("Idle"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Run"), CState_GN_Run::Create(TEXT("Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash"), CState_GN_Dash::Create(TEXT("Dash"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash_End"), CState_GN_Dash_End::Create(TEXT("Dash_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash_2"), CState_GN_Dash_2::Create(TEXT("Dash_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Dash_2_End"), CState_GN_Dash_2_End::Create(TEXT("Dash_2_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN"), CState_GN_Identity::Create(TEXT("Identity_GN"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN_Back"), CState_GN_Identity_Back::Create(TEXT("Identity_GN_Back"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN_Run"), CState_GN_Run_Identity::Create(TEXT("Identity_GN_Run"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Identity_GN_Run_Back"), CState_GN_Run_Identity_Back::Create(TEXT("Identity_GN_Run_Back"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Hand_1"), CState_GN_Attack_Hand1::Create(TEXT("Attack_Hand_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Hand_2"), CState_GN_Attack_Hand2::Create(TEXT("Attack_Hand_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Hand_3"), CState_GN_Attack_Hand3::Create(TEXT("Attack_Hand_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Shot_1"), CState_GN_Attack_Shot1::Create(TEXT("Attack_Shot_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Shot_2"), CState_GN_Attack_Shot2::Create(TEXT("Attack_Shot_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Long_1"), CState_GN_Attack_Long1::Create(TEXT("Attack_Long_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Attack_Long_2"), CState_GN_Attack_Long2::Create(TEXT("Attack_Long_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_FreeShoter"), CState_GN_FreeShooter::Create(TEXT("Skill_GN_FreeShoter"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_TerminatingShot_Start"), CState_GN_TerminatingShot_Start::Create(TEXT("Skill_GN_TerminatingShot_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_TerminatingShot_End"), CState_GN_TerminatingShot_End::Create(TEXT("Skill_GN_TerminatingShot_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_LastSupper"), CState_GN_LastSupper::Create(TEXT("Skill_GN_LastSupper"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_QuickStep_Start"), CState_GN_QuickStep_Start::Create(TEXT("Skill_GN_QuickStep_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_QuickStep_End"), CState_GN_QuickStep_End::Create(TEXT("Skill_GN_QuickStep_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_FocusShot_Start"), CState_GN_FocusShot_Start::Create(TEXT("Skill_GN_FocusShot_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_FocusShot_Loop"), CState_GN_FocusShot_Loop::Create(TEXT("Skill_GN_FocusShot_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_FocusShot_End"), CState_GN_FocusShot_End::Create(TEXT("Skill_GN_FocusShot_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_PerfectShot_Start"), CState_GN_PerfectShot_Start::Create(TEXT("Skill_GN_PerfectShot_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_PerfectShot_Loop"), CState_GN_PerfectShot_Loop::Create(TEXT("Skill_GN_PerfectShot_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_PerfectShot_End"), CState_GN_PerfectShot_End::Create(TEXT("Skill_GN_PerfectShot_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_Apocalypse_Start"), CState_GN_Apocalypse_Start::Create(TEXT("Skill_GN_Apocalypse_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_Apocalypse_Loop"), CState_GN_Apocalypse_Loop::Create(TEXT("Skill_GN_Apocalypse_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_Apocalypse_Success"), CState_GN_Apocalypse_Success::Create(TEXT("Skill_GN_Apocalypse_Success"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_TargetDown_Start"), CState_GN_TargetDown_Start::Create(TEXT("Skill_GN_TargetDown_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_TargetDown_Loop"), CState_GN_TargetDown_Loop::Create(TEXT("Skill_GN_TargetDown_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_TargetDown_Shot"), CState_GN_TargetDown_Shot::Create(TEXT("Skill_GN_TargetDown_Shot"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_TargetDown_End"), CState_GN_TargetDown_End::Create(TEXT("Skill_GN_TargetDown_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_DeathFire_Start"), CState_GN_DeathFire_Start::Create(TEXT("Skill_GN_DeathFire_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_DeathFire_Success"), CState_GN_DeathFire_Success::Create(TEXT("Skill_GN_DeathFire_Success"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_RapidFire_Start"), CState_GN_RapidFire_Start::Create(TEXT("Skill_GN_RapidFire_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_RapidFire_End"), CState_GN_RapidFire_End::Create(TEXT("Skill_GN_RapidFire_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_Gunkata_1"), CState_GN_Gunkata_1::Create(TEXT("Skill_GN_Gunkata_1"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_Gunkata_2"), CState_GN_Gunkata_2::Create(TEXT("Skill_GN_Gunkata_2"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_Gunkata_3"), CState_GN_Gunkata_3::Create(TEXT("Skill_GN_Gunkata_3"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_Grenade"), CState_GN_Grenade::Create(TEXT("Skill_GN_Grenade"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_SprialChaser"), CState_GN_SprialChaser::Create(TEXT("Skill_GN_SprialChaser"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_DeadHard_Start"), CState_GN_DeadHard_Start::Create(TEXT("Skill_GN_DeadHard_Start"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_DeadHard_Loop"), CState_GN_DeadHard_Loop::Create(TEXT("Skill_GN_DeadHard_Loop"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));

	m_pStateMachine->Add_State(TEXT("Skill_GN_DeadHard_End"), CState_GN_DeadHard_End::Create(TEXT("Skill_GN_DeadHard_End"),
		m_pStateMachine, static_cast<CPlayer_Controller*>(m_pController), this));


	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_Skill()
{
	CPlayer_Skill* pSkill = nullptr;
	CPlayer_Skill::PLAYERSKILL_DESC SkillDesc;
	/* 핸드건 스킬 */
	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_QuickStep_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_QuickStep_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_QuickStep_End")));
	pSkill = CSkill_GN_QuickStep::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::Q, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_DeathFire_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_DeathFire_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_DeathFire_Success")));
	pSkill = CSkill_GN_DeathFire::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::W, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_RapidFire_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_RapidFire_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_RapidFire_End")));
	pSkill = CSkill_GN_RapidFire::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::E, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_Gunkata_1");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_Gunkata_1")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_Gunkata_2")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_Gunkata_3")));
	pSkill = CSkill_GN_Gunkata::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::R, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_SprialChaser");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_SprialChaser")));
	pSkill = CSkill_GN_SpiralChaser::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::A, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_Grenade");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_Grenade")));
	pSkill = CSkill_GN_Grenade::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::S, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_DeadHard_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_DeadHard_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_DeadHard_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_DeadHard_End")));
	pSkill = CSkill_GN_DeadHard::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_HandSkill(CPlayer_Controller::SKILL_KEY::D, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	/* 샷건 스킬*/
	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_FreeShoter");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_FreeShoter")));
	pSkill = CSkill_GN_FreeShooter::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_ShotSkill(CPlayer_Controller::SKILL_KEY::Q, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_TerminatingShot_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_TerminatingShot_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_TerminatingShot_End")));
	pSkill = CSkill_GN_TerminatingShot::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_ShotSkill(CPlayer_Controller::SKILL_KEY::W, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_LastSupper");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_LastSupper")));
	pSkill = CSkill_GN_LastSupper::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_ShotSkill(CPlayer_Controller::SKILL_KEY::E, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	/* 장총 스킬 */
	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_FocusShot_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_FocusShot_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_FocusShot_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_FocusShot_End")));
	pSkill = CSkill_GN_FocusShot::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_LongSkill(CPlayer_Controller::SKILL_KEY::A, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_PerfectShot_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_PerfectShot_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_PerfectShot_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_PerfectShot_End")));
	pSkill = CSkill_GN_PerfectShot::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_LongSkill(CPlayer_Controller::SKILL_KEY::S, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_Apocalypse_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_Apocalypse_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_Apocalypse_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_Apocalypse_Success")));
	pSkill = CSkill_GN_Apocalypse::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_LongSkill(CPlayer_Controller::SKILL_KEY::D, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	SkillDesc.pOwner = this;
	SkillDesc.strSkill_StartName = TEXT("Skill_GN_TargetDown_Start");
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_TargetDown_Start")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_TargetDown_Loop")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_TargetDown_Shot")));
	SkillDesc.State_Skills.push_back(m_pStateMachine->Find_State(TEXT("Skill_GN_TargetDown_End")));
	pSkill = CSkill_GN_TargetDown::Create(m_pDevice, m_pContext, this, &SkillDesc);
	m_pController->Set_SkilltoCtrl(pSkill->Get_Skill_Name(), pSkill);
	m_pController->Bind_LongSkill(CPlayer_Controller::SKILL_KEY::F, m_pController->Find_Skill(pSkill->Get_Skill_Name()));
	SkillDesc.State_Skills.clear();

	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_Coliders()
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

HRESULT CPlayer_Gunslinger::Ready_PhysxBoneBranch()
{
	m_pModelCom->Play_Animation(10.0f);


	CPhysXMgr::GetInstance()->Add_Player(this);

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_04"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_r_05"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_04"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_capatcloth_l_05"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_br_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_b_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}

	{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_skirt_bl_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}
	
	/*{
		vector<_uint> Bones;

		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_01"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_02"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_03"));
		Bones.push_back(m_pModelCom->Find_BoneIndex(L"b_hair02_b_04"));

		CPhysXMgr::GetInstance()->Add_BoneBranch(this, Bones);
	}*/



	return S_OK;
}

HRESULT CPlayer_Gunslinger::Ready_SkillUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);
	return S_OK;
}

CPlayer_Gunslinger* CPlayer_Gunslinger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Gunslinger* pInstance = new CPlayer_Gunslinger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Gunslinger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Gunslinger::Clone(void* pArg)
{
	CPlayer_Gunslinger* pInstance = new CPlayer_Gunslinger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Gunslinger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Gunslinger::Free()
{
	__super::Free();
}
