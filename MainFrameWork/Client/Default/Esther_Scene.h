#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEsther_Scene final : public CGameObject
{
public:
	typedef struct tagEsSecneDesc
	{
		wstring strTexutre;
		_float fStartFrame;
		_float fFinalFrame;
		_float fFrameSpeed;
	}ESTHERSCENEDESC;

private:
	CEsther_Scene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Scene(const CGameObject& rhs); 

	virtual ~CEsther_Scene() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_bool	Is_PlayFrame() { return m_bPlayFrame; }

	void	Play_Frame();
	void	Stop_Frame() { m_bPlayFrame = false; }

private: /* �ش� ��ü�� ����ؾ��� ������Ʈ���� �����ϳ�. */
	CRenderer* m_pRendererCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };

private:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	Matrix					m_ViewMatrix, m_ProjMatrix;
	_float					m_fFrame = { 0.f };
	_bool					m_IsFrameEnd = { false };

private:
	_bool					m_bPlayFrame = { false };

	wstring					m_strTexutre;
	_float					m_fStartFrame;
	_float					m_fFinalFrame;
	_float					m_fFrameSpeed;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CEsther_Scene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* �纻��ü�� �����Ҷ� ���������ͷκ��� �����ؿ� �����Ϳܿ� �� �߰��� ����� �����Ͱ� �ִٶ�� �޾ƿ��ڴ�. */
	virtual void Free() override;
};

END