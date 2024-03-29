#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "PartObject.h"
#include "Projectile.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CPlayer;
class CEsther;

class CEsther_Way_Dochul final : public CGameObject
{
private:
	CEsther_Way_Dochul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Way_Dochul(const CEsther_Way_Dochul& rhs);
	virtual ~CEsther_Way_Dochul() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT         Render_Outline();

public:

	CShader*				Get_ShaderCom() { return m_pShaderCom; }

	void					Set_AnimationSpeed(_float fSpeed) { m_fAnimationSpeed = fSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

public:
	void					Set_DebugRender_State(_bool bRender) { m_bDebugRender = bRender; }

	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bRootRot = false, _bool bReverse = false, _bool bUseY = false);

	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

	virtual void			Call_Act1(_float fTimeDelta) { Act1(fTimeDelta); }
	virtual void			Call_Act2(_float fTimeDelta) { Act2(fTimeDelta); }
	virtual void			Call_Act3(_float fTimeDelta) { Act3(fTimeDelta); }

	void					Set_Sound_Init(_bool bSound) { m_bSound1 = bSound, m_bSound2 = bSound, m_bSound3 = bSound; }

public:
	virtual HRESULT			Ready_Coliders() { return S_OK; }

private:
	virtual HRESULT			Ready_Components();

	void					CullingObject();

	void					Check_Finish();
	void					Act1(_float fTimeDelta);
	void					Act2(_float fTimeDelta);
	void					Act3(_float fTimeDelta);

protected:
	_bool							m_bDebugRender = { false };

	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

private:
	_int							m_iAnimIndex = { 0 };
	_int							m_iAnimAct1 = { 0 };
	_int							m_iAnimAct2 = { 0 };
	_int							m_iAnimAct3 = { 0 };

	vector<PROJECTILE_DESC>			m_vecSkillProjDesces;

	/* 컬링 절두체 */
	BoundingSphere	m_tCullingSphere;

	_bool			m_IsDissolve = { false };
	_float			m_fDissolveAcc = { 0.0f };
	_float			m_fMaxDissolve = { 1.f };

	_float			m_fRimLightColor = { 0.95f };

	class CTexture* m_pDissolveTexture = { nullptr };


	_bool			m_bSound1 = false;
	_bool			m_bSound2 = false;
	_bool			m_bSound3 = false;

public:
	static CEsther_Way_Dochul* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();
};

END

