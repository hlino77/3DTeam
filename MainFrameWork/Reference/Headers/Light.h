#pragma once

#include "Base.h"

BEGIN(Engine)
/* �� ������ �����Ѵ�. */
class CTexture;

class CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	const LIGHTDESC* Get_LightDesc() const {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHTDESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	void	Set_StaticShadowMap(CTexture* pTexture) { m_pStaticShadowMap = pTexture; }


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	LIGHTDESC				m_LightDesc;

	CTexture*				m_pStaticShadowMap = nullptr;
public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END