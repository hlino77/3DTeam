#pragma once
#include "Skill.h"
#include "Client_Defines.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Client)
class CSkill_Crystal :
	public CSkill
{

private:
	CSkill_Crystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Crystal(const CSkill& rhs);
	virtual ~CSkill_Crystal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;
	virtual void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)override;
public:
	void					Set_Explosion(_bool bExplosion);
	_bool					Get_Explosion() { return m_bExplosion; }
	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }
	_bool					Get_RimLight() { return m_bRimLight; }
private:
	virtual HRESULT		Ready_Components();
	// CSkill��(��) ���� ��ӵ�
	virtual void Explosion() override;
	virtual HRESULT Ready_Coliders() override;
private:
	_float							m_fCellHeight = 0.f;
	_bool							m_bExplosion = false;
	_bool							m_bWarning = false;
	_float							m_fExplosionDelay = 0.f;
	_bool							m_IsSetuponCell = false;
	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

	_bool							m_bExplosionEffect = false;

public:
	static CSkill_Crystal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

