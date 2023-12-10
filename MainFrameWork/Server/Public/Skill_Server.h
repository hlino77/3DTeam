#pragma once

#include "Server_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Server)
class CSkill_Server : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
		CGameObject* pSkillOwner;
	}MODELDESC;


protected:
	CSkill_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Server(const CSkill_Server& rhs);
	virtual ~CSkill_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() = 0;


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther);
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther);
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther);
	void			Send_Collision(_uint iColLayer, CCollider* pOther, _bool bEnter);


	virtual void	Explosion() PURE;

	void			Send_Explosion();

	virtual HRESULT					Ready_Coliders() = 0;
public:


	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }
	

	CGameObject*			Get_SkillOwner() { return m_pSkillOwner; }

public:


	void					Find_NearTarget();

	void					Send_NearTarget();

	_float					Get_NearTargetDistance();

	void					Send_ColliderState(const _uint& iLayer);

	void					Set_Colliders(_float fTimeDelta);



	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void					Set_Die();


	
protected:
	virtual HRESULT			Ready_Components();



protected:
	CRenderer*						m_pRendererCom = nullptr;

	_float							m_fMoveSpeed = 0.0f;



	_float							m_fFollowDistance = 0.0f;

	CGameObject*					m_pSkillOwner = nullptr;
protected: /* �ش� ��ü�� ����ؾ��� ������Ʈ���� �����ϳ�. */


public:
	virtual void Free();


};

END

