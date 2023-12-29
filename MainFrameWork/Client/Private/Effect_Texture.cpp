#include "stdafx.h"
#include "Effect_Texture.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "VIBuffer_Point.h"

CEffect_Texture::CEffect_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Texture::CEffect_Texture(const CEffect_Texture& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Texture::Initialize_Prototype(EFFECTDESC* pDesc)
{
	if (FAILED(Super::Initialize_Prototype(pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Texture::Initialize(void* pArg)
{
	Matrix& matPivot = reinterpret_cast<CTransform*>(pArg)->Get_WorldMatrix();
	Vec3 vRight = matPivot.Right();
	vRight.Normalize();
	m_matPivot.m[0][0] = vRight.x; m_matPivot.m[0][1] = vRight.y; m_matPivot.m[0][2] = vRight.z;

	Vec3 vUp = matPivot.Up();
	vUp.Normalize();
	m_matPivot.m[1][0] = vRight.x; m_matPivot.m[1][1] = vRight.y; m_matPivot.m[1][2] = vRight.z;

	Vec3 vLook = matPivot.Backward();
	vLook.Normalize();
	m_matPivot.m[2][0] = vRight.x; m_matPivot.m[2][1] = vRight.y; m_matPivot.m[2][2] = vRight.z;

	m_matPivot.m[3][0] = matPivot.m[3][0];
	m_matPivot.m[3][1] = matPivot.m[3][1];
	m_matPivot.m[3][2] = matPivot.m[3][2];

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Texture::Tick(_float fTimeDelta)
{
	Super::Tick(fTimeDelta);

	if (m_IsSequence)
	{
		m_fSequenceTimer += fTimeDelta;
		while (m_fSequenceTimer > m_fSequenceTerm)
		{
			m_fSequenceTimer -= m_fSequenceTerm;
			++m_Variables.vUV_TileIndex.x;
		}

		while (m_Variables.vUV_TileIndex.x >= m_Variables.vUV_TileCount.x)
		{
			m_Variables.vUV_TileIndex.x -= m_Variables.vUV_TileCount.x;
			++m_Variables.vUV_TileIndex.y;

			while (m_Variables.vUV_TileIndex.y >= m_Variables.vUV_TileCount.y)
				m_Variables.vUV_TileIndex.y -= m_Variables.vUV_TileCount.y;
		}
	}
}

void CEffect_Texture::LateTick(_float fTimeDelta)
{
	Super::LateTick(fTimeDelta);
}

HRESULT CEffect_Texture::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Billboard", &m_Billboard, sizeof(tagFX_Billboard))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Begin("Default")))
		return E_FAIL;
	if (FAILED(m_pBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Texture::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Buffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

CEffect_Texture* CEffect_Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc)
{
	CEffect_Texture* pInstance = new CEffect_Texture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX("Failed To Created : CEffect_Texture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Texture::Clone(void* pArg)
{
	CEffect_Texture* pInstance = new CEffect_Texture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Texture");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Texture::Free()
{
	Super::Free();
	Safe_Release(m_pBuffer);
}
