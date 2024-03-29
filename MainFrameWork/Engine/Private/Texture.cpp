#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{

}

CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)
	, m_iNumTextures(rhs.m_iNumTextures)
	, m_SRVs(rhs.m_SRVs)
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		_tchar			szFullPath[MAX_PATH] = TEXT("");

		wsprintf(szFullPath, strTextureFilePath.c_str(), i);

		_tchar			szDrive[MAX_PATH] = TEXT("");
		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szFullPath, szDrive, MAX_PATH, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT hr = 0;

		ID3D11ShaderResourceView* pSRV = nullptr;

		if (false == lstrcmp(TEXT(".dds"), szExt))
			hr = DirectX::CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);

		else if (false == lstrcmp(TEXT(".tga"), szExt))
			hr = E_FAIL;

		else
			hr = DirectX::CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

ID3D11ShaderResourceView* CTexture::Get_SRV(_uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return nullptr;

	return m_SRVs[iTextureIndex];
}

HRESULT CTexture::Set_SRV(CShader* pShader, const char* strConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Bind_Texture(strConstantName, m_SRVs[iTextureIndex]);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strTextureFilePath, iNumTextures)))
	{
		wstring TextureFrontPath = L"../Bin/Resources/Export/Texture/";

		_tchar			szFileName[MAX_PATH] = TEXT("");
		_wsplitpath_s(strTextureFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

		wstring FullPath = TextureFrontPath + szFileName + L".png";

		if (FAILED(pInstance->Initialize_Prototype(FullPath, iNumTextures)))
		{
			wstring strMessage = L"Failed To Created Texture : ";
			strMessage += strTextureFilePath;
			MessageBoxW(nullptr, strMessage.c_str(), L"Fail", MB_OK);
			Safe_Release(pInstance);
		}
	}

	return pInstance;
}

CComponent* CTexture::Clone(CGameObject* pObject, void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTexture");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();
}
