#pragma once
#include "Server_Defines.h"
#include "Skill_Server.h"

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Server)
class CSkill_RisingSun_Server : public CSkill_Server
{
public:
	struct RisingSunDesc
	{
		CGameObject* pSkillOwner = nullptr;
		_int	iObjectID = -1;
	};

protected:
	CSkill_RisingSun_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_RisingSun_Server(const CSkill_RisingSun_Server& rhs);
	virtual ~CSkill_RisingSun_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	virtual void	Explosion() override;
	virtual void	Buff_Player(_uint iPlayerID) override;
	virtual HRESULT					Ready_Coliders() override { return S_OK; }
protected:
	virtual HRESULT			Ready_Components();

private:
	_bool m_bEnd = false;

	_float m_fDelay = 0.0f;

public:
	virtual void Free();
	static CSkill_RisingSun_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);

};

END

