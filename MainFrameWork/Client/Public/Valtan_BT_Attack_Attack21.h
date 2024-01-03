#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack21 :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack21();
    CValtan_BT_Attack_Attack21(const CValtan_BT_Attack_Attack21& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack21() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CValtan_BT_Attack_Attack21* Create(void* pArg);
    virtual void Free() override;
};

END