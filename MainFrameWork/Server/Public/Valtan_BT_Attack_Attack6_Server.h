#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_Attack6_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack6_Server();
    CValtan_BT_Attack_Attack6_Server(const CValtan_BT_Attack_Attack6_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack6_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CValtan_BT_Attack_Attack6_Server* Create(void* pArg);
    virtual void Free() override;
};

END