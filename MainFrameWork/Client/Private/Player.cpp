#include "stdafx.h"
#include "GameInstance.h"
#include "Player.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "PickingMgr.h"
#include "Struct.pb.h"
#include "NavigationMgr.h"
#include "Skill.h"
#include "Pool.h"
#include "Player_Controller.h"

#include "Npc.h"

#include "Chat_Manager.h"
#include "BindShaderDesc.h"
#include "UI_Manager.h"
#include "UI_SpeechBubble.h"
#include "UI_InGame_NamePlate.h"
#include "UI_Inventory.h"
#include "Effect.h"
#include "Effect_Trail.h"

#include "Item.h"
#include "Item_Manager.h"

#include "Party.h"
#include "Damage_Manager.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Player", OBJ_TYPE::PLAYER)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_bControl = Desc->bControl;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_szNickName = Desc->szNickName;
	m_iWeaponIndex = Desc->iWeaponIndex;
	m_iCurrLevel = Desc->iCurrLevel;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;


	m_pRigidBody->SetMass(2.0f);

	m_tCullingSphere.Radius = 2.0f;

	Reset_Triangle();

	if (FAILED(Ready_SpeechBuble()))
		return E_FAIL;
	
	if (FAILED(Ready_NamePlate()))
		return E_FAIL;

	if (m_bControl == false)
	{
		if (FAILED(Ready_Item_NoneControl(Desc->pItemCodes)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Ready_Inventory()))
			return E_FAIL;
	}

	m_pRigidBody->Set_Gravity(false);

	m_tPCStatDesc.iMaxMp = 1000;
	m_tPCStatDesc.iCurMp = m_tPCStatDesc.iMaxMp;

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	if (true == m_bNavi)
	{
		CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);
	}
		
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
}

void CPlayer::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
		m_PlayAnimation.get();

	Set_EffectPos();
	m_pModelCom->Set_ToRootPos(m_pTransformCom);

	for (auto& pPart : m_Parts)
	{
		if (nullptr == pPart.second) continue;

		pPart.second->Tick(fTimeDelta);
	}

	if (nullptr == m_pRendererCom)
		return;
	{
		READ_LOCK
			for (auto& CollisionStay : m_CollisionList)
				OnCollisionStay(CollisionStay.iColLayer, CollisionStay.pCollider);
	}

	CullingObject();

	for (auto& pPart : m_Parts)
	{
		if (nullptr == pPart.second) continue;

		pPart.second->LateTick(fTimeDelta);
	}

	if (m_bControl)
	{
		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		CGameInstance::GetInstance()->Update_LightMatrix(vPos);
	}

	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;
		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
		}
	}

	if ((KEY_HOLD(KEY::CTRL)) && (KEY_TAP(KEY::RBTN)))
	{	
		Get_Picking_NonePlayer();
	}
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}

HRESULT CPlayer::Render_Debug()
{
	for (auto& Colider : m_Coliders)
	{
		if (Colider.second->IsActive())
		{
			Colider.second->DebugRender();
		}

	}

	return S_OK;
}

void CPlayer::Find_NearTarget()
{
	m_pNearTarget = nullptr;
	CGameObject* pBoss = CGameInstance::GetInstance()->Find_NearGameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_BOSS, this);

	if (pBoss)
		m_pNearTarget = pBoss;
	else
	{
		m_pNearTarget = CGameInstance::GetInstance()->Find_NearGameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_MONSTER, this);
	}
}

void CPlayer::Send_NearTarget()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Protocol::S_NEARTARGET pkt;

	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilayer(m_iLayer);

	if (m_pNearTarget)
	{
		pkt.set_itargetobjectid(m_pNearTarget->Get_ObjectID());
		pkt.set_itargetobjectlayer(m_pNearTarget->Get_ObjectLayer());
	}
	else
	{
		pkt.set_itargetobjectid(-1);
		pkt.set_itargetobjectlayer(-1);
	}


	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);

	Safe_Release(pGameInstance);

}

void CPlayer::Go_Straight(_float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}

void CPlayer::Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->LookAt_Lerp(vDir, 5.0f, fTimeDelta);
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}


void CPlayer::Follow_ServerPos(_float fDistance, _float fLerpSpeed)
{
	Vec3 vCurrPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	Vec3 vCurrUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	Matrix matTargetWorld = m_matTargetWorld;

	{
		Vec3 vServerPos(matTargetWorld.m[3]);

		Vec3 vDistance = vServerPos - vCurrPos;
		if (vDistance.Length() > fDistance)
		{
			vCurrPos = Vec3::Lerp(vCurrPos, vServerPos, fLerpSpeed);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCurrPos);
		}
	}

	{
		Vec3 vServerUp(matTargetWorld.m[1]);

		Vec3 vDistance = vServerUp - vCurrUp;
		if (vDistance.Length() > 0.0001f)
		{
			vCurrUp = Vec3::Lerp(vCurrUp, vServerUp, fLerpSpeed * 1.5f);
			m_pTransformCom->Set_Up(vCurrUp);
		}
	}

}

void CPlayer::Set_PlayerUp(_float fTimeDelta)
{
	Vec3 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	vUp.Normalize();
	Vec3 vTargetUp(0.0f, 1.0f, 0.0f);

	if (vUp.Dot(vTargetUp) != 1.0f)
	{
		Vec3 vDir = vTargetUp - vUp;

		_float fDistance = vDir.Length();
		_float fSpeed = 8.0f * fTimeDelta;

		if (fDistance <= fSpeed)
		{
			vUp = vTargetUp;
		}
		else
		{
			vDir.Normalize();
			vUp += vDir * fSpeed;
		}


		m_pTransformCom->Set_Up(vUp);
	}

}

void CPlayer::Body_Collision(CGameObject* pObject)
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Vec3 vOtherPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	Vec3 vDir = vPos - vOtherPos;
	_float fDistance = vDir.Length();


	if (fDistance < 0.5f)
	{
		vDir.Normalize();
		Vec3 vTargetPos = vOtherPos + vDir * 0.5f;
		vPos = Vec3::Lerp(vPos, vTargetPos, 0.2f);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

void CPlayer::Hit_Attack(CCollider* pCollider)
{
	
}

void CPlayer::Set_SlowMotion(_bool bSlow)
{
	if (bSlow)
	{
		if (m_iSlowMotionCount <= 0)
		{
			m_fAttackMoveSpeed = 0.1f;
			m_fAnimationSpeed = 0.01f;
			m_pRigidBody->Set_Active(false);
		}

		m_iSlowMotionCount++;
	}
	else
	{
		--m_iSlowMotionCount;

		if (m_iSlowMotionCount <= 0)
		{
			m_fAttackMoveSpeed = 8.0f;
			m_fAnimationSpeed = 1.0f;
			m_pRigidBody->Set_Active(true);
			m_iSlowMotionCount = 0;
		}
	}

	if (m_bControl)
		Send_SlowMotion(bSlow);
}

void CPlayer::Set_NickName(const wstring& szNickName)
{
	m_szNickName = szNickName;
}


_bool CPlayer::Get_CellPickingPos(Vec3& vPickPos)
{
	if (TEXT("Idle") || TEXT("Run"))
	{
		_float fDist = 10.0f;
		_float fPickDist = 10.0f;
		Vec3 ClickPos;
		m_vecNpcs = m_pGameInstance->Find_GameObjects(m_iCurrLevel, (_uint)LAYER_TYPE::LAYER_NPC);
		for (auto& pNpc : m_vecNpcs)
		{
			if ((_uint)CNpc::NPCTYPE::FUNCTION == static_cast<CNpc*>(pNpc)->Get_NpcType())
			{
				if (true == static_cast<CNpc*>(pNpc)->Intersect_Mouse(ClickPos, fDist))
				{
					if (fPickDist >= fDist)
					{
						fPickDist = fDist;
						vPickPos = ClickPos;
						m_IsClickNpc = true;
					}
				}
			}
		}

		if (true == m_IsClickNpc)
			return true;
	}
	m_IsClickNpc = false;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	SimpleMath::Vector3 vMousePos;
	vMousePos.x = _float(pt.x / (g_iWinSizeX * .5f) - 1);
	vMousePos.y = _float(pt.y / (g_iWinSizeY * -.5f) + 1);
	vMousePos.z = 0.f;

	Matrix matProjInv = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	matProjInv = matProjInv.Invert();

	vMousePos = XMVector3TransformCoord(vMousePos, matProjInv);

	Vec3 vRayPos = Vec3(0.f, 0.f, 0.f);
	Vec3 vRayDir = vMousePos - vRayPos;

	Matrix matViewInv = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	matViewInv = matViewInv.Invert();

	vRayPos = XMVector3TransformCoord(vRayPos, matViewInv);
	vRayDir = XMVector3TransformNormal(vRayDir, matViewInv);

	return CNavigationMgr::GetInstance()->Picking_Cell(m_iCurrLevel, vRayPos, vRayDir, vPickPos);
}

void CPlayer::Get_Picking_NonePlayer()
{
	if (TEXT("Idle") || TEXT("Run"))
	{
		m_vecPlayers = m_pGameInstance->Find_GameObjects(m_iCurrLevel, (_uint)LAYER_TYPE::LAYER_PLAYER);
		for (auto& pPlayer : m_vecPlayers)
		{
			if (false == static_cast<CPlayer*>(pPlayer)->Is_Control())
			{
				if (true == static_cast<CPlayer*>(pPlayer)->Intersect_Mouse())
				{
					m_IsClickPlayer = true;
					CUI_Manager::GetInstance()->Set_EntranceParty_Player(m_IsClickPlayer, static_cast<CPlayer*>(pPlayer));
				}
			}
		}
	}
	m_IsClickPlayer = false;
	CUI_Manager::GetInstance()->Set_EntranceParty_Player(m_IsClickPlayer, nullptr);
}

_bool CPlayer::Is_PartyLeader()
{
	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return false;

	if (nullptr == pPlayer->Get_Party())
		return false;

	if (pPlayer->Get_Party()->Get_PartyMembers().front() == m_iObjectID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

HRESULT CPlayer::Add_Item(wstring strItemTag, CItem* pItem)
{
	auto& iter = m_ItemTags.find(strItemTag);
	if (iter == m_ItemTags.end())
	{
		//추가하는 코드
		Add_Item_to_EmptySlot(strItemTag, pItem);
	}
	else if (false == (*iter).second.bOwn)
	{
		//추가하는 코드
		Add_Item_to_EmptySlot(strItemTag, pItem);

	}
	else if (true == (*iter).second.bOwn)
	{
		m_vecItemSlots[(*iter).second.iIndex].vecItems.push_back(pItem);
	}

	if (nullptr != m_pUI_Inventory)
	{
		m_pUI_Inventory->Update_Used_Item();
	}

	return S_OK;
}

HRESULT CPlayer::Use_Item(wstring strItemTag, _uint iSize, _bool bSend)
{
	auto& iter = m_ItemTags.find(strItemTag);
	if ((iter == m_ItemTags.end())||(false == (*iter).second.bOwn))
	{
		return E_FAIL;
	}
	else
	{
		CItem* pItem = m_vecItemSlots[(*iter).second.iIndex].vecItems.back();

		if ((_uint)CItem::TYPE::CONSUM == m_vecItemSlots[(*iter).second.iIndex].vecItems.back()->Get_ItemType())
		{
			if (1 == m_vecItemSlots[(*iter).second.iIndex].vecItems.size())
			{
				iter->second.bOwn = false;
				m_vecItemSlots[(*iter).second.iIndex].vecItems.clear();
				iter->second.iIndex = -1;

			}
			else
			{
				m_vecItemSlots[(*iter).second.iIndex].vecItems.pop_back();
			}

			pItem->Use_Item(this);
		}
		else if ((_uint)CItem::TYPE::EQUIP == m_vecItemSlots[(*iter).second.iIndex].vecItems.back()->Get_ItemType())
		{
			(*iter).second.bOwn = false;
			m_vecItemSlots[(*iter).second.iIndex].vecItems.clear();
			(*iter).second.iIndex = -1;
			//모코코 머리가 현재 0번에 들어있는데 인덱스가 -1들어가 터짐

			pItem->Use_Item(this);

			if(bSend == true)
				Send_Equips();
		}
	}
	if (nullptr != m_pUI_Inventory)
		m_pUI_Inventory->Update_Used_Item();

	return S_OK;
}

void CPlayer::Send_Equips()
{
	Protocol::S_CHANGEEQUIP pkt;
	pkt.set_ilevel(m_iCurrLevel);
	pkt.set_iplayerid(m_iObjectID);

	for (_uint i = 0; i < (_uint)PART::_END; ++i)
	{
		if (m_pEqupis[i] != nullptr)
		{
			pkt.add_itemcodes((_uint)m_pEqupis[i]->Get_ItemCode());
		}
		else
		{
			pkt.add_itemcodes(-1);
		}
	}
	
	CServerSessionManager::GetInstance()->Send(CClientPacketHandler::MakeSendBuffer(pkt));
}

void CPlayer::Add_Effect(const wstring& szEffectName, CEffect* pEffect)
{
	m_Effects.emplace(szEffectName, pEffect);
}

void CPlayer::Delete_Effect(const wstring& szEffectName)
{
	auto iter = m_Effects.find(szEffectName);
	if (iter == m_Effects.end())
		return;

	(*iter).second->EffectEnd();
	m_Effects.erase(iter);
}

void CPlayer::Delete_Effect_Trail(const wstring& szEffectName, _float fRemainTime)
{
	auto iter = m_Effects.find(szEffectName);
	if (iter == m_Effects.end())
		return;

	dynamic_cast<CEffect_Trail*>((*iter).second)->TrailEnd(fRemainTime);
	m_Effects.erase(iter);
}

CEffect* CPlayer::Get_Effect(const wstring& szEffectName)
{
	auto iter = m_Effects.find(szEffectName);
	if (iter == m_Effects.end())
		return nullptr;

	return (*iter).second;
}

void CPlayer::Set_Several_Weapon_RenderState(CPartObject::PARTS ePart, _bool Is_Render)
{
	if (nullptr == m_Parts[ePart])
		return;

	m_Parts[ePart]->Set_Render(Is_Render);
}

void CPlayer::Load_WorldMatrix(Matrix& matWorld)
{
	matWorld = m_pTransformCom->Get_WorldMatrix();
}

void CPlayer::Swap_Items_In_Inventory(wstring MoveItemTag, wstring OriginItemTag)
{
	auto iter_Temp = m_ItemTags.find(MoveItemTag);
	if (iter_Temp == m_ItemTags.end())
		return;
	auto iter_Dst = m_ItemTags.find(OriginItemTag);
	if (iter_Dst == m_ItemTags.end())
		return;

	iter_swap(&m_vecItemSlots[(*iter_Temp).second.iIndex],
		&m_vecItemSlots[(*iter_Dst).second.iIndex]);

	ITEMTAG_DESC ItemDesc;
	ItemDesc.bOwn = (*iter_Temp).second.bOwn;
	ItemDesc.iIndex = (*iter_Temp).second.iIndex;
	(*iter_Temp).second = (*iter_Dst).second;
	(*iter_Dst).second = ItemDesc;

	CUI_Manager::GetInstance()->Reset_ItemIcon();
	m_pUI_Inventory->Update_Used_Item();
}

void CPlayer::Swap_Items_In_Inventory(wstring MoveItemTag, _uint iSlotIndex)
{
	auto iter_Temp = m_ItemTags.find(MoveItemTag);
	if (iter_Temp == m_ItemTags.end())
		return;

	iter_swap(&m_vecItemSlots[(*iter_Temp).second.iIndex],
		&m_vecItemSlots[iSlotIndex]);
	
	(*iter_Temp).second.iIndex = iSlotIndex;

	CUI_Manager::GetInstance()->Reset_ItemIcon();
	m_pUI_Inventory->Update_Used_Item();
}

HRESULT CPlayer::Equipment_Index_Reset(wstring strItemTag)
{
	auto& iter = m_ItemTags.find(strItemTag);
	if ((iter == m_ItemTags.end()) || (false == (*iter).second.bOwn))
	{
		return E_FAIL;
	}
	else
		(*iter).second.iIndex = -1;

	return S_OK;
}

HRESULT CPlayer::Equipment_Index_Reallocated(wstring strItemTag)
{
	auto& iter = m_ItemTags.find(strItemTag);
	if ((iter == m_ItemTags.end()) || (false == (*iter).second.bOwn))
	{
		return E_FAIL;
	}
	else
	{
		_bool	bAssigned = false;
		size_t smallestUnusedIndex = std::numeric_limits<size_t>::max();
		for (auto iter_Dst : m_ItemTags)
		{
			for (size_t i = 0; i < 64; i++)
			{
				if (i == iter_Dst.second.iIndex)
				{
					bAssigned = true;
					break;
				}
				else
				{
					bAssigned = false;
					if (i < smallestUnusedIndex)
					{
						smallestUnusedIndex = i;
					}
				}
			}
		}
		(*iter).second.iIndex = (_int)smallestUnusedIndex;
	}
	return S_OK;
}

_bool CPlayer::Intersect_Mouse()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_float viewX = (+2.0f * pt.x / g_iWinSizeX - 1.0f) / m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ)(0, 0);
	_float viewY = (-2.0f * pt.y / g_iWinSizeY + 1.0f) / m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ)(1, 1);

	Vec4 vRayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4 vRayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

	Vec3 vWorldRayOrigin = XMVector3TransformCoord(vRayOrigin, m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW));
	Vec3 vWorldRayDir = XMVector3TransformNormal(vRayDir, m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW));
	vWorldRayDir.Normalize();

	Ray MouseRay = Ray(vWorldRayOrigin, vWorldRayDir);

	_float fDist;
	for (auto& Colider : m_Coliders)
	{
		if (Colider.second->IsActive())
		{
			return Colider.second->Intersects(MouseRay, fDist);
		}

	}

	return false;
}

HRESULT CPlayer::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_State */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachine)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = true;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderBody"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		if (pCollider)
		{
			{
				CCollider::ColliderInfo tChildColliderInfo;
				tChildColliderInfo.m_bActive = true;
				tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
				COBBCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderBodyChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, pCollider);
		}
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CPlayer::Ready_SpeechBuble()
{
	m_pSpeechBuble = CPool<CUI_SpeechBubble>::Get_Obj();

	if (m_pSpeechBuble == nullptr)
		return E_FAIL;

	m_pSpeechBuble->Set_Host(this);

	return S_OK;
}

HRESULT CPlayer::Ready_NamePlate()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_bControl)
	{
		m_pNamePlate = static_cast<CUI_InGame_NamePlate*>(pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC,
			(_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_NamePlate"), this));

		if (m_pNamePlate == nullptr)
			return E_FAIL;
		else
			CUI_Manager::GetInstance()->Add_UI(LEVEL_STATIC, m_pNamePlate);
	}
	else
	{
		m_pNamePlate = static_cast<CUI_InGame_NamePlate*>(pGameInstance->Add_GameObject(m_iCurrLevel,
			(_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_NamePlate"), this));

		if (m_pNamePlate == nullptr)
			return E_FAIL;
		else
			CUI_Manager::GetInstance()->Add_UI((LEVELID)m_iCurrLevel, m_pNamePlate);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CPlayer::Ready_Inventory()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance); 

	m_vecItemSlots.resize(64);

	m_pUI_Inventory = static_cast<CUI_Inventory*>(pGameInstance->
		Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_Inventory"),this));
	if (nullptr == m_pUI_Inventory)
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}


HRESULT CPlayer::Ready_Item_NoneControl(_int* ItemCodes)
{
	CItem_Manager* pItem_Manager = GET_INSTANCE(CItem_Manager);

	for (_uint i = 0; i < (_uint)PART::_END; ++i)
	{
		_int iItemCode = *(ItemCodes + i);

		if (iItemCode != -1)
		{
			CItem* pItem = pItem_Manager->Get_Item((ITEMCODE)iItemCode);

			if (pItem == nullptr)
				return E_FAIL;

			pItem->Use_Item(this);
		}
	}

	Safe_Release(pItem_Manager);
	return S_OK;
}

void CPlayer::CullingObject()
{
	if (m_bControl)
	{
		if (m_bRender)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
			m_pRendererCom->Add_DebugObject(this);
		}

		return;
	}

	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_tCullingSphere.Center = vPos;

	const BoundingFrustum& tCamFrustum = CGameInstance::GetInstance()->Get_CamFrustum();

	if (tCamFrustum.Intersects(m_tCullingSphere) == false)
		return;

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
		m_pRendererCom->Add_DebugObject(this);
	}

}

void CPlayer::Update_Skill(SKILLINFO& tSkill, _float fTimeDelta)
{
	if (tSkill.m_bReady == false)
	{
		tSkill.m_fCurrCoolTime += fTimeDelta;

		if (tSkill.m_fCurrCoolTime > tSkill.m_fCoolTime)
		{
			tSkill.m_fCurrCoolTime = 0.0f;
			tSkill.m_bReady = true;
		}
	}
}

void CPlayer::Set_EffectPos()
{
	_uint iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_effectname"));
	Matrix matEffect = m_pModelCom->Get_CombinedMatrix(iBoneIndex) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	matEffect *= m_pTransformCom->Get_WorldMatrix();
	memcpy(&m_vEffectPos, matEffect.m[3], sizeof(Vec3));
	Matrix ViewMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	Matrix ProjMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
	m_vEffectPos = XMVector3TransformCoord(m_vEffectPos, ViewMatrix);
	m_vEffectPos = XMVector3TransformCoord(m_vEffectPos, ProjMatrix);
}

void CPlayer::Add_Item_to_EmptySlot(const wstring& strItemTag, class CItem* pItem)
{
	for (size_t i = 0; i < m_vecItemSlots.size(); i++)
	{
		if (1 > m_vecItemSlots[i].vecItems.size())
		{
			m_vecItemSlots[i].vecItems.push_back(pItem);
			m_ItemTags[strItemTag].bOwn = true;
			m_ItemTags[strItemTag].iIndex = (_int)i;
			break;
		}
	}
}
void CPlayer::Show_Damage(_uint iDamage, _bool IsCritical)
{
	CDamage_Manager::GetInstance()->Print_DamageFont(this, 0.4f, 1.2f, 2.0f, IsCritical, iDamage);
}
_bool CPlayer::Is_CancelState()
{
	wstring strNextState = Get_ServerState();

	if (TEXT("Hit") == strNextState ||
		TEXT("Hit_Common") == strNextState ||
		TEXT("Dead_Start") == strNextState ||
		TEXT("Dead_End") == strNextState ||
		TEXT("Fall") == strNextState ||
		TEXT("Grabbed") == strNextState ||
		TEXT("Stop") == strNextState ||
		TEXT("StandDash") == strNextState)
			return true;

	return false;
}

void CPlayer::Show_SpeechBuble(const wstring& szChat)
{
	m_pSpeechBuble->Active_SpeechBuble(szChat);
}

void CPlayer::Reset_Level()
{
	m_pNamePlate->Print_InGame_Name();
}


void CPlayer::Store_Part()
{
	for (auto& Part : m_Parts)
	{
		if (nullptr == Part.second) continue;
		if (false == Part.second->Is_Render()) continue;

		static_cast<CPartObject*>(Part.second)->Store_Socket();
	}
}

void CPlayer::UnStore_Part()
{
	for (auto& Part : m_Parts)
	{
		if (nullptr == Part.second) continue;
		if (false == Part.second->Is_Render()) continue;

		static_cast<CPartObject*>(Part.second)->UnStore_Socket();
	}
}

void CPlayer::Stop_Update_Part(_bool bUpdate)
{
	for (auto& Part : m_Parts)
	{
		if (nullptr == Part.second) continue;
		if (false == Part.second->Is_Render()) continue;

		static_cast<CPartObject*>(Part.second)->Set_UpdateState(bUpdate);
	}
}

void CPlayer::Send_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_uint iCurrLevel = pGameInstance->Get_CurrLevelIndex();
	if (iCurrLevel >= LEVELID::LEVEL_LOADING)
	{
		Safe_Release(pGameInstance);
		return;
	}

	Protocol::S_ANIMATION pkt;
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilevel(iCurrLevel);
	pkt.set_ilayer((int32)LAYER_TYPE::LAYER_PLAYER);
	pkt.set_ianimindex(iAnimIndex);
	pkt.set_fchangetime(fChangeTime);
	pkt.set_ichangeframe(iChangeFrame);
	pkt.set_istartframe(iStartFrame);

	SendBufferRef pBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Get_ServerSession()->Send(pBuffer);

	Safe_Release(pGameInstance);
}

void CPlayer::Send_State(const wstring& szName)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Protocol::S_STATE pkt;
	pkt.set_strstate(CAsUtils::ToString(szName));
	pkt.set_iweaponindex(m_iWeaponIndex);

	auto tPlayer = pkt.mutable_tobject();

	tPlayer->set_ilevel(m_iCurrLevel);
	tPlayer->set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);
	tPlayer->set_iobjectid(m_iObjectID);

	auto vTargetPos = tPlayer->mutable_vtargetpos();
	vTargetPos->Resize(3, 0.0f);
	Vec3 vPlayerTargetPos = m_vTargetPos.load();
	memcpy(vTargetPos->mutable_data(), &vPlayerTargetPos, sizeof(Vec3));

	auto matWorld = tPlayer->mutable_matworld();
	matWorld->Resize(16, 0.0f);
	Matrix matPlayerWorld = m_pTransformCom->Get_WorldMatrix();
	memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));

	if (m_pNearTarget == nullptr)
		pkt.set_itargetobjectid(-1);
	else
	{
		pkt.set_itargetobjectid(m_pNearTarget->Get_ObjectID());
		pkt.set_itargetobjectlayer(m_pNearTarget->Get_ObjectLayer());
	}

	if (m_pHitObject == nullptr)
		pkt.set_ihitobjectid(-1);
	else
	{
		pkt.set_ihitobjectid(m_pHitObject->Get_ObjectID());
		pkt.set_ihitobjectlayer(m_pHitObject->Get_ObjectLayer());
	}

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);

	Safe_Release(pGameInstance);
}

void CPlayer::Send_ColliderState(const _uint& iLayer)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CSphereCollider* pCollider = m_Coliders[iLayer];

	Protocol::S_COLLIDERSTATE pkt;
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_ilayer(m_iLayer);

	pkt.set_icollayer(pCollider->Get_ColLayer());
	pkt.set_bactive(pCollider->IsActive());
	pkt.set_fradius(pCollider->Get_Radius());
	pkt.set_iattacktype(pCollider->Get_AttackType());
	pkt.set_iattack(pCollider->Get_Attack());
	pkt.set_bslow(pCollider->Get_SlowMotion());

	auto vOffset = pkt.mutable_voffset();
	vOffset->Resize(3, 0.0f);
	Vec3 vColliderOffset = pCollider->Get_Offset();
	memcpy(vOffset->mutable_data(), &vColliderOffset, sizeof(Vec3));

	if (pCollider->Get_Child())
	{
		COBBCollider* pChild = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		auto tchild = pkt.add_tchild();


		auto vOffset = tchild->mutable_voffset();
		vOffset->Resize(3, 0.0f);
		Vec3 vColOffset = pChild->Get_Offset();

		memcpy(vOffset->mutable_data(), &vColOffset, sizeof(Vec3));


		auto vScale = tchild->mutable_vscale();
		vScale->Resize(3, 0.0f);
		Vec3 vColScale = pChild->Get_Scale();

		memcpy(vScale->mutable_data(), &vColScale, sizeof(Vec3));
	}

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);

	Safe_Release(pGameInstance);
}

void CPlayer::Send_SlowMotion(_bool bSlow)
{
	Protocol::S_SLOWMOTION pkt;

	pkt.set_ilevel(CGameInstance::GetInstance()->Get_CurrLevelIndex());
	pkt.set_ilayer(m_iLayer);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_bslow(bSlow);

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

void CPlayer::Send_Hp()
{
	Protocol::S_HP pkt;

	pkt.set_ilevel(m_iCurrLevel);
	pkt.set_ilayer(m_iLayer);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ihp(m_tPCStatDesc.iCurHp);
	pkt.set_imaxhp(m_tPCStatDesc.iMaxHp);

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

void CPlayer::Send_EstherGauge()
{
	Protocol::S_ESTHER pkt;

	pkt.set_ilevel(m_iCurrLevel);
	pkt.set_iplayerid(m_iObjectID);
	pkt.set_iesthergage(m_iCurEstherGage);

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

void CPlayer::Set_State(const wstring& szName)
{
	m_szState = szName;
	m_pStateMachine->Change_State(szName);
	Send_State(szName);
}

void CPlayer::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bRootRot, _bool bReverse, _bool bUseY, _bool bIgnoreRoot)
{
	if (nullptr == m_pModelCom)
		return;

	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame, fRootDist, bRootRot, bReverse, bUseY, bIgnoreRoot);
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CPool<CUI_SpeechBubble>::Return_Obj(m_pSpeechBuble);
	m_pSpeechBuble = nullptr;
	if(m_pUI_Inventory)
		m_pUI_Inventory->Set_Dead(true);
	m_pNamePlate->Set_Dead(true);
}
