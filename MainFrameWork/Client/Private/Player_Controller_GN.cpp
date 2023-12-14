#include "stdafx.h"
#include "..\Public\Player_Controller_GN.h"

CPlayer_Controller_GN::CPlayer_Controller_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CPlayer_Controller_GN::CPlayer_Controller_GN(const CPlayer_Controller_GN& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CPlayer_Controller_GN::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CPlayer_Controller_GN::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPlayer_Controller_GN::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CPlayer_Controller_GN::DebugRender()
{
}

void CPlayer_Controller_GN::Input(const _float& fTimeDelta)
{
}

void CPlayer_Controller_GN::Attack()
{
}

void CPlayer_Controller_GN::Skill()
{
}

void CPlayer_Controller_GN::Hit()
{
}

CPlayer_Controller_GN* CPlayer_Controller_GN::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPlayer_Controller_GN::Clone(CGameObject* pGameObject, void* pArg)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Controller_GN::Free()
{
	__super::Free();
}