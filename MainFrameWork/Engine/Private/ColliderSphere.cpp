#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include "DebugDraw.h"
#include "Model.h"
#include "ColliderOBB.h"
#include "ColliderSphereGroup.h"
#include "ColliderFrustum.h"

CSphereCollider::CSphereCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext, ColliderType::Sphere)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
	:Super(rhs)
	, m_tBoundingSphere(rhs.m_tBoundingSphere)
{

}

HRESULT CSphereCollider::Initialize_Prototype()
{
	Super::Initialize_Prototype();

	return S_OK;
}

HRESULT CSphereCollider::Initialize(void* pArg)
{
	m_tBoundingSphere.Center = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
	m_tBoundingSphere.Radius = 1.0f;

	if (pArg == nullptr)
		return E_FAIL;

	ColliderInfo* tCollider = static_cast<ColliderInfo*>(pArg);

	m_iColLayer = tCollider->m_iLayer;
	m_bActive = tCollider->m_bActive;

	return S_OK;
}

void CSphereCollider::Update_Collider()
{
	Set_Center();

	if (m_pChild)
		m_pChild->Update_Collider();
}


void CSphereCollider::DebugRender()
{
	if (true == m_bColRender)
	{
		Super::DebugRender();

		m_pBatch->Begin();

		DX::Draw(m_pBatch, m_tBoundingSphere, Colors::Green);

		m_pBatch->End();


		if (m_pChild)
			m_pChild->DebugRender();
	}

}

_bool CSphereCollider::Intersects(SimpleMath::Ray& ray, OUT _float& distance)
{
	if (m_tBoundingSphere.Intersects(ray.position, ray.direction, OUT distance) == false)
		return false;

	if (m_pChild)
	{
		if (m_pChild->Intersects(ray, distance) == false)
			return false;
	}

	return true;
}

_bool CSphereCollider::Intersects(Super* other)
{
	if (Intersects_Bounding(other) == false)
		return false;

	if(other->Get_Child())
	{
		if (Intersects_Bounding(other->Get_Child()) == false)
			return false;
	}


	if (m_pChild)
	{
		if(m_pChild->Intersects(other) == false)
			return false;
	}


	return true;
}

_bool CSphereCollider::Intersects_Bounding(Super* other)
{
	_bool bCollision = false;

	switch (other->GetColliderType())
	{
	case ColliderType::Sphere:
		bCollision = m_tBoundingSphere.Intersects(static_cast<CSphereCollider*>(other)->GetBoundingSphere());
		break;
	case ColliderType::OBB:
		bCollision = m_tBoundingSphere.Intersects(static_cast<COBBCollider*>(other)->GetBoundingBox());
		break;
	case ColliderType::AABB:
		bCollision = false;
		break;
	case ColliderType::Frustum:
		bCollision = m_tBoundingSphere.Intersects(static_cast<CFrustumCollider*>(other)->GetBoundingFrustum());
		break;
	case ColliderType::Group:
		bCollision = other->Intersects(this);
		break;
	case ColliderType::Doughnut:
		bCollision = other->Intersects(this);
		break;
	}

	return bCollision;
}

_bool CSphereCollider::Intersects_Box(const BoundingBox& Collider)
{
	if (m_tBoundingSphere.Intersects(Collider))
	{
		if (m_pChild)
		{
			return m_pChild->Intersects_Box(Collider);
		}
		else
			return true;
	}

	return false;
}

void CSphereCollider::Set_Center()
{
	Matrix matObjectWorld = m_pOwner->Get_TransformCom()->Get_WorldMatrix();

	if (m_vOffset != Vec3(0.0f, 0.0f, 0.0f))
	{
		Vec3 vPos(0.0f, 0.0f, 0.0f);
		vPos = XMVector3TransformNormal(m_vOffset, matObjectWorld);
		vPos.Normalize();
		vPos *= m_vOffset.Length();
		vPos += Vec3(matObjectWorld.m[3]);
		m_tBoundingSphere.Center = vPos;
	}
	else
	{
		m_tBoundingSphere.Center = Vec3(matObjectWorld.m[3]);
	}
}

void CSphereCollider::Set_Center_ToBone()
{
	Matrix matOwnerWolrd = m_pOwner->Get_TransformCom()->Get_WorldMatrix();

	Matrix matBone = m_pOwner->Get_ModelCom()->Get_CurrBoneMatrix(m_iBoneIndex);

	Matrix matResult = matBone * matOwnerWolrd;
	if (m_vOffset != Vec3(0.0f, 0.0f, 0.0f))
	{
		Vec3 vPos(0.0f, 0.0f, 0.0f);
		vPos = XMVector3TransformNormal(m_vOffset, matResult);
		vPos.Normalize();
		vPos *= m_vOffset.Length();
		vPos += Vec3(matResult.m[3]);
		m_tBoundingSphere.Center = vPos;
	}
	else
	{
		m_tBoundingSphere.Center = Vec3(matResult.m[3]);
	}
}


CSphereCollider* CSphereCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSphereCollider* pInstance = new CSphereCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSphereCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CSphereCollider::Clone(CGameObject* pGameObject, void* pArg)
{
	CSphereCollider* pInstance = new CSphereCollider(*this);
	pInstance->m_pOwner = pGameObject;
	//pInstance->m_pRigidBody = pInstance->m_pGameObject->GetRigidBody();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSphereCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSphereCollider::Free()
{
	Super::Free();
}
