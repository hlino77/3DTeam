#pragma once
#include "Npc.h"

BEGIN(Client)

class CPlayer;

class CFunction_Npc abstract : public CNpc
{

protected:
	CFunction_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFunction_Npc(const CFunction_Npc& rhs);
	virtual ~CFunction_Npc() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();

public:
	virtual void			Activate_Function() {};

protected:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_Parts();

	virtual HRESULT			Render_Model();
	virtual HRESULT			Render_Model_Shadow();
	virtual HRESULT			Render_PartModel();
	virtual HRESULT			Render_PartModel_Shadow();

	void					Set_Colliders(_float fTimeDelta);

protected:
	_bool					m_bCallFunctionFirst = { false };


public:
	virtual void Free();
};

END

