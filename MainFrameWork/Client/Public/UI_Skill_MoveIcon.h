#pragma once
#include "UI.h"

BEGIN(Engine)
class CPlayer_Skill;
END

BEGIN(Client)

class CUI_SkillI_MoveFrame :
    public CUI
{
    enum MOUSE_MOD
    {
        MOUSE_NORMAL, MOUSE_TOOLTIP, MOUSE_SWAP_SKILL, MOUSE_SWAP_ITEM, MOUSE_SWAP_EMOTION, MOUSE_END
    };

private:
    CUI_SkillI_MoveFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_SkillI_MoveFrame(const CUI& rhs);
    virtual ~CUI_SkillI_MoveFrame() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}
    void    Set_IsMove();
    void    Set_PreIconFrame(CUI* pSkillFrame) { m_pPreSkillFrame = pSkillFrame; }
    void    Set_PickedIconFrame(CUI* pSkillFrame);
    void    Set_CurrIconFrame(CUI* pSkillFrame);
    _uint   Get_SwapMod() { return m_eMouseMode; }
    void    Set_SwapMod(_uint eMouseMode) { m_eMouseMode = (MOUSE_MOD)eMouseMode; }

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    MOUSE_MOD   m_eMouseMode = { MOUSE_NORMAL };
    _bool   m_bPicked = { false };
    _bool   m_bMoveIcon = { false };
   CUI* m_pPreSkillFrame = {nullptr};
   CUI* m_pPickedSkillFrame = { nullptr };
   CUI* m_pCurrSkillFrame = { nullptr };
   CTexture* m_pSkillIcon = { nullptr };

public:
    static  CUI_SkillI_MoveFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END