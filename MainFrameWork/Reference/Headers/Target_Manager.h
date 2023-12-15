#pragma once

/* ����Ÿ�ٵ��� �����ؼ� ��Ƶд�. */
#include "Base.h"
#include "Hasher.h"

BEGIN(Engine)

class ENGINE_DLL CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const Vec4& vColor);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT Bind_SRV(class CShader* pShader, const wstring& strTargetTag, const _char* pConstantName);
	HRESULT Make_SRVTexture(const wstring& szPath, const wstring& strTargetTag);
	HRESULT	Copy_SRV(const wstring& strTargetTag, ID3D11ShaderResourceView** pSRV);
	HRESULT Clear_RenderTarget(const wstring& strTargetTag);


	/* strMRTTag�� �ش��ϴ� list�� ����ִ� Ÿ�ٵ��� ��ġ�� ���ε��Ѵ�. */
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag);
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, ID3D11DepthStencilView* pDSV);

	/* �ٽ� ���� ���·� �����Ѵ�. */
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

public:
	HRESULT Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render(const wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


private:

	unordered_map<const wstring, class CRenderTarget*, djb2Hasher>			m_RenderTargets;

	/* ��ġ�� ���ÿ� ���ε��Ǿ���ϴ� Ÿ�ٵ��� �̸� ����ΰڴ�. */
	unordered_map<const wstring, vector<class CRenderTarget*>, djb2Hasher>		m_MRTs;

private:
	ID3D11RenderTargetView*					m_pBackBufferRTV = { nullptr };
	ID3D11DepthStencilView*					m_pDSV = { nullptr };

private:
	class CRenderTarget* Find_RenderTarget(const wstring& strTargetTag);
	vector<class CRenderTarget*>* Find_MRT(const wstring & strMRTTag);

public:
	virtual void Free() override;
};

END