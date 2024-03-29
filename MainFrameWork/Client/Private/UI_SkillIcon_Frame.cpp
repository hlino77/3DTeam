#include "stdafx.h"
#include "UI_SkillIcon_Frame.h"
#include "GameInstance.h"
#include "Player.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "Controller_WR.h"
#include "Player_Slayer.h"
#include "Controller_WDR.h"
#include "Player_Destroyer.h"
#include "Controller_MG.h"
#include "Player_Bard.h"
#include "Controller_SP.h"
#include "Player_Doaga.h"
#include "TextBox.h"
#include "Sound_Manager.h"
#include "UI_Manager.h"

CUI_SkillIcon_Frame::CUI_SkillIcon_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_SkillIcon_Frame::CUI_SkillIcon_Frame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_SkillIcon_Frame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_eSkillKey = *static_cast<_uint*>(pArg) + 1;
        wstring strIndex = to_wstring(m_eSkillKey);
        m_strUITag = TEXT("SkillIcon_Frame");
        m_strUITag += strIndex;
    }
    else
        m_strUITag = TEXT("SkillIcon_Frame");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 64.f;
    m_fSizeY = 64.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    if (FAILED(Initialize_Percent()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Initialize_Percent()
{
    m_szFont = L"�ؽ�Lv1����Bold";
    Ready_TextBox();

    Set_Active(false);

    return S_OK;
}

void CUI_SkillIcon_Frame::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    Create_Rect();
    Picking_UI();
    Get_Player_BindingSkill();

    m_fResultCool = m_fCoolMaxTime - m_fCurrCool;
}

void CUI_SkillIcon_Frame::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
    if (!m_bPick)
    {
        if (0.f < m_fAlphaShine)
            m_fAlphaShine -= 2.f* fTimeDelta;
        else if (0 >= m_fAlphaShine)
            m_fAlphaShine = 0.f;
        m_bSound = false;
    }
    else
    {
        CUI_Manager::GetInstance()->Set_Player_Mouse(false);
        if (0.8f > m_fAlphaShine)
            m_fAlphaShine += 2.5f * fTimeDelta;
        else if (0.8f <= m_fAlphaShine)
            m_fAlphaShine = 0.8f;
        if (!m_bSound)
        {
            m_bSound = true;
            CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
        }
        Picking_Icon();
    }
    if (m_fCoolMaxTime == m_fResultCool)
    {
        m_fCoolAngle = XM_PI;
        if(nullptr != m_pCoolTimetWnd)
            m_pCoolTimetWnd->Set_Active(false);
    }
    else
    {
        m_fCoolRatio = 1.0f - (m_fResultCool / m_fCoolMaxTime);
        m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
        if((1 != m_iTextureIndex))
            Print_CoolTime();
    }
}

HRESULT CUI_SkillIcon_Frame::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if(m_bHaveSkill)
    {
        if (FAILED(Bind_ShaderResources_Skill()))
            return E_FAIL;
        m_pTextureCom_Skill->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
        m_pShaderCom->Begin(19);
        m_pVIBufferCom->Render();
    }

    if (FAILED(Bind_ShaderResources_Shine()))
        return E_FAIL;
    m_pTextureCom_Shine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pTextureCom_Frame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if((m_bHaveSkill)&&(m_fCoolMaxTime > m_fResultCool))
        m_pCoolTimetWnd->Render();

    return S_OK;
}

void CUI_SkillIcon_Frame::ReSet_PickedSkillIcon()
{
    if (true == CUI_Manager::GetInstance()->Is_PickedIcon())
    {
        if (KEY_AWAY(KEY::LBTN))
            CUI_Manager::GetInstance()->Reset_ItemIcon();
    }
}

void CUI_SkillIcon_Frame::Picking_Icon()
{
    if (false == CUI_Manager::GetInstance()->Is_PickedIcon())
    {
        if (KEY_HOLD(KEY::LBTN))
        {
            CUI_Manager::GetInstance()->Picked_SkillIcon(m_pSkill->Get_ObjectTag(), m_pSkill->Get_Skill_Texture(), m_eSkillKey);
        }
    }
    else if (true == CUI_Manager::GetInstance()->Is_PickedIcon())
    {
        if (KEY_AWAY(KEY::LBTN))
        {
            if ((CUI_Manager::GetInstance()->Is_PickedIcon())&&(0 == m_iTextureIndex))
            {
                Set_Player_BindingSkill();
                CUI_Manager::GetInstance()->Reset_ItemIcon();
                CUI_Manager::GetInstance()->Set_Player_Mouse(true);
            }
            else
            {
                CUI_Manager::GetInstance()->Reset_ItemIcon();
                CUI_Manager::GetInstance()->Set_Player_Mouse(true);
            }
        }
    }
}

void CUI_SkillIcon_Frame::Set_Player_BindingSkill()
{
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    if (nullptr != pPlayer && L"Gunslinger" == pPlayer->Get_ObjectTag())
    {
       CPlayer_Controller_GN::GN_IDENTITY eIDentity = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller())->Get_GN_Identity();
       CPlayer_Skill* pSkill = static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Get_PlayerSkill_Weapon(eIDentity, (CPlayer_Controller::SKILL_KEY)CUI_Manager::GetInstance()->Get_BindingKey());
       if(nullptr != pSkill)
            Set_GN_BindingSkill(pPlayer, pSkill);
    }
    else if (nullptr != pPlayer && L"WR" == pPlayer->Get_ObjectTag())
    {
        CPlayer_Skill* pSkill = static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Find_Skill(CUI_Manager::GetInstance()->Get_PickedTag());
        if (nullptr != pSkill)
            Set_WR_BindingSkill(pPlayer, pSkill);
    }
    else if (nullptr != pPlayer && L"WDR" == pPlayer->Get_ObjectTag())
    {
        CPlayer_Skill* pSkill = static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Find_Skill(CUI_Manager::GetInstance()->Get_PickedTag());
        if (nullptr != pSkill)
            Set_WDR_BindingSkill(pPlayer, pSkill);
    }
    else if (nullptr != pPlayer && L"SP" == pPlayer->Get_ObjectTag())
    {
        CPlayer_Skill* pSkill = static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Find_Skill(CUI_Manager::GetInstance()->Get_PickedTag());
        if (nullptr != pSkill)
            Set_SP_BindingSkill(pPlayer, pSkill);
    }
}

void CUI_SkillIcon_Frame::Set_GN_BindingSkill(CPlayer* pPlayer, CPlayer_Skill* pSkill)
{
    CPlayer_Skill* pOriginSkill = m_pSkill;
    _uint iOriginKey = (CPlayer_Controller::SKILL_KEY)m_eSkillKey;
    _uint iSwapKey = CUI_Manager::GetInstance()->Get_BindingKey();

    CPlayer_Controller_GN::GN_IDENTITY eIDentity = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller())->Get_GN_Identity();
    switch (eIDentity)
    {
    case CPlayer_Controller_GN::GN_IDENTITY::HAND:
        static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_HandSkill((CPlayer_Controller::SKILL_KEY)iOriginKey, pSkill);
        if (nullptr != pOriginSkill)
        {
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_HandSkill((CPlayer_Controller::SKILL_KEY)iSwapKey, pOriginSkill);
        }
        else
        {
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_HandSkill((CPlayer_Controller::SKILL_KEY)iSwapKey, nullptr);
        }
        static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Change_Skill_Iden(CPlayer_Controller_GN::GN_IDENTITY::HAND);
        break;
    case CPlayer_Controller_GN::GN_IDENTITY::SHOT:
        static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_ShotSkill((CPlayer_Controller::SKILL_KEY)iOriginKey, pSkill);
        if (nullptr != pOriginSkill)
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_ShotSkill((CPlayer_Controller::SKILL_KEY)iSwapKey, pOriginSkill);
        else
        {
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_ShotSkill((CPlayer_Controller::SKILL_KEY)iSwapKey, nullptr);
        }
        static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Change_Skill_Iden(CPlayer_Controller_GN::GN_IDENTITY::SHOT);
        break;
    case CPlayer_Controller_GN::GN_IDENTITY::LONG:
        static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_LongSkill((CPlayer_Controller::SKILL_KEY)iOriginKey, pSkill);
        if (nullptr != pOriginSkill)
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_LongSkill((CPlayer_Controller::SKILL_KEY)iSwapKey, pOriginSkill);
        else
        {
            static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Bind_LongSkill((CPlayer_Controller::SKILL_KEY)iSwapKey, nullptr);
        }
        static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Change_Skill_Iden(CPlayer_Controller_GN::GN_IDENTITY::LONG);
        break;
    }
}

void CUI_SkillIcon_Frame::Set_WR_BindingSkill(CPlayer* pPlayer, CPlayer_Skill* pSkill)
{
    CPlayer_Skill* pOriginSkill = m_pSkill;
    _uint iOriginKey = (CPlayer_Controller::SKILL_KEY)m_eSkillKey;
    _uint iSwapKey = CUI_Manager::GetInstance()->Get_BindingKey();

    static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Bind_Skill((CPlayer_Controller::SKILL_KEY)iOriginKey, pSkill);
    static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Bind_Skill((CPlayer_Controller::SKILL_KEY)iSwapKey, pOriginSkill);
}

void CUI_SkillIcon_Frame::Set_WDR_BindingSkill(CPlayer* pPlayer, CPlayer_Skill* pSkill)
{
    CPlayer_Skill* pOriginSkill = m_pSkill;
    _uint iOriginKey = (CPlayer_Controller::SKILL_KEY)m_eSkillKey;
    _uint iSwapKey = CUI_Manager::GetInstance()->Get_BindingKey();

    static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Bind_Skill((CPlayer_Controller::SKILL_KEY)iOriginKey, pSkill);
    static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Bind_Skill((CPlayer_Controller::SKILL_KEY)iSwapKey, pOriginSkill);
}

void CUI_SkillIcon_Frame::Set_SP_BindingSkill(CPlayer* pPlayer, CPlayer_Skill* pSkill)
{
    CPlayer_Skill* pOriginSkill = m_pSkill;
    _uint iOriginKey = (CPlayer_Controller::SKILL_KEY)m_eSkillKey;
    _uint iSwapKey = CUI_Manager::GetInstance()->Get_BindingKey();

    static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Bind_Skill((CPlayer_Controller::SKILL_KEY)iOriginKey, pSkill);
    static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Bind_Skill((CPlayer_Controller::SKILL_KEY)iSwapKey, pOriginSkill);
}

void CUI_SkillIcon_Frame::Get_Player_BindingSkill()
{
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    CTexture* pTexture = nullptr;

    if (nullptr != pPlayer && L"Gunslinger" == pPlayer->Get_ObjectTag())
        Get_Player_GN(pPlayer, pTexture);
    else if (nullptr != pPlayer && L"WR" == pPlayer->Get_ObjectTag())
        Get_Player_WR(pPlayer, pTexture);
    else if (nullptr != pPlayer && L"WDR" == pPlayer->Get_ObjectTag())
        Get_Player_WDR(pPlayer, pTexture);
    else if (nullptr != pPlayer && L"MG" == pPlayer->Get_ObjectTag())
        Get_Player_MG(pPlayer, pTexture);
    else if (nullptr != pPlayer && L"SP" == pPlayer->Get_ObjectTag())
        Get_Player_SP(pPlayer, pTexture);
}

void CUI_SkillIcon_Frame::Get_Player_GN(CPlayer* _pPlayer, CTexture* _pTexture)
{
    if (nullptr != _pPlayer)
    {
        CPlayer_Controller_GN::GN_IDENTITY eIDentity = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(_pPlayer)->Get_GN_Controller())->Get_GN_Identity();
        switch (eIDentity)
        {
        case CPlayer_Controller_GN::GN_IDENTITY::HAND:
            m_iTextureIndex = 0; 
            break;
        case CPlayer_Controller_GN::GN_IDENTITY::SHOT:
            if (5 <= m_eSkillKey)
            {
                m_iTextureIndex = 1;
                m_pCoolTimetWnd->Set_Active(false);
            }
            else
                m_iTextureIndex = 0;
            break;
        case CPlayer_Controller_GN::GN_IDENTITY::LONG:
            if (4 >= m_eSkillKey)
            {
                m_iTextureIndex = 1;
                m_pCoolTimetWnd->Set_Active(false);
            }
            else
                m_iTextureIndex = 0;
            break;
        }

        m_pSkill = static_cast<CPlayer_Gunslinger*>(_pPlayer)->
            Get_GN_Controller()->Get_PlayerSkill_Weapon(eIDentity, (CPlayer_Controller::SKILL_KEY)m_eSkillKey);
        if (nullptr != m_pSkill)
        {
            _pTexture = (m_pSkill->Get_Skill_Texture());
            m_fCoolMaxTime = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(_pPlayer)->Get_GN_Controller())->Get_Skill_CoolTime((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            m_fCurrCool = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(_pPlayer)->Get_GN_Controller())->Get_Skill_CoolDown((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            if (true == static_cast<CPlayer_Gunslinger*>(_pPlayer)->Get_GN_Controller()->Get_StatusEffect_State((_uint)CPlayer_Controller::STATUSEFFECT::SILENCE))
                m_bSilence = true;
            else
                m_bSilence = false;
            Safe_AddRef(_pTexture);
            if (nullptr != _pTexture)
            {
                m_bHaveSkill = true;
                m_pTextureCom_Skill = static_cast<CTexture*>(_pTexture->Clone(nullptr, nullptr));
            }
            Safe_Release(_pTexture);
        }
        else
        {
            m_bHaveSkill = false;
            Safe_Release(m_pTextureCom_Skill);//m_pTextureCom_Skill = nullptr;
        }
    }
}

void CUI_SkillIcon_Frame::Get_Player_WR(CPlayer* _pPlayer, CTexture* _pTexture)
{
    if (nullptr != _pPlayer)
    {
        m_pSkill = static_cast<CPlayer_Slayer*>(_pPlayer)->
            Get_WR_Controller()->Get_PlayerSkill((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
        if (nullptr != m_pSkill)
        {
            _pTexture = (m_pSkill->Get_Skill_Texture());
            m_fCoolMaxTime = static_cast<CController_WR*>(static_cast<CPlayer_Slayer*>(_pPlayer)->Get_WR_Controller())->Get_Skill_CoolTime((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            m_fCurrCool = static_cast<CController_WR*>(static_cast<CPlayer_Slayer*>(_pPlayer)->Get_WR_Controller())->Get_Skill_CoolDown((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            if (true == static_cast<CPlayer_Slayer*>(_pPlayer)->Get_WR_Controller()->Get_StatusEffect_State((_uint)CPlayer_Controller::STATUSEFFECT::SILENCE))
                m_bSilence = true;
            else
                m_bSilence = false;
            Safe_AddRef(_pTexture);
            if (nullptr != _pTexture)
            {
                m_bHaveSkill = true;
                m_pTextureCom_Skill = static_cast<CTexture*>(_pTexture->Clone(nullptr, nullptr));
            }
            Safe_Release(_pTexture);
        }
        else
        {
            m_bHaveSkill = false;
            Safe_Release(m_pTextureCom_Skill);//m_pTextureCom_Skill = nullptr;
        }
    }
}

void CUI_SkillIcon_Frame::Get_Player_WDR(CPlayer* _pPlayer, CTexture* _pTexture)
{
    if (nullptr != _pPlayer)
    {
        m_pSkill = static_cast<CPlayer_Destroyer*>(_pPlayer)->
            Get_WDR_Controller()->Get_PlayerSkill((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
        if (nullptr != m_pSkill)
        {
            _pTexture = (m_pSkill->Get_Skill_Texture());
            m_fCoolMaxTime = static_cast<CController_WDR*>(static_cast<CPlayer_Destroyer*>(_pPlayer)->Get_WDR_Controller())->Get_Skill_CoolTime((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            m_fCurrCool = static_cast<CController_WDR*>(static_cast<CPlayer_Destroyer*>(_pPlayer)->Get_WDR_Controller())->Get_Skill_CoolDown((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            if (true == static_cast<CPlayer_Destroyer*>(_pPlayer)->Get_WDR_Controller()->Get_StatusEffect_State((_uint)CPlayer_Controller::STATUSEFFECT::SILENCE))
                m_bSilence = true;
            else
                m_bSilence = false;
            Safe_AddRef(_pTexture);
            if (nullptr != _pTexture)
            {
                m_bHaveSkill = true;
                m_pTextureCom_Skill = static_cast<CTexture*>(_pTexture->Clone(nullptr, nullptr));
            }
            Safe_Release(_pTexture);
        }
        else
        {
            m_bHaveSkill = false;
            Safe_Release(m_pTextureCom_Skill); //m_pTextureCom_Skill = nullptr;
        }
    }
}

void CUI_SkillIcon_Frame::Get_Player_MG(CPlayer* _pPlayer, CTexture* _pTexture)
{
    if (nullptr != _pPlayer)
    {
        m_pSkill = static_cast<CPlayer_Bard*>(_pPlayer)->
            Get_MG_Controller()->Get_PlayerSkill((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
        if (nullptr != m_pSkill)
        {
            _pTexture = (m_pSkill->Get_Skill_Texture());
            m_fCoolMaxTime = static_cast<CController_MG*>(static_cast<CPlayer_Bard*>(_pPlayer)->Get_MG_Controller())->Get_Skill_CoolTime((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            m_fCurrCool = static_cast<CController_MG*>(static_cast<CPlayer_Bard*>(_pPlayer)->Get_MG_Controller())->Get_Skill_CoolDown((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            if (true == static_cast<CPlayer_Bard*>(_pPlayer)->Get_MG_Controller()->Get_StatusEffect_State((_uint)CPlayer_Controller::STATUSEFFECT::SILENCE))
                m_bSilence = true;
            else
                m_bSilence = false;
            Safe_AddRef(_pTexture);
            if (nullptr != _pTexture)
            {
                m_bHaveSkill = true;
                m_pTextureCom_Skill = static_cast<CTexture*>(_pTexture->Clone(nullptr, nullptr));
            }
            Safe_Release(_pTexture);
        }
        else
        {
            m_bHaveSkill = false;
            Safe_Release(m_pTextureCom_Skill);
        }
    }
}

void CUI_SkillIcon_Frame::Get_Player_SP(CPlayer* _pPlayer, CTexture* _pTexture)
{
    if (nullptr != _pPlayer)
    {
        m_pSkill = static_cast<CPlayer_Doaga*>(_pPlayer)->
            Get_SP_Controller()->Get_PlayerSkill((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
        if (nullptr != m_pSkill)
        {
            _pTexture = (m_pSkill->Get_Skill_Texture());
            m_fCoolMaxTime = static_cast<CController_SP*>(static_cast<CPlayer_Doaga*>(_pPlayer)->Get_SP_Controller())->Get_Skill_CoolTime((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            m_fCurrCool = static_cast<CController_SP*>(static_cast<CPlayer_Doaga*>(_pPlayer)->Get_SP_Controller())->Get_Skill_CoolDown((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            if (true == static_cast<CPlayer_Doaga*>(_pPlayer)->Get_SP_Controller()->Get_StatusEffect_State((_uint)CPlayer_Controller::STATUSEFFECT::SILENCE))
                m_bSilence = true;
            else
                m_bSilence = false;
            Safe_AddRef(_pTexture);
            if (nullptr != _pTexture)
            {
                m_bHaveSkill = true;
                m_pTextureCom_Skill = static_cast<CTexture*>(_pTexture->Clone(nullptr, nullptr));
            }
            Safe_Release(_pTexture);
        }
        else
        {
            m_bHaveSkill = false;
            Safe_Release(m_pTextureCom_Skill);
        }
    }
}

HRESULT CUI_SkillIcon_Frame::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Empty"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Shine"),
        TEXT("Com_TextureShine"), (CComponent**)&m_pTextureCom_Shine)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Frame"),
        TEXT("Com_TextureFrame"), (CComponent**)&m_pTextureCom_Frame)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources()
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

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_Picked()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vecPickedColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCoolAngle, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_Shine()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlphaShine, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_Skill()
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

    if((TEXT("Fall") == CServerSessionManager::GetInstance()->Get_Player()->Get_State()) ||(TEXT("Dead_Start") == CServerSessionManager::GetInstance()->Get_Player()->Get_State()) || (TEXT("Dead_End") == CServerSessionManager::GetInstance()->Get_Player()->Get_State()))
        m_bSilence = true;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Silence", &m_bSilence, sizeof(_bool))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCoolAngle, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_ChangeFrame()
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

void CUI_SkillIcon_Frame::Print_CoolTime()
{
    if ((nullptr == m_pCoolTimetWnd))
        return;

    m_pCoolTimetWnd->Set_Active(true);
    m_pCoolTimetWnd->Clear_Text();
    m_pCoolTimetWnd->Set_Alpha(1.f);

    Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
    m_pCoolTimetWnd->Get_TransformCom()->Set_Scale(Vec3(22.f, 22.f, 0.f));
    m_pCoolTimetWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

    Set_StringCoolTime();
    Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"���ǰ����", m_strCoolTime);
    Vec2 vOrigin = vMeasure * 0.5f;
    
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-1"), m_szFont, m_strCoolTime, Vec2(22.f -1.f, 22.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-2"), m_szFont, m_strCoolTime, Vec2(22.f + 1.f, 22.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-3"), m_szFont, m_strCoolTime, Vec2(22.f, 22.f -1.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    m_pCoolTimetWnd->Set_Text(m_strWndName + TEXT("-4"), m_szFont, m_strCoolTime, Vec2(22.f, 22.f + 1.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(0.f, 0.f, 0.f, 1.f));
    
    m_pCoolTimetWnd->Set_Text(m_strWndName, m_szFont, m_strCoolTime, Vec2(22.f, 22.f), Vec2(0.8f, 0.8f), vOrigin, 0.f, Vec4(1.f, 1.f, 1.f, 1.f));
}

void CUI_SkillIcon_Frame::Set_Active(_bool bActive)
{
    m_pCoolTimetWnd->Set_Active(bActive);
}

HRESULT CUI_SkillIcon_Frame::Ready_TextBox()
{
    CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
    Safe_AddRef(pGameInstance);
    if (nullptr == m_pCoolTimetWnd)
    {
        CTextBox::TEXTBOXDESC tTextDesc;
        tTextDesc.szTextBoxTag = TEXT("SkillCoolWnd");
        tTextDesc.szTextBoxTag += m_eSkillKey;
        m_strWndName = tTextDesc.szTextBoxTag;
        tTextDesc.vSize = Vec2(44.f, 44.0f);
        m_pCoolTimetWnd = static_cast<CTextBox*>(pGameInstance->
            Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

        if (nullptr == m_pCoolTimetWnd)
        {
            Safe_Release(pGameInstance);
            return E_FAIL;
        }

        m_pCoolTimetWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
        m_pCoolTimetWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
        m_pCoolTimetWnd->Set_Render(false);
    }
    Safe_Release(pGameInstance);
    return S_OK;
}

void CUI_SkillIcon_Frame::Start_CoolTimeText()
{
    m_strCoolTime.clear();
    m_pCoolTimetWnd->Set_Active(true);
}

void CUI_SkillIcon_Frame::End_CoolTimeText()
{
    m_strCoolTime.clear();
    m_pCoolTimetWnd->Set_Active(false);
}

void CUI_SkillIcon_Frame::Set_StringCoolTime()
{
    wstring strCool = to_wstring((_uint)m_fResultCool);
    m_strCoolTime.clear();
    m_strCoolTime = strCool + TEXT("s");
}

CUI_SkillIcon_Frame* CUI_SkillIcon_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_SkillIcon_Frame* pInstance = new CUI_SkillIcon_Frame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_SkillIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_SkillIcon_Frame::Clone(void* pArg)
{
    CUI_SkillIcon_Frame* pInstance = new CUI_SkillIcon_Frame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_SkillIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_SkillIcon_Frame::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pCoolTimetWnd->Set_Dead(true);
    Safe_Release(m_pTextureCom_Skill);//�׽�Ʈ��
    Safe_Release(m_pTextureCom_Shine);//���콺 �ø��� ������ ����Ʈ
    Safe_Release(m_pTextureCom_Frame);//������
}
