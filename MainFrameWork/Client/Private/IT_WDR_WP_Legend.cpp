#include "stdafx.h"
#include "..\Public\IT_WDR_WP_Legend.h"
#include "Player.h"

CIT_WDR_WP_Legend::CIT_WDR_WP_Legend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_WDR_WP_Legend::CIT_WDR_WP_Legend(const CIT_WDR_WP_Legend& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_WDR_WP_Legend::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_WDR_WP_Legend::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::RED;
	m_iItemType = (_uint)TYPE::EQUIP;
	m_iEquipType = (_uint)PART::WEAPON;

	m_strObjectTag = TEXT("IT_WDR_WP_Legend");
	m_strItemName = TEXT("전설의 모코코 망치");
	m_strItemDescript = TEXT("");

	m_tStatChangeDesc.iAtkPower = 500;

	return S_OK;
}

void CIT_WDR_WP_Legend::Tick(_float fTimeDelta)
{
}

void CIT_WDR_WP_Legend::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_WDR_WP_Legend::Render()
{
	return S_OK;
}

HRESULT CIT_WDR_WP_Legend::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	if (nullptr != pOwner->Get_EquipItem((_uint)PART::WEAPON))
		pOwner->Get_EquipItem((_uint)PART::WEAPON)->Disuse_Item(pOwner, false);

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower += m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(m_pModelCom);
	pOwner->Set_EquipItem((_uint)PART::WEAPON, this);

	Set_EquipWearing(true);

	return S_OK;
}

HRESULT CIT_WDR_WP_Legend::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;
	Set_EquipWearing(false);
	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower -= m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(nullptr);
	pOwner->Set_EquipItem((_uint)PART::WEAPON, nullptr);
	if (pOwner->Is_Control() && false == bUseDefault)
		pOwner->Add_Item(m_strObjectTag, this);

	return S_OK;
}

void CIT_WDR_WP_Legend::Upgrade_Item()
{
	m_tStatChangeDesc.iAtkPower += 100;
}

HRESULT CIT_WDR_WP_Legend::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_WDR_WP_Legend";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Legend_Weaopn"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_WDR_WP_Legend* CIT_WDR_WP_Legend::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_WDR_WP_Legend* pInstance = new CIT_WDR_WP_Legend(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_WDR_WP_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_WDR_WP_Legend::Clone(void* pArg)
{
	CIT_WDR_WP_Legend* pInstance = new CIT_WDR_WP_Legend(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_WDR_WP_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_WDR_WP_Legend::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
