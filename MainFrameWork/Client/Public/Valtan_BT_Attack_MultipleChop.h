#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_MultipleChop :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_MultipleChop();
    CValtan_BT_Attack_MultipleChop(const CValtan_BT_Attack_MultipleChop& rhs) = delete;
    virtual ~CValtan_BT_Attack_MultipleChop() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

public:
    virtual void        On_FirstAnimStart()    override;

private:
    _bool m_bShoot[4] = {};
    _bool m_bEffectStart[4] = { false, false, false, false };

public:
    static	CValtan_BT_Attack_MultipleChop* Create(void* pArg);
    virtual void Free() override;
};

END