#include "stdafx.h"
#include "UI_Player_HPFrame.h"
#include "GameInstance.h"

CUI_Player_HPFrame::CUI_Player_HPFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Player_HPFrame::CUI_Player_HPFrame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Player_HPFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Player_HPFrame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Player_HPFrame");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 850.f;
    m_fSizeX = g_iWinSizeX;
    m_fSizeY = 10.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_Player_HPFrame::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Player_HPFrame::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_Player_HPFrame::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_Player_HPFrame::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Frame"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player_HPFrame::Bind_ShaderResources()
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

CUI_Player_HPFrame* CUI_Player_HPFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Player_HPFrame* pInstance = new CUI_Player_HPFrame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Player_HPFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player_HPFrame::Clone(void* pArg)
{
    CUI_Player_HPFrame* pInstance = new CUI_Player_HPFrame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Player_HPFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Player_HPFrame::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
