#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;

class CValtan_BT_Attack_CounterAttack :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_CounterAttack();
    CValtan_BT_Attack_CounterAttack(const CValtan_BT_Attack_CounterAttack& rhs) = delete;
    virtual ~CValtan_BT_Attack_CounterAttack() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


    vector<CEffect*> m_Particles;
    _bool m_bParticle = false;

    _bool m_bAttack = false;
public:
    static	CValtan_BT_Attack_CounterAttack* Create(void* pArg);
    virtual void Free() override;
};

END