#include "stdafx.h"
#include "GameInstance.h"
#include "Boss_Server.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Skill_Server.h"
#include "BehaviorTree.h"

CBoss_Server::CBoss_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster_Server(pDevice, pContext)
{
}

CBoss_Server::CBoss_Server(const CBoss_Server& rhs)
	: CMonster_Server(rhs)
{
}

HRESULT CBoss_Server::Initialize_Prototype()
{
	m_strObjectTag = L"Boss";
	m_iObjType = OBJ_TYPE::BOSS;
    return S_OK;
}

HRESULT CBoss_Server::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_szModelName = Desc->strFileName;
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_iCurrLevel = Desc->iLevel;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;

	m_fNoticeRange = 20.f;
	
	m_pRigidBody->SetMass(2.0f);

	return S_OK;
}

void CBoss_Server::Tick(_float fTimeDelta)
{
	CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);

	m_fSkillCoolDown += fTimeDelta;
		Find_NearTarget(fTimeDelta);
	if(m_pBehaviorTree!= nullptr)
		m_pBehaviorTree->Tick(fTimeDelta);
	m_pRigidBody->Tick(fTimeDelta);
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
}

void CBoss_Server::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
	{
		m_PlayAnimation.get();
		Set_to_RootPosition(fTimeDelta, m_fRootTargetDistance);
	}
	{
		READ_LOCK
			for (auto& CollisionStay : m_CollisionList)
				OnCollisionStay(CollisionStay.iColLayer, CollisionStay.pCollider);
	}

	Set_Colliders(fTimeDelta);

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBoss_Server::Render()
{

	return S_OK;
}

void CBoss_Server::Set_SlowMotion(_bool bSlow)
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

	Send_SlowMotion(bSlow);
}

void CBoss_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	
}

void CBoss_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CBoss_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	
}

void CBoss_Server::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration)
{
	WRITE_LOCK

		if (!m_bInvincible)
			m_iHp -= iDamage;


	if ((_uint)STATUSEFFECT::COUNTER == iStatusEffect && m_IsCounterSkill)
	{
		m_IsHit = true;
		m_IsCounterSkill = false;
		m_IsCountered = true;
	}
	m_fStatusEffects[iStatusEffect] += fDuration;
	if (m_iHp < 1.f)
		m_IsHit = true;

	Send_Collision(iDamage, vHitPos, STATUSEFFECT(iStatusEffect), fForce, fDuration);
}

void CBoss_Server::Move_to_SpawnPosition()
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vSpawnPosition);
}

HRESULT CBoss_Server::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	///* For.Com_State */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachine)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Boss_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	

	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

HRESULT CBoss_Server::Ready_BehaviourTree()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_BehaviorTree"), TEXT("Com_Behavior"), (CComponent**)&m_pBehaviorTree)))
		return E_FAIL;
	return S_OK;
}



void CBoss_Server::Set_Colliders(_float fTimeDelta)
{

}

HRESULT CBoss_Server::Ready_Coliders()
{


	return S_OK;
}



CGameObject* CBoss_Server::Clone(void* pArg)
{
	CBoss_Server* pInstance = new CBoss_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CMonster" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Server::Free()
{
	__super::Free();



	Safe_Release(m_pModelCom);
	Safe_Release(m_pBehaviorTree);
	Safe_Release(m_pTransformCom);
}
