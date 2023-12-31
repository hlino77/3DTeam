#include "stdafx.h"
#include "Camera_Free.h"
#include "Engine_Defines.h"
#include "GameInstance.h"


CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs, CTransform::TRANSFORMDESC * pArg)
	: CCamera(rhs, pArg)
{
	
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	
	m_bActive = false;

	return S_OK;
}

void CCamera_Free::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Tick(fTimeDelta);

	if (LEVEL_TOOL == m_pGameInstance->Get_CurrLevelIndex())
		return;

	if (KEY_HOLD(KEY::UP_ARROW))
	{
		m_pTransformCom->Go_Straight(5.0f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		m_pTransformCom->Go_Backward(5.0f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		m_pTransformCom->Go_Left(5.0f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		m_pTransformCom->Go_Right(5.0f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::P))
	{
		m_pTransformCom->Go_Up(fTimeDelta * 5.f);
	}

	if (KEY_HOLD(KEY::O))
	{
		m_pTransformCom->Go_Down(fTimeDelta * 5.f);
	}

	if (KEY_TAP(KEY::F2))
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_CameraDesc.vEye));
		m_pTransformCom->LookAt(XMLoadFloat4(&m_CameraDesc.vAt));
	}

	if (KEY_TAP(KEY::TAB))
	{
		m_bMouse = !m_bMouse;
		ShowCursor(m_bMouse);
	}

	if (!m_bMouse)
	{

		POINT MousePos;

		MousePos.x = g_iWinSizeX / 2;
		MousePos.y = g_iWinSizeY / 2;

		ClientToScreen(g_hWnd, &MousePos);
		SetCursorPos(MousePos.x, MousePos.y);

		_long	MouseMove  = 0;

		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
		{
			if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
				m_pTransformCom->Move_Dir(-m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta, MouseMove * 3.f);
			else
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * 0.05f);
		}

		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
		{
			if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
				m_pTransformCom->Move_Dir(m_pTransformCom->Get_State(CTransform::STATE_UP), fTimeDelta, MouseMove * 3.f);
			else	
				m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * 0.05f);

		}
	}

	Safe_Release(pGameInstance);
}

void CCamera_Free::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

HRESULT CCamera_Free::Ready_Components()
{
	__super::Ready_Components();
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Free*		pInstance = new CCamera_Free(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void* pArg)
{
	CCamera::CAMERADESC*		pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Free*		pInstance = new CCamera_Free(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();


}
