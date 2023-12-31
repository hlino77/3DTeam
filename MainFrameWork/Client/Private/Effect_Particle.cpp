#include "stdafx.h"
#include "Effect_Particle.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "VIBuffer_Particle.h"
#include "Utils.h"

CEffect_Particle::CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Particle::CEffect_Particle(const CEffect_Particle& rhs)
	: Super(rhs)
	, m_Billboard(rhs.m_Billboard)
	, m_Particle(rhs.m_Particle)
	, m_strParticlePassName(rhs.m_strParticlePassName)
{
}

HRESULT CEffect_Particle::Initialize_Prototype(EFFECTDESC* pDesc)
{
	if (FAILED(Super::Initialize_Prototype(pDesc)))
		return E_FAIL;

	m_Billboard.iBillboard = pDesc->iBillboard;

	m_Particle.vEmitDirection = pDesc->vEmitDirection;

	m_strParticlePassName = pDesc->strParticlePassName;

	return S_OK;
}

HRESULT CEffect_Particle::Initialize(void* pArg)
{
	if (FAILED(Super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Particle::Tick(_float fTimeDelta)
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

	m_Particle.fGameTime = m_fTimeAcc;
	m_Particle.fTimeStep = fTimeDelta;
}

void CEffect_Particle::LateTick(_float fTimeDelta)
{
	Super::LateTick(fTimeDelta);
}

HRESULT CEffect_Particle::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	m_Particle.vEmitPosition = m_matCombined.Translation();

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Billboard", &m_Billboard, sizeof(tagFX_Billboard))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Particle", &m_Particle, sizeof(tagFX_Particle))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Texture("g_RandomTexture", m_pRandomTextureSRV)))
		return E_FAIL;

	if (FAILED(m_pBuffer->Render(m_strParticlePassName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Buffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ParticleSystem"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	CUtils* pUtils = GET_INSTANCE(CUtils);
	pUtils->CreateRandomTexture1DSRV(m_pDevice, &m_pRandomTextureSRV);
	RELEASE_INSTANCE(CUtils);

	return S_OK;
}

CEffect_Particle* CEffect_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc)
{
	CEffect_Particle* pInstance = new CEffect_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX("Failed To Created : CEffect_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Particle::Clone(void* pArg)
{
	CEffect_Particle* pInstance = new CEffect_Particle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Particle::Free()
{
	Super::Free();
	Safe_Release(m_pBuffer);
}
