#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CZombie_BT_BattleIdle_Server :
    public CBT_Action
{
private:
    CZombie_BT_BattleIdle_Server();
    CZombie_BT_BattleIdle_Server(const CZombie_BT_BattleIdle_Server& rhs) = delete;
    virtual ~CZombie_BT_BattleIdle_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_BattleIdle_Server* Create(void* pArg);
    virtual void Free() override;
};

END