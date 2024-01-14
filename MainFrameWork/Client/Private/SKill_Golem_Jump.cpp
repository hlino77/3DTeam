#include "stdafx.h"
#include "SKill_Golem_Jump.h"
#include "GameInstance.h"
#include <ColliderOBB.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
CSKill_Golem_Jump::CSKill_Golem_Jump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_Golem_Jump::CSKill_Golem_Jump(const CSKill_Golem_Jump& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_Golem_Jump::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_Golem_Jump::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_iAtk = 10;
	m_fForce = 0.f;
	m_fMoveSpeed = 7.5f;
	m_fLastTime = 2.f;
    return S_OK;
}

void CSKill_Golem_Jump::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
	CSphereCollider* pCollider = m_Coliders[0];
	_float fRadius = pCollider->Get_Radius();
	fRadius += 3.f * fTimeDelta;
	pCollider->Set_Radius(fRadius);

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
	Vec3 vScale = pChildCollider->Get_Scale();
	vScale.z += 3.f * fTimeDelta;
	pChildCollider->Set_Scale(vScale);



	pChildCollider->Set_Scale(Vec3(vScale));
}

void CSKill_Golem_Jump::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSKill_Golem_Jump::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSKill_Golem_Jump::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Golem_Jump::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Golem_Jump::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_Golem_Jump::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkill"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		if (pCollider)
		{
			{
				CCollider::ColliderInfo tChildColliderInfo;
				tChildColliderInfo.m_bActive = false;
				tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
				COBBCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderSkillChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace(0, pCollider);
		}
	}

	{
		CSphereCollider* pCollider = m_Coliders[0];
		pCollider->Set_Radius(1.2f);
		pCollider->SetActive(true);
		pCollider->Set_Offset(Vec3(0.0f, 1.f, 0.f));;
		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(Vec3(1.f, 1.f, 0.5f));
		pChildCollider->Set_Offset(Vec3(0.0f, 1.f, 0.f));
		pChildCollider->SetActive(true);
	}

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
	return S_OK;
}

HRESULT CSKill_Golem_Jump::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSKill_Golem_Jump* CSKill_Golem_Jump::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_Golem_Jump* pInstance = new CSKill_Golem_Jump(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSKill_Golem_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_Golem_Jump::Clone(void* pArg)
{
    CSKill_Golem_Jump* pInstance = new CSKill_Golem_Jump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSKill_Golem_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_Golem_Jump::Free()
{
	__super::Free();
}

