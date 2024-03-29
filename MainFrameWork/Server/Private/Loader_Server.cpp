#include "stdafx.h"
#include "Loader_Server.h"
#include "GameInstance.h"
#include "GameSessionManager.h"
#include "Player_Server.h"
#include "Monster_Zombie_Server.h"
#include "Monster_Plant_Server.h"
#include "Boss_Golem_Server.h"
#include "Monster_Ghoul_Server.h"
#include "Monster_Reaper_Server.h"
#include "Monster_Pawn_Server.h"
#include "Monster_Prison_Server.h"
#include "Boss_King_Server.h"
#include "Boss_Valtan_Server.h"
#include "ChaosDungean_Server.h"
#include "NavigationMgr.h"
#include "Guide_Chaos_Npc_Server.h"
#include "Guide_Valtan_Npc_Server.h"
#include "ValtanMain_Server.h"
#include "BreakAbleObject_Server.h"
#include "Boss_Valtan_CounterGhost_Server.h"
#include "Skill_RisingSun_Server.h"
#include "Skill_TeleportDoor_Server.h"

CLoader_Server::CLoader_Server()
{
}

_uint APIENTRY ThreadEntry(void* pArg)
{

	CoInitializeEx(nullptr, 0);

	/* 새롭게 생성된 스레드가 일한다. */
	CLoader_Server* pLoader = (CLoader_Server*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader_Server::Initialize(LEVELID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader_Server::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;

	switch (m_eNextLevel)
	{
	case LEVEL_BERN:
		hr = Loading_For_Level_Bern();
		break;
	}

	if (FAILED(hr))
		return -1;

	LeaveCriticalSection(&m_Critical_Section);

	return 0;
}

HRESULT CLoader_Server::Loading_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_BERN, L"BernCastle.Navi");
	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_CHAOS_1, L"Chaos1.Navi");
	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_CHAOS_2, L"Chaos2.Navi");
	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_CHAOS_3, L"Chaos3.Navi");
	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_VALTANMAIN, L"Boss_End.Navi");


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer_Server::Create(nullptr, nullptr))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Zombie"),
		CMonster_Zombie_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Plant"),
		CMonster_Plant_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Golem"),
		CBoss_Golem_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Ghoul"),
		CMonster_Ghoul_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Reaper"),
		CMonster_Reaper_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_King"),
		CBoss_King_Server::Create(nullptr, nullptr))))
		return E_FAIL;
	  
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pawn"),
		CMonster_Pawn_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Prison"),
		CMonster_Prison_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Prison"),
		CMonster_Prison_Server::Create(nullptr, nullptr))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Valtan"),
		CBoss_Valtan_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Valtan_CounterGhost"),
		CBoss_Valtan_CounterGhost_Server::Create(nullptr, nullptr))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungean"),
		CChaosDungean_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ValtanMain"),
		CValtanMain_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	/* 기능 NPC */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Guide_Chaos_Npc"),
		CGuide_Chaos_Npc_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Guide_Valtan_Npc"),
		CGuide_Valtan_Npc_Server::Create(nullptr, nullptr))))
		return E_FAIL;


	//===============Skill ===========================

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_RisingSun"),
		CSkill_RisingSun_Server::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Skill_TeleportDoor"),
		CSkill_TeleportDoor_Server::Create(nullptr, nullptr))))
		return E_FAIL;


	// Map Object -> BreakAbleObject
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BreakAbleObject"),
	//	CBreakAbleObject_Server::Create(nullptr, nullptr))))
	//	return E_FAIL;


	Loading_Model_For_Level_Bern();

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Server::Loading_Model_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	{
		wstring strFileName = L"Gunslinger";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, PivotMatrix))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"WR";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, PivotMatrix))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"WDR";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, PivotMatrix))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"MG";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, PivotMatrix))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"SP";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, PivotMatrix))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Monster_Zombie";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Monster_Plant";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Boss_Golem";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Monster_Ghoul";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Monster_Reaper";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Boss_King";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Monster_Pawn";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	{
		wstring strFileName = L"Boss_Valtan";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);



	return S_OK;
}

CLoader_Server* CLoader_Server::Create(LEVELID eNextLevel)
{
	CLoader_Server* pInstance = new CLoader_Server();

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Server::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);
}
