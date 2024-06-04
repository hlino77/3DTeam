#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CCommon_BT_Dead_Server :
    public CBT_Action
{
private:
    CCommon_BT_Dead_Server();
    CCommon_BT_Dead_Server(const CCommon_BT_Dead_Server& rhs) = delete;
    virtual ~CCommon_BT_Dead_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Dead_Server* Create(void* pArg);
    virtual void Free() override;
};

END