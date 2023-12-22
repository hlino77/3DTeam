#pragma once
#include "GameObject.h"
#include <random>

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CEffect : public CGameObject
{

protected:
	/* ������ ������ �� */
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻�� ������ �� */
	CEffect(const CEffect& rhs); /* ���� ������. */
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() PURE;
	
protected: /* �ش� ��ü�� ����ؾ��� ������Ʈ���� �����ϳ�. */
	CTexture* m_pTextureCom = { nullptr };

	_float					m_fAlpha = 1.0f;

protected:
	random_device*		m_RandomDevice = nullptr;

protected:
	virtual HRESULT Ready_Components();

public:
	virtual void Free();
};

END