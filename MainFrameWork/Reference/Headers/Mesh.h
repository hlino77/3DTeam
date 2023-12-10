#pragma once


#include "VIBuffer.h"
#include "Model.h"
#include "AsFileUtils.h"

/* ���� �����ϴ� �ϳ��� �޽�. */
/* �� �޽ø� �����ϴ� ����, �ε��� ���۸� �����Ѵ�. */

BEGIN(Engine)


class CMesh final : public CVIBuffer
{

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(class CModel* pModel, Matrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);	

public:
	HRESULT	LoadData_FromMeshFile(CModel::TYPE eModelType, CAsFileUtils* pFileUtils, Matrix PivotMatrix, _bool bColMesh);
	HRESULT	LoadData_FromConverter(CModel::TYPE eModelType, shared_ptr<asMesh> pMesh, Matrix PivotMatrix);


	VTXANIMMODEL* Get_Vertices() { return m_pVertices; }
	FACEINDICES32* Get_Indices() { return m_pIndices; }
private:
	//char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = 0;
private:
	/* �� �޽ÿ� ���⤷�� �ִ� ���� ����. */
	_uint							m_iNumBones = 0;

	/* �� �޽ÿ� ������ �ִ� ������ ��Ƴ�����. */
	/* why ? : �޽� ���� �������� �� �� �޽ÿ� ���⤷�� �ִ� ������ ����� ��Ƽ� ��{���𷯤� ���ä�������. */

	_uint							m_iBoneIndex;
	wstring							m_szName;
	wstring							m_szMaterialName;

	VTXANIMMODEL*					m_pVertices = nullptr;
	FACEINDICES32*					m_pIndices = nullptr;
private:
	HRESULT Ready_Vertices(VTXANIMMODEL* pVertices, Matrix PivotMatrix);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType , class CModel* pModel, Matrix PivotMatrix);
	virtual void Free() override;
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
};

END