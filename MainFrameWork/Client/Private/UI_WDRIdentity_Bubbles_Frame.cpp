#include "stdafx.h"
#include "UI_WDRIdentity_Bubbles_Frame.h"
#include "GameInstance.h"

CUI_WDRIdentity_Bubbles_Frame::CUI_WDRIdentity_Bubbles_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_WDRIdentity_Bubbles_Frame::CUI_WDRIdentity_Bubbles_Frame(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WDRIdentity_Bubbles_Frame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity_Bubbles_Frame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityWDR_BubblesFrame");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fAlpha = 1.f;
	m_vColor = { 3.f, 3.f, 3.f, 1.f };
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_WDRIdentity_Bubbles_Frame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_WDRIdentity_Bubbles_Frame::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_WDRIdentity_Bubbles_Frame::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_WDRIdentity_Bubbles_Frame::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_Bubbles_Frame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WDRIdentity_Bubbles_Frame::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

CUI_WDRIdentity_Bubbles_Frame* CUI_WDRIdentity_Bubbles_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WDRIdentity_Bubbles_Frame* pInstance = new CUI_WDRIdentity_Bubbles_Frame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WDRIdentity_Bubbles_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WDRIdentity_Bubbles_Frame::Clone(void* pArg)
{
	CUI_WDRIdentity_Bubbles_Frame* pInstance = new CUI_WDRIdentity_Bubbles_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WDRIdentity_Bubbles_Frame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WDRIdentity_Bubbles_Frame::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
