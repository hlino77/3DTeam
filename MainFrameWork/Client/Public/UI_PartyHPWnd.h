#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END


BEGIN(Client)
class CPlayer;
class CUI_PartyHPWnd :
    public CUI
{
public:
    struct PARTYHP_DESC
    {
        class CPlayer* pPlayer = nullptr;
        wstring strObjectTag = TEXT("");
        _uint   iPartyIndex;
    }; 

private:
    CUI_PartyHPWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_PartyHPWnd(const CUI& rhs);
    virtual ~CUI_PartyHPWnd() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox( );
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_PlayerDesc(PARTYHP_DESC tDesc);
    void    Set_PartyLeader(_bool bLeader) { m_bPartyLeader = bLeader; }

private:
    void    Update_PlayeHp();
    void    Print_NickName();

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_HpFill();
    virtual HRESULT Bind_ShaderResources_Emblem();
    virtual HRESULT Bind_ShaderResources_DeathMark();
    virtual HRESULT Bind_ShaderResources_Num();
    virtual HRESULT Bind_ShaderResources_LeaderCrown();
    HRESULT Ready_NameTextBox();

private:
    CPlayer* m_pPartyPlayer = { nullptr };

    CTexture* m_pTexture_PartyEmblem = { nullptr };
    CTransform* m_pTransform_PartyEmblem = { nullptr };

    CTexture* m_pTexture_PartyHPFill = { nullptr };
    CTransform* m_pTransform_PartyHP = { nullptr };

    CTexture* m_pTexture_LeaderCrown = { nullptr };
    CTransform* m_pTransform_LeaderCrown = { nullptr };

    CTexture* m_pTexture_LNum = { nullptr };
    CTexture* m_pTexture_DNum = { nullptr };
    CTransform* m_pTransform_Num = { nullptr };

    CTexture* m_pTexture_DeathMark = { nullptr };
    CTransform* m_pTransform_DeathMark = { nullptr };

    _uint   m_iEmblemIndex = { 0 };
    _uint   m_iNumIndex = { 0 };
    _uint   m_iNumTextureIndex = { 0 };

    _float  m_fHpWndAlpha = { 0.f };
    _float  m_fX_Emblem = { 0.f };
    _float  m_fX_Num = { 0.f };
    _float  m_fMaxHp = { 0.f };
    _float  m_fCurrHp = { 0.f };
    _float  m_fHpRatio = { 0.f };
    _float  m_fDeathMarkAlpha = { 0.f };

    CTextBox* m_pTextBox = { nullptr };
    wstring m_strWndTag;
    wstring m_strFont;
    wstring m_strName = TEXT("");

    _bool   m_bPartyLeader = { false };

    Vec4    m_vTextColor = { 1.f, 1.f, 1.f, 1.f };
    
    _bool   m_bReNewName = { false };
    wstring m_strObjectTag = TEXT("");
public:
    static  CUI_PartyHPWnd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END