#include "stdafx.h"
#include "..\Public\IT_WDR_Body_Mococo.h"
#include "Player.h"

CIT_WDR_Body_Mococo::CIT_WDR_Body_Mococo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_WDR_Body_Mococo::CIT_WDR_Body_Mococo(const CIT_WDR_Body_Mococo& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_WDR_Body_Mococo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_WDR_Body_Mococo::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::PURPLE;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_WDR_Body_Mococo");
	m_strItemName = TEXT("������ ����");
	m_strItemDescript = TEXT("");

	m_iEquipType = (_uint)PART::BODY;

	m_vecUseEquipSlot.push_back((_uint)PART::BODY);
	m_vecUseEquipSlot.push_back((_uint)PART::SHOULDER);
	m_vecUseEquipSlot.push_back((_uint)PART::ARM);
	m_vecUseEquipSlot.push_back((_uint)PART::LEG);

	m_tStatChangeDesc.iHp = 500;

	return S_OK;
}

void CIT_WDR_Body_Mococo::Tick(_float fTimeDelta)
{
}

void CIT_WDR_Body_Mococo::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_WDR_Body_Mococo::Render()
{
	return S_OK;
}

HRESULT CIT_WDR_Body_Mococo::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp += m_tStatChangeDesc.iHp;
	tPcStat.iCurHp += m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);
	static_cast<CPlayer*>(pOwner)->Send_Hp();

	for (auto& i : m_vecUseEquipSlot)
	{
		if (nullptr != pOwner->Get_EquipItem(i))
		{
			pOwner->Get_EquipItem(i)->Disuse_Item(pOwner, false);
		}
		pOwner->Set_EquipItem(i, this);

		if (i == m_iEquipType)
		{
			pOwner->Set_ModelPart(i, m_pModelCom);
		}
		else
		{
			pOwner->Set_ModelPart(i, nullptr);
		}
	}
	Set_EquipWearing(true);
	
	return S_OK;
}

HRESULT CIT_WDR_Body_Mococo::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;
	Set_EquipWearing(false);

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp -= m_tStatChangeDesc.iHp;
	tPcStat.iCurHp -= m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);
	static_cast<CPlayer*>(pOwner)->Send_Hp();

	for (auto& i : m_vecUseEquipSlot)
	{
		pOwner->Set_ModelPart(i, pOwner->Get_DefaultPart(i));
		pOwner->Set_EquipItem(i, nullptr);
	}
	if (pOwner->Is_Control() && false == bUseDefault)
		pOwner->Add_Item(m_strObjectTag, this);

	return S_OK;
}

void CIT_WDR_Body_Mococo::Upgrade_Item()
{
	m_tStatChangeDesc.iHp += 100;
}

HRESULT CIT_WDR_Body_Mococo::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_WDR_Body_BaseMococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Mococo_Body"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_WDR_Body_Mococo* CIT_WDR_Body_Mococo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_WDR_Body_Mococo* pInstance = new CIT_WDR_Body_Mococo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_WDR_Body_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_WDR_Body_Mococo::Clone(void* pArg)
{
	CIT_WDR_Body_Mococo* pInstance = new CIT_WDR_Body_Mococo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_WDR_Body_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_WDR_Body_Mococo::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
