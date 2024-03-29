#include "stdafx.h"
#include "UI_LoadingArrow.h"
#include "GameInstance.h"
#include "UI_LoadingFill.h"
#include "UI_Manager.h"

CUI_LoadingArrow::CUI_LoadingArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_LoadingArrow::CUI_LoadingArrow(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_LoadingArrow::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_LoadingArrow::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Loading_Arrow");

    m_fX = 50.f;
    m_fY = 850.f;
    m_fSizeX = 200.f;
    m_fSizeY = 30.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
    return S_OK;
}

void CUI_LoadingArrow::Tick(_float fTimeDelta)
{
}

void CUI_LoadingArrow::LateTick(_float fTimeDelta)
{
    if (m_bRender)
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_LoadingArrow::Render()
{
    __super::Render();

    return S_OK;
}

void CUI_LoadingArrow::Move_Arrow(_float fSizeX)
{
    m_fX = fSizeX - 50.f;
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
}

HRESULT CUI_LoadingArrow::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_BarArrow"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_LoadingArrow::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_LoadingArrow* CUI_LoadingArrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_LoadingArrow* pInstance = new CUI_LoadingArrow(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_LoadingArrow");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_LoadingArrow::Clone(void* pArg)
{
    CUI_LoadingArrow* pInstance = new CUI_LoadingArrow(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_LoadingArrow");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_LoadingArrow::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
