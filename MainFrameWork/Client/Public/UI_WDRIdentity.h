#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WDRIdentity final:
    public CUI
{
private:
    CUI_WDRIdentity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity(const CUI& rhs);
    virtual ~CUI_WDRIdentity() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    HRESULT	UI_Set();
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

public:
    static  CUI_WDRIdentity* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END