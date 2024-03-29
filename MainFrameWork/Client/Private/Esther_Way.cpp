#include "stdafx.h"
#include "..\Public\Esther_Way.h"
#include "GameInstance.h"
#include "Esther_Way_Cut.h"
#include "Esther_Way_Skill.h"
#include "Esther_Scene.h"

CEsther_Way::CEsther_Way(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther(pDevice, pContext)
{
}

CEsther_Way::CEsther_Way(const CEsther_Way& rhs)
	: CEsther(rhs)
{
}

HRESULT CEsther_Way::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Way::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Way");
	m_iEstherType = (_uint)ESTHERTYPE::WY;

	CEsther_Skill::ESTHERSKILLDESC SkillDesc;
	SkillDesc.pLeaderPlayer = m_pLeaderPlayer;
	SkillDesc.pOwnerEsther = this;
	m_pEsther_Skill = static_cast<CEsther_Way_Skill*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Way_Skill"), &SkillDesc));


	CEsther_Scene::ESTHERSCENEDESC SceneDesc;
	SceneDesc.fStartFrame = 0.f;
	SceneDesc.fFinalFrame = 211.f;
	SceneDesc.fFrameSpeed = 25.f;
	SceneDesc.strTexutre = TEXT("Esther_Cut_Way");
	m_pEsther_Scene = static_cast<CEsther_Scene*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Scene"), &SceneDesc));

	/*CEsther_Cut::ESTHERCUTDESC CutDesc;
	CutDesc.pLeaderPlayer = m_pLeaderPlayer;
	CutDesc.pOwnerEsther = this;
	m_pEsther_Cut = static_cast<CEsther_Way_Cut*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Way_Cut"), &CutDesc));*/

	return S_OK;
}

void CEsther_Way::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CEsther_Way::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEsther_Way::Render()
{
	return S_OK;
}

void CEsther_Way::Leader_Active_Esther()
{
	__super::Leader_Active_Esther();
}

HRESULT CEsther_Way::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

CEsther_Way* CEsther_Way::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Way* pInstance = new CEsther_Way(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Way");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Way::Clone(void* pArg)
{
	CEsther_Way* pInstance = new CEsther_Way(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Way");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Way::Free()
{
	__super::Free();
}
