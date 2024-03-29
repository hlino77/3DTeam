#pragma once
#include "Esther_Cut.h"

BEGIN(Client)

class CEsther;
class CPlayer;

class CEsther_Way_Cut final : public CEsther_Cut
{
private:
	CEsther_Way_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Way_Cut(const CEsther_Way_Cut& rhs);
	virtual ~CEsther_Way_Cut() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual void			Reset() override;
	virtual void			Ready() override;
	virtual void			Check_Finish() override;

private:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_ModelPart();
	virtual HRESULT			Ready_Parts();
	virtual HRESULT			Ready_CutCamera();

private:
	void					Act1(_float fTimeDelta);
	void					Act2(_float fTimeDelta);
	void					Act3(_float fTimeDelta);

private:
	_uint					m_iAnimIndex;
	_bool					m_bLerpActive = false;


public:
	static CEsther_Way_Cut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END

