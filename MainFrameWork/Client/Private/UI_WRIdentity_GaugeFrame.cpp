#include "stdafx.h"
#include "UI_WRIdentity_GaugeFrame.h"
#include "GameInstance.h"

CUI_WRIdentity_GaugeFrame::CUI_WRIdentity_GaugeFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_WRIdentity_GaugeFrame::CUI_WRIdentity_GaugeFrame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_WRIdentity_GaugeFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_WRIdentity_GaugeFrame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("WRIdentity_GaugeFrame");

    m_fX = 800.f;
    m_fY = 826.f;
    m_fSizeX = 240.f;
    m_fSizeY = 160.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_WRIdentity_GaugeFrame::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_WRIdentity_GaugeFrame::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_WRIdentity_GaugeFrame::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_WRIdentity_GaugeFrame::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_GaugeFrame"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_WRIdentity_GaugeFrame::Bind_ShaderResources()
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

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_WRIdentity_GaugeFrame* CUI_WRIdentity_GaugeFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_WRIdentity_GaugeFrame* pInstance = new CUI_WRIdentity_GaugeFrame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_WRIdentity_GaugeFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_WRIdentity_GaugeFrame::Clone(void* pArg)
{
    CUI_WRIdentity_GaugeFrame* pInstance = new CUI_WRIdentity_GaugeFrame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_WRIdentity_GaugeFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_WRIdentity_GaugeFrame::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
