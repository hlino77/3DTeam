#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CGolem_BT_Attack_Charge_Punch :
    public CBT_Action
{
private:
    CGolem_BT_Attack_Charge_Punch();
    CGolem_BT_Attack_Charge_Punch(const CGolem_BT_Attack_Charge_Punch& rhs) = delete;
    virtual ~CGolem_BT_Attack_Charge_Punch() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

public:
    _bool 	m_Shoot = true;
public:
    static	CGolem_BT_Attack_Charge_Punch* Create(void* pArg);
    virtual void Free() override;
};

END