#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN_Spark final:
    public CUI
{
private:
    CUI_IdentityGN_Spark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_IdentityGN_Spark(const CUI& rhs);
    virtual ~CUI_IdentityGN_Spark() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    _bool   m_bHidden = { false };
    _bool   m_bTapKey = { false };
    CUI* m_pMainFrame = { nullptr };
    Vec2    m_vecSizeOrigin = { 128.f, 88.f };
public:
    static  CUI_IdentityGN_Spark* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END