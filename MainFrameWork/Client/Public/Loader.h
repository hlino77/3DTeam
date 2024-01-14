#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include <filesystem>

namespace fs = std::filesystem;
/* 1. ������������ ���� �ε� �۾��� ���٤�������. */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual~CLoader() = default;

public:
	const wstring Get_LoadingText() const {
		return m_strLoading;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

public:
	/* eNextLevel�� ���� �ε��� �ϱ����� �����带 �����Ѵ�. */
	HRESULT Initialize(LEVELID eNextLevel);
	_int	Loading();


private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	LEVELID					m_eNextLevel = { LEVEL_END };
	wstring					m_strLoading = { TEXT("") };
	_bool					m_isFinished = { false };

private:
	HANDLE					m_hThread = { 0 };
	CRITICAL_SECTION		m_Critical_Section;

	vector<future<HRESULT>> m_Futures;

private:
	HRESULT Loading_For_Level_Logo();
	HRESULT Loading_For_Level_Tool();
	HRESULT	Loading_For_Level_Tool_Npc();
	HRESULT Loading_For_Level_ServerSelect();
	HRESULT Loading_For_Level_Lobby();
	HRESULT Loading_For_Level_Bern();
	HRESULT Loading_For_Level_Chaos1();
	HRESULT Loading_For_Level_Chaos2();
	HRESULT Loading_For_Level_Chaos3();

	HRESULT	Load_NpcData();
	HRESULT Start_Load_Npc(const wstring& strPath);
	HRESULT	AutoLoad(const fs::path& strPath, LEVELID eLevel, Matrix Pivot = XMMatrixIdentity());

	HRESULT Load_MapData(LEVELID eLevel, const wstring& szFilePath);
	HRESULT Load_ColMesh(LEVELID eLevel, const wstring& szFilePath);
	HRESULT Loading_LobbyUI();
	HRESULT	Loading_QuickSlot();
	HRESULT	Loading_SkillIcon();
	HRESULT Loading_ChaosDungeon_UI();
	HRESULT	Loading_Npc_UI();
	HRESULT	Loading_Npc_UI_Texture();

	HRESULT Loading_Model_For_Level_Bern();
	HRESULT Loading_Model_For_Level_Lobby();
	HRESULT	Loading_Model_For_Level_Tool_Npc();

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel);
	virtual void Free() override;
};

END 