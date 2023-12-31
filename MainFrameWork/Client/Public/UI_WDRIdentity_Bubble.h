#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WDRIdentity_Bubble final:
    public CUI
{
private:
    CUI_WDRIdentity_Bubble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WDRIdentity_Bubble(const CUI& rhs);
    virtual ~CUI_WDRIdentity_Bubble() = default;

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
    void Update_Bubbles(_float fTimeDelta);
    void Spawan_BubbleAnim(_uint iBubbleCount , _float fTimeDelta);
    void Decrease_Bubble(_float fTimeDelta);

private:
    CGameObject* m_pPlayer = { nullptr };
    _bool   m_bSpawnBubbleAnim = { false };
    _bool   m_bDecrease = { false };

    _uint   m_iBubbleCurrCount = { 0 };
    _uint   m_iBubblePreCount = { 0 };
    _uint   m_iBubbleIndex = { 0 };

    Vec4    m_vMaxColor = { 10.f, 10.f, 10.f, 10.f };

public:
    static  CUI_WDRIdentity_Bubble* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END