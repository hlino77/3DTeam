#include "stdafx.h"
#include "UI_Mouse_ToolTip_Icon.h"
#include "GameInstance.h"
#include "Player_Skill.h"

CUI_Mouse_ToolTip_Icon::CUI_Mouse_ToolTip_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Mouse_ToolTip_Icon::CUI_Mouse_ToolTip_Icon(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Mouse_ToolTip_Icon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Mouse_ToolTip_Icon::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Mouse_ToolTip_Icon");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 44.f;
    m_fSizeY = 44.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_Mouse_ToolTip_Icon::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Mouse_ToolTip_Icon::LateTick(_float fTimeDelta)
{
    if (nullptr != m_pTextureCom)
        __super::LateTick(fTimeDelta);
}

HRESULT CUI_Mouse_ToolTip_Icon::Render()
{
    if (nullptr != m_pTextureCom)
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_Mouse_ToolTip_Icon::Set_IconTexture(CTexture* pTexture)
{
    m_pTextureCom = static_cast<CTexture*>(pTexture->Clone(nullptr, nullptr));
}

HRESULT CUI_Mouse_ToolTip_Icon::Ready_Components()
{
    __super::Ready_Components();

    return S_OK;
}

HRESULT CUI_Mouse_ToolTip_Icon::Bind_ShaderResources()
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

CUI_Mouse_ToolTip_Icon* CUI_Mouse_ToolTip_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Mouse_ToolTip_Icon* pInstance = new CUI_Mouse_ToolTip_Icon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Mouse_ToolTip_Icon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Mouse_ToolTip_Icon::Clone(void* pArg)
{
    CUI_Mouse_ToolTip_Icon* pInstance = new CUI_Mouse_ToolTip_Icon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Mouse_ToolTip_Icon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Mouse_ToolTip_Icon::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
