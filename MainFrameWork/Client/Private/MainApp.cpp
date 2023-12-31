#include "stdafx.h"
#include "..\Public\MainApp.h"
#include <filesystem>
#include "tinyxml2.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "StateMachine.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "RigidBody.h"
#include "MainLogo.h"
#include "BackGround_Loading.h"
#include "ColliderOBB.h"
#include "ColliderSphereGroup.h"
#include "CollisionManager.h"
#include "ColliderFrustum.h"
#include "NavigationMgr.h"
#include "EventMgr.h"
#include "VIBuffer_Point.h"
#include "VIBuffer_Particle.h"
#include "PhysXMgr.h"

#include "UseLock_Transform.h"
#include "Camera_Free.h"
#include "Player.h"
#include "Camera_Player.h"
#include "BackGround_MainLogo.h"
#include "Chat_Manager.h"
#include "UI_LoadingWnd.h"
#include "UI_LoadingLabelTop.h"
#include "UI_LoadingLabelBottom.h"
#include "UI_LoadingEmptyBar.h"
#include "UI_LoadingFill.h"
#include "UI_LoadingArrow.h"
#include "UI_LoadingShine.h"
#include "UI_Manager.h"
#include "UI_Mouse_Cursor.h"
#include "BehaviorTree.h"
#include "UI_Tool.h"
#include "TextBox.h"
#include "LockFree_Transform.h"
#include "Projectile.h"
#include "UI_DamageFont.h"
#include "Pool.h"
#include "Damage_Manager.h"
#include "Effect_Manager.h"
#include "ThreadManager.h"
#include "UI_Loading.h"
#include "Effect.h"
#include "UI_SpeechBubble.h"
#include "QuadTreeMgr.h"

namespace fs = std::filesystem;

_float g_fVolume;

CMainApp::CMainApp()	
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	g_fVolume = 0.5f;

	/* 1. 내 게임의 초기화를 수행할꺼야. */
	/* 1-1. 그래픽장치를 초기화한다. */
	/* 1-2. 사운드장치를 초기화한다. */
	/* 1-3. 입력장치를 초기화한다. */
	GRAPHIC_DESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	SocketUtils::Init();

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, _uint(LAYER_TYPE::LAYER_END), GraphicDesc, &m_pDevice, &m_pContext, g_hWnd, g_hInstance)))
		return E_FAIL;
		
	if (FAILED(Initialize_Client()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	/* 1-4. 게임내에서 사용할 레벨(씬)을 생성한다.   */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	/* 1-4-1. 게임내에서 사용할 여러 자원(텍스쳐, 모델, 객체) 들을 준비한다.  */

	if (FAILED(Ready_Prototype_Font()))
		return E_FAIL;
	
	if (FAILED(Ready_Prototype_Effect()))
		return E_FAIL;

	if (FAILED(Ready_Camera_Free()))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::F1))
	{
		Active_Camera_Free();
	}
	if (KEY_TAP(KEY::F3))
	{
		CQuadTreeMgr::GetInstance()->Set_Stop(false);
	}
	if (KEY_TAP(KEY::F4))
	{
		CQuadTreeMgr::GetInstance()->Set_Stop(true);
	}

	if (KEY_HOLD(KEY::N) && KEY_TAP(KEY::NUM_1))
		CNavigationMgr::GetInstance()->OnOff_Render();

	m_pGameInstance->FinalTick(fTimeDelta);

	CServerSessionManager::GetInstance()->Tick(fTimeDelta);

	/* 게임내에 존재하는 여러 객체들의 갱신. */
	/* 레벨의 갱신 */
	m_pGameInstance->Tick(fTimeDelta);

	CChat_Manager::GetInstance()->CursurTick(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	/* 게임내에 존재하는 여러 객체들의 렌더링. */
	m_pGameInstance->Clear_BackBuffer_View(Vec4(0.f, 0.f, 0.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer_Com->Ready_InstanceRender();
	m_pRenderer_Com->Draw();
	
	CChat_Manager::GetInstance()->Render();

	CNavigationMgr::GetInstance()->Render(m_pGameInstance->Get_CurrLevelIndex());

	m_pGameInstance->Render_Debug();
	/* 초기화한 장면에 객체들을 그린다. */
	m_pGameInstance->Present();

	

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* 로고레베릉ㄹ 할당하고 싶었지만. 로고레벨을 위한 로딩레벨을 먼저 생성하여 로딩작업을 수행할꺼야. */
	/* 로딩객체에게 eLevelID라는 내가 실제 할당ㅎ아고 싶었던 레벨열거체를 준거지?! 실제할당하고싶었던 레벨에 자원을 준비라하라고 */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID, L"None"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Initialize_Client()
{
	CCollisionManager::GetInstance()->Reserve_Manager((_uint)LAYER_COLLIDER::LAYER_END);

	CNavigationMgr::GetInstance()->Reserve_Manager(m_pDevice, m_pContext);

	CEventMgr::GetInstance()->Reserve_EventMgr(m_pDevice, m_pContext);
	
	CPhysXMgr::GetInstance()->ReserveManager();

	CGameInstance::GetInstance()->Initialize_LoopChannel(CHANNELID::CHANNEL_LOOPSTART, CHANNEL_END);

	CChat_Manager::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext);

	CUI_Manager::GetInstance()->Reserve_Manager();

	CEffect_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext);
	//CUI_Tool::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext);

	ThreadManager::GetInstance()->ReserveManager(0);

	if (FAILED(CUI_Tool::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext)))
		return E_FAIL;

	// Manager Reserve
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer_Com = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer_Com);

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"),
		CVIBuffer_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_VIBuffer_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"),
		CVIBuffer_Particle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		Engine::CLockFree_Transform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"),
		CUseLock_Transform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_RigidBody */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		CRigidBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
	//	return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_State */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Behavior */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BehaviorTree"),
		CBehaviorTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"),
		CSphereCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SphereColliderGroup"),
		CSphereColliderGroup::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"),
		COBBCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FrustumColider"),
		CFrustumCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MainLogo"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/MainLogo.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/BackGround_Loading.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LostArk_MainLogo"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/MainLogo/Login_Logo%d.png", 36))))
		return E_FAIL;

	//신규로딩텍스처
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingBackGround"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i1b.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingImgs"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/LoadingImg%d.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/Loading_Rect.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Top"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i18_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Bottom"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i16_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_EmptyBar"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i7_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_FillBar"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i6_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_BarArrow"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i5_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Shine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/Loading_Shine.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Basic_Mouse"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Mouse/Basic_Mouse_ReSize.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading"),
		CBackGround_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LostArk_MainLogo"),
		CBackGround_MainLogo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LoadingWnd"),
		CUI_LoadingWnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LoadingLabel_Top"),
		CUI_LoadingLabelTop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LoadingLabel_Bottom"),
		CUI_LoadingLabelBottom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_EmptyBar"),
		CUI_LoadingEmptyBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_Fill"),
		CUI_LoadingFill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_Arrow"),
		CUI_LoadingArrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_Shine"),
		CUI_LoadingShine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MouseCursor"),
		CUI_Mouse_Cursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Model */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL::Elements, VTXMODEL::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_AnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL::Elements, VTXANIMMODEL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModelInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel_Instance.hlsl"), VTXINSTANCE_ANIMMODEL::Elements, VTXINSTANCE_ANIMMODEL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SkyBox"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkyBox.hlsl"), VTXMODEL::Elements, VTXMODEL::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_EffectMesh.hlsl"), VTXMODEL::Elements, VTXMODEL::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_EffectTex.hlsl"), VTXPOINT::Elements, VTXPOINT::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ParticleSystem"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_ParticleSystem.hlsl"), VTXPARTICLE::Elements, VTXPARTICLE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_StaticModelInstace"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel_Instance.hlsl"), VTXINSTANCE_MODEL::Elements, VTXINSTANCE_MODEL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext, L"Free_Camera"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TextBox"),
		CTextBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingUI"),
		CUI_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CDamage_Manager::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext);
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Projectile"),
		CProjectile::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 200; ++i)
	{
		CProjectile* pProjectile = dynamic_cast<CProjectile*>(m_pGameInstance->Add_GameObject((_uint)LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Projectile"));
		CPool<CProjectile>::Return_Obj(pProjectile);
	}


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Middle"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chat_Bubble_Middle.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Up"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chat_Bubble_Up.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Down"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chat_Bubble_Down.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpeechBubble"),
		CUI_SpeechBubble::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 10; ++i)
	{
		_uint iIndex = i;
		CUI_SpeechBubble* pUI = dynamic_cast<CUI_SpeechBubble*>(m_pGameInstance->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_SpeechBubble"), &iIndex));
		CPool<CUI_SpeechBubble>::Return_Obj(pUI);
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Font()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->AddFont(L"125", L"../Bin/Resources/Fonts/125.spritefont");
	pGameInstance->AddFont(L"AdobeFont", L"../Bin/Resources/Fonts/AdobeFont.spritefont");
	pGameInstance->AddFont(L"Cascadia", L"../Bin/Resources/Fonts/Cascadia.spritefont");
	pGameInstance->AddFont(L"한컴고딕", L"../Bin/Resources/Fonts/한컴고딕.spritefont");
	pGameInstance->AddFont(L"단조", L"../Bin/Resources/Fonts/단조.spritefont");
	pGameInstance->AddFont(L"던파연마된칼날", L"../Bin/Resources/Fonts/던파연마된칼날.spritefont");
	pGameInstance->AddFont(L"넥슨Lv1고딕", L"../Bin/Resources/Fonts/넥슨Lv1고딕.spritefont");
	pGameInstance->AddFont(L"빛의계승자", L"../Bin/Resources/Fonts/빛의계승자.spritefont");
	
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Effect()
{	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// Mesh
	wstring strMeshEffectFilePath = TEXT("../Bin/Resources/Effects/FX_Meshes/");
	for (const auto& category : fs::directory_iterator(strMeshEffectFilePath))
	{
		if (!category.is_directory())
			continue;
		for (const auto& entry : fs::directory_iterator(category.path()))
		{
			Matrix matPivot = Matrix::Identity;
			const wstring& strFileName = entry.path().stem();
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;
			wstring strFinalPath = category.path().generic_wstring() + TEXT("/");

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFinalPath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	// Effect
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMainApp::Ready_Camera_Free()
{
	CCamera::CAMERADESC tCameraDesc;

	tCameraDesc.iLayer = (_uint)LAYER_TYPE::LAYER_CAMERA;
	tCameraDesc.vEye = Vec4(0.f, 10.f, -10.f, 1.f);
	tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	tCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	tCameraDesc.fNear = 0.2f;
	tCameraDesc.fFar = 1200.0f;
	tCameraDesc.TransformDesc.fRotationPerSec = 10.0f;
	tCameraDesc.TransformDesc.fSpeedPerSec = 10.0f;

	CGameObject* pCamera = CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free", &tCameraDesc);

	m_pCamera = dynamic_cast<CCamera_Free*>(pCamera);

	return S_OK;
}

void CMainApp::Active_Camera_Free()
{
	if (m_pCamera->Is_Active())
	{
		if (nullptr != CServerSessionManager::GetInstance()->Get_Player())
		{
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Active(true);
		}
		m_pCamera->Set_Active(false);
	}                                 
	else
	{
		if (nullptr != CServerSessionManager::GetInstance()->Get_Player())
		{
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Active(false);
			m_pCamera->Get_TransformCom()->Set_WorldMatrix(CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Get_TransformCom()->Get_WorldMatrix());
		}
		m_pCamera->Set_Active(true);
	}
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{	
	__super::Free();

	Safe_Release(m_pRenderer_Com);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	CGameInstance::Release_Engine();

	SocketUtils::Clear();
}
