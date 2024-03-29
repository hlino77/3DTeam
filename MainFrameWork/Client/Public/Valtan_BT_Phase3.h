#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Phase3 :
    public CBT_Action
{
private:
    CValtan_BT_Phase3();
    CValtan_BT_Phase3(const CValtan_BT_Phase3& rhs) = delete;
    virtual ~CValtan_BT_Phase3() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
    virtual void On_FirstAnimStart()override;

public:
    static	CValtan_BT_Phase3* Create(void* pArg);
    virtual void Free() override;
};

END