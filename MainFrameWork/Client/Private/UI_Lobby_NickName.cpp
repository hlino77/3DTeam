#include "stdafx.h"
#include "UI_Lobby_NickName.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "Player_Select.h"
#include "UI_Lobby_NickNameChange.h"
#include "UI_Manager.h"
#include "Sound_Manager.h"

CUI_Lobby_NickName::CUI_Lobby_NickName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Lobby_NickName::CUI_Lobby_NickName(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Lobby_NickName::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Lobby_NickName::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_uint* strIndex = static_cast<_uint*>(pArg);
		m_strUITag = TEXT("Lobby_NickName");
		m_strUITag += to_wstring(*strIndex + 1);
		Initialize_Index(*strIndex);
	}
	else
		m_strUITag = TEXT("Lobby_NickName");

	Initialize_Textbox();
	Random_NickName();
	Print_NickName();
	return S_OK;
}

HRESULT CUI_Lobby_NickName::Initialize_Textbox()
{
	m_strFont = L"���Ŀ�����Į��";
	m_strClassFont = TEXT("�ؽ�Lv1����Bold");
	Ready_TextBox();
	m_pNickNameWnd->Set_Active(true);
	m_pClassNameWnd->Set_Active(true);
	m_pLevelWnd->Set_Active(true);
	return S_OK;
}

void CUI_Lobby_NickName::Tick(_float fTimeDelta)
{
	InitialIze_EmblemPos();
	__super::Tick(fTimeDelta);
	Moving_Rect();
	Picking_UI();
	Update_NickNameFrame();
}

void CUI_Lobby_NickName::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	LateUpdate_NickNameFrame();
	Get_NickName_Changer();
}

HRESULT CUI_Lobby_NickName::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom_NickNameShine->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Emblem()))
		return E_FAIL;
	m_pTextureCom_Emblem->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iClassIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_pNickNameWnd->Render();
	m_pClassNameWnd->Render();
	m_pLevelWnd->Render();
	return S_OK;
}

HRESULT CUI_Lobby_NickName::Ready_Components()
{
	__super::Ready_Components();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Emblem"), (CComponent**)&m_pTransformCom_Emblem)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Frame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_ShineEffect"),
		TEXT("Com_Texture_Shine"), (CComponent**)&m_pTextureCom_NickNameShine)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emblem"),
		TEXT("Com_Texture_Emblem"), (CComponent**)&m_pTextureCom_Emblem)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CUI_Lobby_NickName::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Lobby_NickName::Bind_ShaderResources_Emblem()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Emblem->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	return S_OK;
}

void CUI_Lobby_NickName::Update_NickNameFrame()
{
	//UI Clicked
	if ((m_bPick) && (BUTTON_STATE::BUTTON_NORMAL == m_iButtonState))
		m_iButtonState = BUTTON_STATE::BUTTON_PICKING;
	if ((BUTTON_STATE::BUTTON_PICKING == m_iButtonState) && ((KEY_HOLD(KEY::LBTN)) || (KEY_TAP(KEY::LBTN))))
	{
		m_iButtonState = BUTTON_STATE::BUTTON_PICKED;
		static_cast<CPlayer_Select*>(m_pLobbyCharacter)->Clicked();
	}

	if ((!m_bPick) && (BUTTON_STATE::BUTTON_PICKING == m_iButtonState))
		m_iButtonState = BUTTON_STATE::BUTTON_NORMAL;
	
}

void CUI_Lobby_NickName::Get_NickName_Changer()
{
	CUI* pUI = CUI_Manager::GetInstance()->Find_UIPart(LEVEL_LOBBY, TEXT("UI_Lobby"), TEXT("Lobby_NameChanger"));

	if ((nullptr != pUI)&&(BUTTON_STATE::BUTTON_PICKED == m_iButtonState))
		static_cast<CUI_Lobby_NickNameChange*>(pUI)->Set_NickNmaeFrame(this);
}

void CUI_Lobby_NickName::LateUpdate_NickNameFrame()
{
	//Player Clicked
	if (static_cast<CPlayer_Select*>(m_pLobbyCharacter)->Is_Selected())
		m_iButtonState = BUTTON_STATE::BUTTON_PICKED;
	else
		m_iButtonState = BUTTON_STATE::BUTTON_NORMAL;

	if (BUTTON_STATE::BUTTON_PICKED == m_iButtonState)
	{
		if (!m_bSound)
		{
			m_bSound = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		m_iTextureIndex = 1;
	}
	else
	{
		m_iTextureIndex = 0;
		m_bSound = false;
	}
}

void CUI_Lobby_NickName::InitialIze_EmblemPos()
{
	if (m_bInitializeEmblem)
		return;

	Vec3 vPos = {};
	vPos.x = (m_fX - (m_fSizeX * 0.40f) - g_iWinSizeX * 0.5f);
	vPos.y = (-(m_fY - 10.f)) + (g_iWinSizeY * 0.5f);
	vPos.z = 0.2f;

	m_pTransformCom_Emblem->Set_Scale(Vec3(32.f, 32.f, 0.2f));
	m_pTransformCom_Emblem->Set_State(CTransform::STATE_POSITION, vPos);

	m_bInitializeEmblem = true;
}

void CUI_Lobby_NickName::Update_Equipment_Level()
{
}

void CUI_Lobby_NickName::Initialize_Index(_uint iIndex)
{
	m_fSizeX = 205.f;
	m_fSizeY = 53.f;

	switch (iIndex)
	{
	case 0:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObject((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("Gunslinger"));
		m_iClassIndex = (_uint)CHR_CLASS::GUNSLINGER;
		m_fX = 330.f;
		m_fY = 800.f;
		m_strClassName = TEXT("�ǽ�����");
		break;

	case 1:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObject((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("WDR"));
		m_iClassIndex = (_uint)CHR_CLASS::DESTROYER;
		m_fX = 630.f;
		m_fY = 800.f;
		m_strClassName = TEXT("��Ʈ���̾�");
		break;

	case 2:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObject((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("SP"));
		m_iClassIndex = (_uint)CHR_CLASS::DOAGA;
		m_fX = 930.f;
		m_fY = 800.f;
		m_strClassName = TEXT("��ȭ��");
		break;

	case 3:
		m_pLobbyCharacter = CGameInstance::GetInstance()->Find_GameObject((_uint)LEVELID::LEVEL_LOBBY,
			(_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("WR"));
		m_iClassIndex = (_uint)CHR_CLASS::SLAYER;
		m_fX = 1230.f;
		m_fY = 800.f;
		m_strClassName = TEXT("�����̾�");
		break;
	}

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	m_pTransformCom_Emblem->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom_Emblem->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
}

void CUI_Lobby_NickName::Print_NickName()
{
	if (nullptr != m_pNickNameWnd)
	{
		m_pNickNameWnd->Clear_Text();
		m_pNickNameWnd->Set_Alpha(1.f);

		Vec3 vResultPos = Vec3((m_fX + 35.f) - g_iWinSizeX * 0.5f, -(m_fY - 10.f) + g_iWinSizeY * 0.5f, 0.f);
		m_pNickNameWnd->Get_TransformCom()->Set_Scale(Vec3(205.f, 53.0f, 0.f));// Vec2(205.f, 53.0f);
		m_pNickNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"���ǰ����", m_strNickName);
		Vec2 vOrigin = vMeasure * 0.5f;

		m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex) + TEXT("-1"), m_strFont, m_strNickName, Vec2(10.f - 1.f, 26.5f), Vec2(0.4f, 0.4f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex) + TEXT("-2"), m_strFont, m_strNickName, Vec2(10.f + 1.f, 26.5f), Vec2(0.4f, 0.4f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
		m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex) + TEXT("-3"), m_strFont, m_strNickName, Vec2(10.f, 26.5f - 1.f), Vec2(0.4f, 0.4f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
		m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex) + TEXT("-4"), m_strFont, m_strNickName, Vec2(10.f, 26.5f + 1.f), Vec2(0.4f, 0.4f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.f));
		m_pNickNameWnd->Set_Text(TEXT("NickNameWnd") + to_wstring(m_iClassIndex), m_strFont, m_strNickName, Vec2(10.f, 26.5f), Vec2(0.4f, 0.4f), Vec2(0.f, 0.f), 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (nullptr != m_pLevelWnd)
	{
		m_pLevelWnd->Clear_Text();
		m_pLevelWnd->Set_Alpha(1.f);

		Vec3 vResultPos = Vec3((m_fX - 35.f) - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.f);
		m_pLevelWnd->Get_TransformCom()->Set_Scale(Vec3(205.f, 53.0f, 0.f));// Vec2(205.f, 53.0f);
		m_pLevelWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"�ؽ�Lv1����Bold", TEXT("Lv.1"));
		Vec2 vOrigin = vMeasure * 0.5f;
		m_pLevelWnd->Set_Text(TEXT("NickNameWnd_Level") + to_wstring(m_iClassIndex), L"�ؽ�Lv1����Bold", TEXT("Lv.1"), Vec2(m_fSizeX * 0.27f, m_fSizeY * 0.75f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 0.78, 0.0f, 1.0f));
	}

	if (nullptr != m_pClassNameWnd)
	{
		m_pClassNameWnd->Clear_Text();
		m_pClassNameWnd->Set_Alpha(1.f);

		Vec3 vResultPosClass = Vec3((m_fX - 55.f) - g_iWinSizeX * 0.5f, -(m_fY - 25.f) + g_iWinSizeY * 0.5f, 0.f);
		m_pClassNameWnd->Get_TransformCom()->Set_Scale(Vec3(205.f, 25.f, 0.f));
		m_pClassNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPosClass);

		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"�ؽ�Lv1����Bold", m_strClassName);
		Vec2 vOrigin = vMeasure * 0.5f;
		m_pClassNameWnd->Set_Text(TEXT("ClassNameWnd-1") + to_wstring(m_iClassIndex), m_strClassFont, m_strClassName, Vec2(102.5f -1.f, 12.5f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pClassNameWnd->Set_Text(TEXT("ClassNameWnd-2") + to_wstring(m_iClassIndex), m_strClassFont, m_strClassName, Vec2(102.5f +1.f, 12.5f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pClassNameWnd->Set_Text(TEXT("ClassNameWnd-3") + to_wstring(m_iClassIndex), m_strClassFont, m_strClassName, Vec2(102.5f, 12.5f -1.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pClassNameWnd->Set_Text(TEXT("ClassNameWnd-4") + to_wstring(m_iClassIndex), m_strClassFont, m_strClassName, Vec2(102.5f, 12.5f +1.f), Vec2(0.3f, 0.3f), Vec2(0.f, 0.f), 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pClassNameWnd->Set_Text(TEXT("ClassNameWnd") + to_wstring(m_iClassIndex), m_strClassFont, m_strClassName, Vec2(102.5f, 12.5f), Vec2(0.3f, 0.3f), Vec2(0.f,0.f), 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pClassNameWnd->Set_Alpha(0.6f);
	}
}

HRESULT CUI_Lobby_NickName::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pNickNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("NickNameWnd") + to_wstring(m_iClassIndex);
		tTextDesc.vSize = Vec2(205.f, 53.0f);
		m_pNickNameWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
	
		if (nullptr == m_pNickNameWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pNickNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pNickNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	if (nullptr == m_pClassNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("ClassNameWnd") + to_wstring(m_iClassIndex);
		tTextDesc.vSize = Vec2(205.f, 25.0f);
		m_pClassNameWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pClassNameWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pClassNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pClassNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}
	if (nullptr == m_pLevelWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("ClassLevelWnd") + to_wstring(m_iClassIndex);
		tTextDesc.vSize = Vec2(205.f, 53.0f);
		m_pLevelWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pLevelWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pLevelWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pLevelWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}
	m_pNickNameWnd->Set_Render(false);
	m_pClassNameWnd->Set_Render(false);
	m_pLevelWnd->Set_Render(false);
	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_Lobby_NickName::Start_NickNameText()
{
	m_strNickName.clear();
	m_pNickNameWnd->Set_Active(true);

	m_strClassName.clear();
	m_pClassNameWnd->Set_Active(true);
}

void CUI_Lobby_NickName::End_NickNameText()
{
	m_strNickName.clear();
	m_pNickNameWnd->Set_Active(false);

	m_strClassName.clear();
	m_pClassNameWnd->Set_Active(false);
}

void CUI_Lobby_NickName::Set_NickName(const wstring& strStageName)
{
	m_strNickName.clear();
	m_strNickName = strStageName;
	Print_NickName();
}

void CUI_Lobby_NickName::Random_NickName()
{
	/*_int iRandomIndex =  CGameInstance::GetInstance()->Random_Int(0,10);

	switch (iRandomIndex)	
	{
	case 0 :
		m_strNickName = TEXT("���6232��");
		break;
	case 1:
		m_strNickName = TEXT("�ξ��ϰ��;��");
		break;
	case 2:
		m_strNickName = TEXT("������Nullptr");
		break;
	case 3:
		m_strNickName = TEXT("�̰ſ־ȵ�");
		break;
	case 4:
		m_strNickName = TEXT("��������");
		break;
	case 5:
		m_strNickName = TEXT("������Gpt");
		break;
	case 6:
		m_strNickName = TEXT("�ҰԳʹ�����");
		break;
	case 7:
		m_strNickName = TEXT("�ξư�����");
		break;
	case 8:
		m_strNickName = TEXT("å���ڴ�����");
		break;
	case 9:
		m_strNickName = TEXT("���߸��ƴ�");
		break;
	case 10:
		m_strNickName = TEXT("����ּ���");
		break;
	}*/
	if ((_uint)CHR_CLASS::GUNSLINGER == m_iClassIndex)
		m_strNickName = TEXT("�ǽ�����");
	else if((_uint)CHR_CLASS::SLAYER ==  m_iClassIndex)
		m_strNickName = TEXT("����䳢�ϵ�");
	else if ((_uint)CHR_CLASS::DESTROYER == m_iClassIndex)
		m_strNickName = TEXT("��Ʈ���̾�");
	else if ((_uint)CHR_CLASS::DOAGA == m_iClassIndex)
		m_strNickName = TEXT("��ȭ��");
}

CUI_Lobby_NickName* CUI_Lobby_NickName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Lobby_NickName* pInstance = new CUI_Lobby_NickName(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Lobby_NickName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Lobby_NickName::Clone(void* pArg)
{
	CUI_Lobby_NickName* pInstance = new CUI_Lobby_NickName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Lobby_NickName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Lobby_NickName::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pNickNameWnd->Set_Dead(true);
	m_pClassNameWnd->Set_Dead(true);
	m_pLevelWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom_NickNameShine);
	Safe_Release(m_pTextureCom_Emblem);
	Safe_Release(m_pTextureCom_Equipment);
	Safe_Release(m_pTransformCom_Emblem);
}
