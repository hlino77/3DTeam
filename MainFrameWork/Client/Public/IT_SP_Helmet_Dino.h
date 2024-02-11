#pragma once

#include "Client_Defines.h"
#include "Item.h"

BEGIN(Client)

class CIT_SP_Helmet_Dino final : public CItem
{
private:
	CIT_SP_Helmet_Dino(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIT_SP_Helmet_Dino(const CIT_SP_Helmet_Dino& rhs);
	virtual ~CIT_SP_Helmet_Dino() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Tick(_float fTimeDelta);
	virtual void				LateTick(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	virtual HRESULT				Use_Item(CPlayer* pOwner) override;
	virtual HRESULT				Disuse_Item(CPlayer* pOwner, _bool bUseDefault) override;

	virtual void				Upgrade_Item() override;

private:
	virtual HRESULT				Ready_Components();

public:
	static CIT_SP_Helmet_Dino* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

