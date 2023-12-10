#include "VIBuffer_Point.h"

CVIBuffer_Point::CVIBuffer_Point(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Point::CVIBuffer_Point(const CVIBuffer_Point & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Point::Initialize_Prototype()
{
#pragma region INDEX_BUFFER

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	/* �������ۿ� �ε��� ���۸� ���峮. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = 2;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �Ҵ��Ѵ�. (Lock, unLock ȣ�� �Ұ�)*/
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort*		pIndices = new _ushort;
	*pIndices = 0;


	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete(pIndices);
#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Point::Initialize(void * pArg)
{

	return S_OK;
}

HRESULT CVIBuffer_Point::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize)
{
	if (nullptr == m_pContext)
		return E_FAIL;


	_uint iStrideInstance = sizeof(VTXINSTANCE);

	ID3D11Buffer* pVertexBuffers[] = {
		pInstanceBuffer
	};

	_uint			iStrides[] = {
		iStrideInstance
	};

	_uint			iOffsets[] = {
		0
	};


	/* ���ؽ� ���۵��� �Ҵ��Ѵ�. */
	/* �׸���� �������� + ���º�ȯ�� �������� */
	m_pContext->IASetVertexBuffers(0, 1, pVertexBuffers, iStrides, iOffsets);

	/* �ε��� ���۸� �Ҵ��Ѵ�. */
	/* �׸����� �ϴ� �ν��Ͻ��� ������ŭ Ȯ��Ǿ��ִ� �ε��� ����. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* �ش� �������� � ������� �׸�����. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* �ε����� ����Ű�� ������ Ȱ���Ͽ� �׸���. */
	m_pContext->DrawIndexedInstanced(1, iSize, 0, 0, 0);


	return S_OK;
}

HRESULT CVIBuffer_Point::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, _uint iStride)
{
	if (nullptr == m_pContext)
		return E_FAIL;


	_uint iStrideInstance = iStride;

	ID3D11Buffer* pVertexBuffers[] = {
		pInstanceBuffer
	};

	_uint			iStrides[] = {
		iStrideInstance
	};

	_uint			iOffsets[] = {
		0
	};


	/* ���ؽ� ���۵��� �Ҵ��Ѵ�. */
	/* �׸���� �������� + ���º�ȯ�� �������� */
	m_pContext->IASetVertexBuffers(0, 1, pVertexBuffers, iStrides, iOffsets);

	/* �ε��� ���۸� �Ҵ��Ѵ�. */
	/* �׸����� �ϴ� �ν��Ͻ��� ������ŭ Ȯ��Ǿ��ִ� �ε��� ����. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* �ش� �������� � ������� �׸�����. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* �ε����� ����Ű�� ������ Ȱ���Ͽ� �׸���. */
	m_pContext->DrawIndexedInstanced(1, iSize, 0, 0, 0);


	return S_OK;
}


CVIBuffer_Point * CVIBuffer_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Point::Clone(CGameObject* pObject, void* pArg)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Point::Free()
{
	__super::Free();


}
