#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Player_Select_GN.h"
#include "Key_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "PickingMgr.h"
#include "ColliderOBB.h"
#include "PhysXMgr.h"
#include "Pool.h"
#include "CollisionManager.h"
#include "ColliderSphereGroup.h"
#include "Player_Skill.h"

CPlayer_Select_GN::CPlayer_Select_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPlayer_Select(pDevice, pContext)
{
}

CPlayer_Select_GN::CPlayer_Select_GN(const CPlayer_Select_GN& rhs)
	: CPlayer_Select(rhs)
{
}

HRESULT CPlayer_Select_GN::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Select_GN::Initialize(void* pArg)
{
	m_strObjectTag = TEXT("Gunslinger");

	__super::Initialize(pArg);

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	m_vSelectPos = Vec3(-0.2f, 0.04f, 0.642f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSelectPos);
	m_pTransformCom->My_Rotation(Vec3(0.f, 140.f, 0.f));

	m_iSelectAnim_Normal = m_pModelCom->Initailize_FindAnimation(L"characterdeselection_normal_loop_1", 1.0f);
	if (m_iSelectAnim_Normal == -1)
		return E_FAIL;

	m_iSelectAnim_Start = m_pModelCom->Initailize_FindAnimation(L"characterselection_normal_start_1", 1.0f);
	if (m_iSelectAnim_Start == -1)
		return E_FAIL;

	m_iSelectAnim_Loop = m_pModelCom->Initailize_FindAnimation(L"characterselection_normal_loop_1", 1.0f);
	if (m_iSelectAnim_Start == -1)
		return E_FAIL;

	m_iSelectAnim_End = m_pModelCom->Initailize_FindAnimation(L"characterselection_normal_end_1", 1.0f);
	if (m_iSelectAnim_Start == -1)
		return E_FAIL;

	m_iSelectAnim = m_iSelectAnim_Normal;
	Reserve_Animation(m_iSelectAnim, 0.2f, 0, 0);

	return S_OK;
}

void CPlayer_Select_GN::Tick(_float fTimeDelta)
{
	if (m_iSelectAnim != m_iSelectAnim_Normal && m_pModelCom->Is_AnimationEnd(m_iSelectAnim))
	{
		if (m_iSelectAnim == m_iSelectAnim_Start)
		{
			m_iSelectAnim = m_iSelectAnim_Loop;
			Reserve_Animation(m_iSelectAnim, 0.2f, 0, 0);
		}
		else if (m_iSelectAnim == m_iSelectAnim_End)
		{
			m_iSelectAnim = m_iSelectAnim_Normal;
			Reserve_Animation(m_iSelectAnim, 0.2f, 0, 0);
		}
	}

	__super::Tick(fTimeDelta);
}

void CPlayer_Select_GN::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Set_Colliders(fTimeDelta);

}

HRESULT CPlayer_Select_GN::Render()
{
	__super::Render();

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pModelPartCom[i]) continue;

		_uint		iNumMeshes = m_pModelPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pModelPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPlayer_Select_GN::Render_ShadowDepth()
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

void CPlayer_Select_GN::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

HRESULT CPlayer_Select_GN::Ready_Components()
{
	__super::Ready_Components();

	/* 초기 장비 및 얼굴 설정 */
	CModel::CHANGECOLOR pChangeColor;
	pChangeColor.vColor_R = Vec4(1.f, 1.f, 1.f, 1.f);
	pChangeColor.vColor_G = Vec4(1.f, 0.01f, 0.f, 0.692807f);
	pChangeColor.vColor_B = Vec4(0.01f, 1.f, 0.24f, 0.587838f);

	wstring strComName = L"Prototype_Component_Model_GN_Head_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelPartCom[(_uint)PART::HELMET], &pChangeColor)))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_Body_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PART::BODY], &pChangeColor)))
		return E_FAIL; 

	return S_OK;
}

HRESULT CPlayer_Select_GN::Ready_Coliders()
{
	{
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->SetActive(true);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Radius(0.5f);
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Set_Offset(Vec3(0.0f, 0.4f, 0.0f));


		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER]->Get_Child());
		pChildCollider->Set_Scale(Vec3(0.15f, 0.4f, 0.15f));
		pChildCollider->Set_Offset(Vec3(0.0f, 0.4f, 0.0f));
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

CPlayer_Select_GN* CPlayer_Select_GN::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Select_GN* pInstance = new CPlayer_Select_GN(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer_Select_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Select_GN::Clone(void* pArg)
{
	CPlayer_Select_GN* pInstance = new CPlayer_Select_GN(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer_Select_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Select_GN::Free()
{
	__super::Free();
}
