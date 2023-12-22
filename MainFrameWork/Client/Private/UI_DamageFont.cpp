#include "stdafx.h"
#include "UI_DamageFont.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "Pool.h"

CUI_DamageFont::CUI_DamageFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_DamageFont::CUI_DamageFont(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_DamageFont::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_DamageFont::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Damage_Font");
    m_szFont = L"���ǰ����";

    if (nullptr != pArg)
    {
        _uint* RenderTargetIndex = static_cast<_uint*>(pArg);
        m_szRenderTargetIndex += to_wstring(*RenderTargetIndex);
    }
    Ready_TextBox();

    Set_Active(false);

    return S_OK;
}

void CUI_DamageFont::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    if (m_bActive)
    {
        m_fDuration += fTimeDelta;
        if (1.5f < m_fDuration)
        {
            Set_Active(false);
            CPool<CUI_DamageFont>::Return_Obj(this);
        }
        if (!m_bCriticalHit)
        {
            if (0.2f >= m_fDuration)
            {
                m_pDamageFontWnd->Get_TransformCom()->Set_Scale(Vec3(m_vTextBoxMaxScale.x, m_vTextBoxMaxScale.y, 0.f));
            }
            else if (0.5f < m_fDuration)
            {
                m_pDamageFontWnd->Decrease_Alpha( 2.f * fTimeDelta );
                m_pDamageFontWnd->Get_TransformCom()->Set_Scale(Vec3(m_vTextBoxScale.x, m_vTextBoxScale.y, 0.f));
                m_pDamageFontWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(m_vHostPos.x, m_vHostPos.y + 50.f, m_fRandomZ));
            }
        }
        else
        {
            if (0.4f >= m_fDuration)
            {
                m_pDamageFontWnd->Get_TransformCom()->Set_Scale(Vec3(m_vTextBoxMaxScale.x, m_vTextBoxMaxScale.y, 0.f));
            }
            else if ((0.4f < m_fDuration) && (1.5f >= m_fDuration))
            {
                m_pDamageFontWnd->Get_TransformCom()->Set_Scale(Vec3(m_vTextBoxScale.x, m_vTextBoxScale.y, 0.f));
            }
        }
    }
}

void CUI_DamageFont::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_DamageFont::Render()
{

    return S_OK;
}

void CUI_DamageFont::Set_Active(_bool bActive)
{
    m_bActive = bActive;
    m_pDamageFontWnd->Set_Active(m_bActive);
}

void CUI_DamageFont::Print_DamageFont(Vec3 vPos, _float fLength, _bool IsCritical, _uint iDamage)
{
    m_pDamageFontWnd->Clear_Text();
    m_pDamageFontWnd->Set_Alpha(1.f);
    m_fDuration = 0.f;
    m_vTextBoxScale = Vec2(600.f, 80.f);
    m_vTextBoxScale *= 0.8f;
    m_bCriticalHit = IsCritical;
    Vec2 vTextPos(300.0f, 40.0f);
    Vec3 vHostPos = vPos;

    Matrix ViewMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
    Matrix ProjMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);

    vHostPos = XMVector3TransformCoord(vHostPos, ViewMatrix);
    vHostPos = XMVector3TransformCoord(vHostPos, ProjMatrix);
    m_vHostPos = Vec2(vHostPos.x, vHostPos.y);

    Vec2 vOffSet;
    vOffSet.x =   CGameInstance::GetInstance()->Get_RandomFloat(-1.f, 1.f);
    vOffSet.y = CGameInstance::GetInstance()->Get_RandomFloat(-1.f, 1.f);

    vOffSet.Normalize();
    vOffSet *= ( rand() % 10 ) * fLength;
    m_vHostPos += vOffSet;

    if (0 < iDamage)
    {
        if (!IsCritical)
        {
            wstring szDamage;
            szDamage = to_wstring(iDamage);
            
            _uint iIndex = 1;
            for (_uint i = szDamage.length() - 1; i > 0; --i)
            {
                if ((iIndex % 3) == 0)
                {
                    szDamage.insert(i, L",");
                }

                ++iIndex;
            }

            m_vTextBoxScale *= 0.8f;

            Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"���ǰ����", szDamage);
            Vec2 vOrigin = vMeasure * 0.5f;

            m_pDamageFontWnd->Set_Text(L"DamageFontWnd1", m_szFont, szDamage, Vec2(vTextPos.x - 2, vTextPos.y), Vec2(1.0f, 1.0f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 0.5f));
            m_pDamageFontWnd->Set_Text(L"DamageFontWnd2", m_szFont, szDamage, Vec2(vTextPos.x + 2, vTextPos.y), Vec2(1.0f, 1.0f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 0.5f));
            m_pDamageFontWnd->Set_Text(L"DamageFontWnd", m_szFont, szDamage, vTextPos, Vec2(1.0f, 1.0f), vOrigin, 0.f, m_vColorNormal);
        }
        else
        {
            wstring szDamage;
            szDamage = to_wstring(iDamage);

            _uint iIndex = 1;
            for (_uint i = szDamage.length()-1; i > 0; --i)
            {
                if ((iIndex % 3) == 0)
                {
                    szDamage.insert(i, L",");
                }

                ++iIndex;
            }

            Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"���ǰ����", szDamage);
            Vec2 vOrigin = vMeasure * 0.5f;

            m_pDamageFontWnd->Set_Text(L"DamageFontWnd1", m_szFont, szDamage, Vec2(vTextPos.x - 2, vTextPos.y), Vec2(1.0f, 1.0f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 0.5f));
            m_pDamageFontWnd->Set_Text(L"DamageFontWnd2", m_szFont, szDamage, Vec2(vTextPos.x + 2, vTextPos.y), Vec2(1.0f, 1.0f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 0.5f));
            m_pDamageFontWnd->Set_Text(L"DamageFontWnd", m_szFont, szDamage, vTextPos, Vec2(1.0f, 1.0f), vOrigin, 0.f, m_vColorCrit);
        }
        m_vTextBoxMaxScale = m_vTextBoxScale * 1.2f;

        m_fRandomZ = CGameInstance::GetInstance()->Get_RandomFloat(0.f, 1.f);

        m_pDamageFontWnd->Get_TransformCom()->Set_Scale(Vec3(m_vTextBoxMaxScale.x, m_vTextBoxMaxScale.y, 0.f));
        m_pDamageFontWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(m_vHostPos.x, m_vHostPos.y, m_fRandomZ));
    }

    Set_Active(true);
}

void CUI_DamageFont::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_DamageFont::Ready_Components()
{
    __super::Ready_Components();


    return S_OK;
}

HRESULT CUI_DamageFont::Bind_ShaderResources()
{
   
    return S_OK;
}

HRESULT CUI_DamageFont::Ready_TextBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    Safe_AddRef(pGameInstance);

    if (m_pDamageFontWnd == nullptr)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = L"DamageFontWnd";
        tTextDesc.szTextBoxTag += m_szRenderTargetIndex;
        tTextDesc.vSize = Vec2(600.f, 80.0f);
        m_vTextBoxScale = tTextDesc.vSize;
        m_pDamageFontWnd = static_cast<CTextBox*>(pGameInstance->Add_GameObject(LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
    
        if (m_pDamageFontWnd == nullptr)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pDamageFontWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pDamageFontWnd->Set_Pos(g_iWinSizeX* 0.5f, g_iWinSizeY * 0.5f);
       // m_pDamageFontWnd->Set_Pos(189.5f, 693.f);���� ��ġ�� ����
    }

    Safe_Release(pGameInstance);

    return S_OK;
}

void CUI_DamageFont::Release_ChatWindows()
{
    Safe_Release(m_pDamageFontWnd);
}

void CUI_DamageFont::Update_DamageFont()
{
   
}

void CUI_DamageFont::Start_DamageFont()
{
    m_szInputText.clear();
    m_iDamage = 0;
    m_pDamageFontWnd->Set_Active(true);
}

void CUI_DamageFont::End_DamageFont()
{
    m_szInputText.clear();
    m_iDamage = 0;
    m_pDamageFontWnd->Set_Active(false);
}

CUI_DamageFont* CUI_DamageFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_DamageFont* pInstance = new CUI_DamageFont(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_DamageFont");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_DamageFont::Clone(void* pArg)
{
    CUI_DamageFont* pInstance = new CUI_DamageFont(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_DamageFont");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_DamageFont::Free()
{
    __super::Free();
    Safe_Release(m_pDamageFontWnd);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}