#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

class CPlayer_Controller_GN final : public CPlayer_Controller
{
public:
	enum GN_IDENTITY { HAND, SHOT, LONG, _END };

private:
	CPlayer_Controller_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Controller_GN(const CPlayer_Controller_GN& rhs);
	virtual ~CPlayer_Controller_GN() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;

public:
	virtual _bool Is_Idle();

	_uint	Is_GN_Identity();

public:
	void	Get_GN_IdentityMessage(_uint iIndex) { GN_Identity(iIndex); }

public:
	GN_IDENTITY		Get_GN_Identity() { return m_eIdentity; }
	GN_IDENTITY		Get_GN_PreIdentity() { return m_ePreIdentity; }

private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack() override;
	virtual void	Skill() override;
	virtual void	Hit() override;
	void			GN_Identity(_uint index);

private:
	GN_IDENTITY		m_eIdentity = { GN_IDENTITY::HAND };
	GN_IDENTITY		m_ePreIdentity = { GN_IDENTITY::_END };

public:
	static CPlayer_Controller_GN* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END