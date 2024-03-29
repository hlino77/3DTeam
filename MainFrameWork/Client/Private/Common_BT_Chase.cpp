#include "stdafx.h"
#include "Common_BT_Chase.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CCommon_BT_Chase::CCommon_BT_Chase()
{
}

void CCommon_BT_Chase::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Chase::OnUpdate(const _float& fTimeDelta)
{


	if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance() <1.f)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[1].iAnimIndex &&m_pGameObject->Get_ModelCom()->Get_NextAnim() != m_vecAnimDesc[1].iAnimIndex)
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[1].iAnimIndex, m_vecAnimDesc[1].fChangeTime,
			m_vecAnimDesc[1].iStartFrame, m_vecAnimDesc[1].iChangeFrame);
		return BT_RUNNING;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[0].iAnimIndex&& m_pGameObject->Get_ModelCom()->Get_NextAnim()	!= m_vecAnimDesc[0].iAnimIndex)
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[0].iAnimIndex, m_vecAnimDesc[0].fChangeTime,
			m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame);
	_float fSpeed = 1.5f * static_cast<CMonster*>(m_pGameObject)->Get_MoveSpeed();
	if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance() < 2.f)
		static_cast<CMonster*>(m_pGameObject)->Move_Dir(static_cast<CMonster*>(m_pGameObject)->Get_Target_Direction(), fSpeed, fTimeDelta);
	else
		static_cast<CMonster*>(m_pGameObject)->Move_Dir(static_cast<CMonster*>(m_pGameObject)->Get_Target_RandomDirection(), fSpeed, fTimeDelta);
	
	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			Add_Sound(0, 0, L"Effect", L"Pawn_29", 0 , 0.2f);
			if (CSound_Manager::GetInstance()->Is_Channel_Playing(L"Pawn_3"))
				Add_Sound_Channel(L"Effect", L"Pawn_3",0.2f);
		}
	}
	return BT_RUNNING;
}

void CCommon_BT_Chase::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Chase* CCommon_BT_Chase::Create(void* pArg)
{
	CCommon_BT_Chase* pInstance = new CCommon_BT_Chase;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Chase::Free()
{
	__super::Free();
}
