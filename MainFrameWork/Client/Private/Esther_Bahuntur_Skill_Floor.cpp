#include "stdafx.h"
#include "GameInstance.h"
#include "Esther_Bahuntur_Skill_Floor.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "PickingMgr.h"
#include "Struct.pb.h"
#include "NavigationMgr.h"
#include "Skill.h"
#include "Pool.h"

#include "Player.h"
#include "Esther.h"

CEsther_Bahuntur_Skill_Floor::CEsther_Bahuntur_Skill_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Eshter_Skill_Floor", OBJ_TYPE::ESTHER)
{
}

CEsther_Bahuntur_Skill_Floor::CEsther_Bahuntur_Skill_Floor(const CEsther_Bahuntur_Skill_Floor& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEsther_Bahuntur_Skill_Floor::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill_Floor::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);
	m_tCullingSphere.Radius = 2.0f;

	m_bActive = false;

	return S_OK;
}

void CEsther_Bahuntur_Skill_Floor::Tick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

}

void CEsther_Bahuntur_Skill_Floor::LateTick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (true == m_IsDissolve)
	{
		if (false == m_IsReverseDissolve)
		{
			m_fDissolveAcc += fTimeDelta;
			if (m_fDissolveAcc >= m_fMaxDissolve)
			{
				m_fDissolveAcc = m_fMaxDissolve;
				Set_Active(false);
			}
		}
		else
		{
			m_fDissolveAcc -= fTimeDelta;
			if (m_fDissolveAcc <= 0.0f)
			{
				m_fDissolveAcc = 0.0f;
				m_IsDissolve = false;
			}
		}
	}

	CullingObject();

	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;
		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
		}
	}
}

void CEsther_Bahuntur_Skill_Floor::Act1(_float fTimeDelta)
{
	m_IsDissolve = true;
	m_IsReverseDissolve = false;
	m_fDissolveAcc = 0.0f;
}

void CEsther_Bahuntur_Skill_Floor::Act2(_float fTimeDelta)
{
}

void CEsther_Bahuntur_Skill_Floor::Call_Finish()
{
	m_IsFinish = true;
}

void CEsther_Bahuntur_Skill_Floor::Ready()
{
	m_bActive = true;
	m_fDissolveAcc = m_fMaxDissolve;
	m_IsDissolve = true;
	m_IsReverseDissolve = true;
}

HRESULT CEsther_Bahuntur_Skill_Floor::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	if (true == m_IsDissolve)
	{
		_int   bDissolve = true;
		_int  bReverseDissovle = m_IsReverseDissolve;
		_float fDissolveDensity = 1.f;
		_float fDissolveValue = 0.1f;
		_float fDissolveColorValue = 0.05f;
		_int  bDissolveEmissive = true;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
			return E_FAIL;

		Vec4 vDissolveColor = Vec4(1.3f, 1.3f, 1.3f, 1.f);
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vDissolveColor, sizeof(Vec4))))
			return E_FAIL;

		_float g_fDissolveAmount = m_fDissolveAcc / m_fMaxDissolve;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &g_fDissolveAmount, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", m_pDissolveTexture->Get_SRV())))
			return E_FAIL;
	}

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render_SingleMesh(m_pShaderCom, i)))
			return E_FAIL;
	}

	if (true == m_IsDissolve)
	{
		_int bDissolve = false;
		_int   bReverseDissovle = false;
		_float fDissolveDensity = 1.f;
		_float fDissolveValue = 0.1f;
		_float fDissolveColorValue = 0.0f;
		_int   bDissolveEmissive = false;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
			return E_FAIL;

		Vec4 vDissolveColor = Vec4::One;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vDissolveColor, sizeof(Vec4))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill_Floor::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "ShadowPass")))
			return S_OK;
	}

	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill_Floor::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	wstring strComName = L"Prototype_Component_Model_ESBT_Skill_Floor";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_DissolveTexture_Monster"), TEXT("Com_DissolveTexture"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CEsther_Bahuntur_Skill_Floor::CullingObject()
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_tCullingSphere.Center = vPos;

	const BoundingFrustum& tCamFrustum = CGameInstance::GetInstance()->Get_CamFrustum();

	if (tCamFrustum.Intersects(m_tCullingSphere) == false)
		return;

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
	}

}

void CEsther_Bahuntur_Skill_Floor::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bRootRot, _bool bReverse, _bool bUseY)
{
	if (nullptr == m_pModelCom)
		return;

	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame, fRootDist, bRootRot, bReverse, bUseY);
}

CEsther_Bahuntur_Skill_Floor* CEsther_Bahuntur_Skill_Floor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Bahuntur_Skill_Floor* pInstance = new CEsther_Bahuntur_Skill_Floor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Bahuntur_Skill_Floor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Bahuntur_Skill_Floor::Clone(void* pArg)
{
	CEsther_Bahuntur_Skill_Floor* pInstance = new CEsther_Bahuntur_Skill_Floor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Bahuntur_Skill_Floor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Bahuntur_Skill_Floor::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTexture);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
