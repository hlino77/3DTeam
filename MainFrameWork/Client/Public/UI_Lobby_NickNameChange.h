#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)

class CUI_Lobby_NickNameChange final:
    public CUI
{
public:
    struct LOBBYSTAGE_DESC {
        CUI* pUI;
        _uint iIndex;
    };

private:
    CUI_Lobby_NickNameChange(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Lobby_NickNameChange(const CUI& rhs);
    virtual ~CUI_Lobby_NickNameChange() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_NickNmaeFrame(CUI* pUI) { m_pNickName = pUI; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    virtual HRESULT Bind_ShaderResources_NewWnd();
    void    Render_InputString();

private:
    void    Update_NickNameFrame();
    void    Update_NickNameChangeWnd();

private:
    _uint   m_iCharacterIndex = { 0 };
    CUI*     m_pNickName = { nullptr };

    //new wnd
    CTexture* m_pTextureCom_NewWnd = { nullptr };
    CTransform* m_pTransformCom_NewWnd = { nullptr };
    _bool       m_bOnWnd = { false };

public:
    static  CUI_Lobby_NickNameChange* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END