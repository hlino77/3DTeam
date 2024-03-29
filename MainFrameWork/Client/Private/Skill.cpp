#include "stdafx.h"
#include "GameInstance.h"
#include "Skill.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "ServerSessionManager.h"
#include <ColliderDoughnut.h>
#include <ColliderFrustum.h>


CSkill::CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Skill", OBJ_TYPE::SKILL)
{
}

CSkill::CSkill(const CSkill& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkill::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_pSkillOwner = Desc->pOwner;
	m_szModelName = Desc->strFileName;
	m_iCurrLevel = m_pSkillOwner->Get_CurrLevel();
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if(m_pRigidBody)
		m_pRigidBody->SetMass(2.0f);
	
    return S_OK;
}

void CSkill::Tick(_float fTimeDelta)
{
	m_fLastTime -= fTimeDelta;
	if (m_fLastTime < 0.f)
		for (auto& Collider : m_Coliders)
			Collider.second->SetActive(false);
	if (m_fLastTime < -5.f)
		Set_Die();
}

void CSkill::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_DebugObject(this);
	Set_Colliders(fTimeDelta);
}

HRESULT CSkill::Render()
{

	return S_OK;
}

HRESULT CSkill::Render_Debug()
{
	for (auto& Colider : m_Coliders)
	{
		if (Colider.second->IsActive())
		{
			Colider.second->DebugRender();
			if (Colider.second->Get_Child())
				Colider.second->Get_Child()->DebugRender();
		}

	}

	return S_OK;
}

void CSkill::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	
}

void CSkill::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	
}
void CSkill::Set_SphereRadius( _float fRadius)
{
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(fRadius);
}

void CSkill::Set_DoughnutRadii(_float OutsideRadius, _float InsideRadius)
{
	if (m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child()->GetColliderType() != ColliderType::Doughnut)
		return;
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(OutsideRadius);

	dynamic_cast<CDoughnutCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child())->Set_Radius(InsideRadius);
}

void CSkill::Set_PizzaRadii(_float OutsideRadius, _float InsideRadius)
{
	if (m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child()->GetColliderType() != ColliderType::Frustum)
		return;
	if (OutsideRadius <= InsideRadius)
		return;
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(OutsideRadius);

	CFrustumCollider* pChildCollider = dynamic_cast<CFrustumCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());

	pChildCollider->Set_Far(OutsideRadius);
	pChildCollider->Set_Near(InsideRadius);
}

void CSkill::Set_PizzaSlope(_float fLeftSlope, _float fRightSlope)
{
	if (m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child()->GetColliderType() != ColliderType::Frustum)
		return;
	if (fLeftSlope == fRightSlope)
		return;

	CFrustumCollider* pChildCollider = dynamic_cast<CFrustumCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Get_Child());

	pChildCollider->Set_Slopes(Vec4(1.f, -0.05f, tanf(XMConvertToRadians(fLeftSlope)), tanf(XMConvertToRadians(fRightSlope))));
}

_bool CSkill::Get_Collider_Center(_uint iID, Vec3* pCenter)
{
	for (auto pCollider : m_Coliders)
	{
		if (pCollider.second->GetID() == iID)
		{
			*pCenter = pCollider.second->Get_Center();
			return true;
		}
	}
	return false;
}

HRESULT CSkill::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}


void CSkill::Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->LookAt_Lerp_ForLand(vDir, 5.0f, fTimeDelta);
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}


void CSkill::Set_Die()
{
	for (auto& Collider : m_Coliders)
		Collider.second->SetActive(false);

	m_bDead = true;
}

void CSkill::Find_NearTarget()
{
	m_pNearTarget = nullptr;
	m_pNearTarget = CGameInstance::GetInstance()->Find_NearGameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_PLAYER, this);
}

void CSkill::Send_NearTarget()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Protocol::S_NEARTARGET pkt;
	
	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilayer(m_iLayer);


	pkt.set_itargetobjectid(m_pNearTarget->Get_ObjectID());
	pkt.set_itargetobjectlayer(m_pNearTarget->Get_ObjectLayer());


	Safe_Release(pGameInstance);
}

_float CSkill::Get_NearTargetDistance()
{
	if (m_pNearTarget == nullptr)
		return 10000.0f;

	Vec3 vTargetPos = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fDistance = (vTargetPos - vPos).Length();
	return fDistance;
}



void CSkill::Send_ColliderState(const _uint& iLayer)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CSphereCollider* pCollider = m_Coliders[iLayer];

	Protocol::S_COLLIDERSTATE pkt;
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_ilayer(m_iLayer);

	pkt.set_icollayer(pCollider->Get_ColLayer());
	pkt.set_bactive(pCollider->IsActive());
	pkt.set_fradius(pCollider->Get_Radius());
	pkt.set_iattacktype(pCollider->Get_AttackType());
	pkt.set_iattack(pCollider->Get_Attack());
	pkt.set_bslow(pCollider->Get_SlowMotion());

	auto vOffset = pkt.mutable_voffset();
	vOffset->Resize(3, 0.0f);
	Vec3 vColliderOffset = pCollider->Get_Offset();
	memcpy(vOffset->mutable_data(), &vColliderOffset, sizeof(Vec3));

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);

	Safe_Release(pGameInstance);
}

void CSkill::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}


void CSkill::Free()
{
	__super::Free();

	for (auto& Collider : m_Coliders)
		CCollisionManager::GetInstance()->Out_Colider(Collider.second);

	Safe_Release(m_pModelCom);

	Safe_Release(m_pTransformCom);
}
