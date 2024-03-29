#include "stdafx.h"
#include "Skill_Golem_Jump.h"
#include "GameInstance.h"
#include <ColliderOBB.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
CSkill_Golem_Jump::CSkill_Golem_Jump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_Golem_Jump::CSkill_Golem_Jump(const CSkill_Golem_Jump& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_Golem_Jump::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Golem_Jump::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
	m_SkillDesc.iAtk = 10;
	m_SkillDesc.fForce = 30.f;
	m_fMoveSpeed = 7.5f;
	m_fLastTime = 0.4f;
    return S_OK;
}

void CSkill_Golem_Jump::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
	CSphereCollider* pCollider = m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS];
	_float fRadius = pCollider->Get_Radius();
	fRadius += 30.f * fTimeDelta;
	pCollider->Set_Radius(fRadius);

	COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
	Vec3 vScale = pChildCollider->Get_Scale();
	vScale.z += 30.f * fTimeDelta;
	pChildCollider->Set_Scale(vScale);



	pChildCollider->Set_Scale(Vec3(vScale));
}

void CSkill_Golem_Jump::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSkill_Golem_Jump::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSkill_Golem_Jump::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Golem_Jump::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_Golem_Jump::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSkill_Golem_Jump::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS;
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

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
		}
	}

	{
		CSphereCollider* pCollider = m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS];
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

HRESULT CSkill_Golem_Jump::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSkill_Golem_Jump* CSkill_Golem_Jump::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Golem_Jump* pInstance = new CSkill_Golem_Jump(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_Golem_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Golem_Jump::Clone(void* pArg)
{
    CSkill_Golem_Jump* pInstance = new CSkill_Golem_Jump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_Golem_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Golem_Jump::Free()
{
	__super::Free();
}

