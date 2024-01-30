#pragma once
#include "Player.h"

BEGIN(Client)

class CLineCircle;

class CPlayer_Doaga : public CPlayer
{
public:

private:
	CPlayer_Doaga(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Doaga(const CPlayer_Doaga& rhs);
	virtual ~CPlayer_Doaga() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT	Render_ShadowDepth();

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	virtual	void	OnCollisionEnter_NoneControl(const _uint iColLayer, class CCollider* pOther);
	virtual	void	OnCollisionExit_NoneControl(const _uint iColLayer, class CCollider* pOther);
	virtual void	Set_Skill(CGameObject* pGameObject) override;

public:
	void			Send_PlayerInfo();
	void			Set_Colliders(_float fTimeDelta);

	virtual _bool	Get_CellPickingPos(Vec3& vPickPos);

public:
	class CController_SP* Get_SP_Controller() { return m_pController; }

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Parts() override;
	virtual HRESULT Ready_Coliders() override;

	HRESULT			Ready_State();
	HRESULT			Ready_Skill();
	HRESULT			Ready_PhysxBoneBranch();
	HRESULT			Ready_Item();
	HRESULT			Ready_Esther();

private:
	HRESULT			Ready_SkillUI();


private:
	_float					m_fSendInfoTime = 0.0f;

	class CController_SP* m_pController = { nullptr };
	


public:
	static CPlayer_Doaga* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END