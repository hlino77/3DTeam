#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_BattleIdle_Server :
    public CBT_Action
{
private:
    CValtan_BT_BattleIdle_Server();
    CValtan_BT_BattleIdle_Server(const CValtan_BT_BattleIdle_Server& rhs) = delete;
    virtual ~CValtan_BT_BattleIdle_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _float                 m_fIdletime   = 0.f;

public:
    static	CValtan_BT_BattleIdle_Server* Create(void* pArg);
    virtual void Free() override;
};

END