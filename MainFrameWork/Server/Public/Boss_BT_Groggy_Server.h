#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CBoss_BT_Groggy_Server :
    public CBT_Action
{
private:
    CBoss_BT_Groggy_Server();
    CBoss_BT_Groggy_Server(const CBoss_BT_Groggy_Server& rhs) = delete;
    virtual ~CBoss_BT_Groggy_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CBoss_BT_Groggy_Server* Create(void* pArg);
    virtual void Free() override;
};

END