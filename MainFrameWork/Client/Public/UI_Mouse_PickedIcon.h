#pragma once
#include "UI.h"

BEGIN(Engine)
class CPlayer_Skill;
END

BEGIN(Client)
class CItem;
class CPlayer;

class CUI_Mouse_PickedIcon :
    public CUI
{
private:
    CUI_Mouse_PickedIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Mouse_PickedIcon(const CUI& rhs);
    virtual ~CUI_Mouse_PickedIcon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_IsPicked(_bool IsPicked) { m_bPicked = IsPicked; }
    _bool   Get_IsPicked() { return m_bPicked; }
    void    Set_IconTexture(CTexture* pTextureCom, _uint iTextureIndex);
    void    Set_SkillIconTexture(CTexture* pTextureCom);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();
    void    UnPickedIcon();

private:
    CTexture* m_pTexture_Grade = { nullptr };
    _bool   m_bPicked = { false };
    _bool   m_bItem = { false };

public:
    static  CUI_Mouse_PickedIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END