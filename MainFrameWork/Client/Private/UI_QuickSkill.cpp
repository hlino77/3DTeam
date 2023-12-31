#include "stdafx.h"
#include "UI_QuickSkill.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_SkillIcon_Frame.h"
#include "UI_Skill_MoveIcon.h"

CUI_QuickSkill::CUI_QuickSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}
CUI_QuickSkill::CUI_QuickSkill(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_QuickSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_QuickSkill::Initialize(void* pArg)
{

	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("UI_QuickSkill");

	return S_OK;
}

void CUI_QuickSkill::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_QuickSkill::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_QuickSkill::Render()
{
	return S_OK;
}

void CUI_QuickSkill::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_QuickSkill::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUI = nullptr;

	for (size_t i = 0; i < 8; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_SkillFrame")
		, &i));
		if (nullptr != pUI)
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	Load_UIData(TEXT("Skill_Icon_Desc"));

	for (auto& iter : m_vecUIParts)
	{
		iter->Create_Rect();
	}
	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_QuickSkill* CUI_QuickSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_QuickSkill* pInstance = new CUI_QuickSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_QuickSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_QuickSkill::Clone(void* pArg)
{
	CUI_QuickSkill* pInstance = new CUI_QuickSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_QuickSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_QuickSkill::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
