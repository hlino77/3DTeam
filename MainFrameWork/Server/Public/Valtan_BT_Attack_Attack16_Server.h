#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_Attack16_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack16_Server();
    CValtan_BT_Attack_Attack16_Server(const CValtan_BT_Attack_Attack16_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack16_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
    virtual void On_FirstAnimStart()override;
    virtual void On_LastAnimEnd()override;
private:
    Vec3                m_vLandPosition = {};

public:
    static	CValtan_BT_Attack_Attack16_Server* Create(void* pArg);
    virtual void Free() override;
};

END