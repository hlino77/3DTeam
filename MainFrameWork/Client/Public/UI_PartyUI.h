#pragma once
#include "UI.h"

BEGIN(Engine)
class CTextBox;
END

BEGIN(Client)
class CPlayer;
class CUI_PartyEntrance;
class CUI_PartyUI :
    public CUI
{
public:
    struct INVITEPARTY_DESC
    {
        class CPlayer* pPlayer;
        _bool   bInvite;
    };

private:
    CUI_PartyUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_PartyUI(const CUI& rhs);
    virtual ~CUI_PartyUI() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    HRESULT Initialize_TextBox(class CPlayer* pOwner);
    HRESULT Initialize_Get_Players();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

private:
    void    Update_PartInfo();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Print_Text();
    void    Set_Active_EntranceParty(CPlayer* pPartyLeader, CPlayer* pPlayer);
    void    Add_PartyInfo(vector<_uint>vecPlayersId);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    HRESULT UI_Set();
    HRESULT Ready_NameTextBox(CPlayer* pOwner);

private:
    //기본 텍스처클래스와 트랜스폼클래스는 메인파티Wnd 텍스처 사용
    CUI_PartyEntrance* m_pUI_PartyEntrance = { nullptr };
    _uint   m_iPartyIndex = { 0 };
    CTextBox* m_pTextBox = { nullptr };
    wstring m_strWndTag;
    wstring m_strFont;

public:
    static  CUI_PartyUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END