#include "stdafx.h"
#include "Valtan_BT_Attack_Attack_Base.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"

CValtan_BT_Attack_Attack_Base::CValtan_BT_Attack_Attack_Base()
{
}

void CValtan_BT_Attack_Attack_Base::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack_Base::OnUpdate(const _float& fTimeDelta)
{
	if (m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim() && m_bStart)
	{
		On_FirstAnimStart();
		m_bStart = false;
	}
	if (m_iCurrAnimation == m_iMaxAnimation - 1 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iMaxAnimation - 1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iMaxAnimation - 1].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iMaxAnimation - 1].iAnimIndex) - 3 && m_bEnd)
	{
		On_LastAnimEnd();
		m_bEnd = false;
	}
	if (m_vecAnimDesc[m_iCurrAnimation].bIsLoop && m_vecAnimDesc[m_iCurrAnimation].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && m_vecAnimDesc[m_iCurrAnimation].IsEndInstant)
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		else if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && !m_vecAnimDesc[m_iCurrAnimation].IsEndInstant && m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		m_fLoopTime += fTimeDelta;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) - 3 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		if (m_iCurrAnimation == m_iMaxAnimation - 1)
			return BT_SUCCESS;
		else
		{
			m_iCurrAnimation++;

			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);
			static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[m_iCurrAnimation].strAnimName, m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);
		}
	}
	return BT_RUNNING;
}

void CValtan_BT_Attack_Attack_Base::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack_Base* CValtan_BT_Attack_Attack_Base::Create(void* pArg)
{
	CValtan_BT_Attack_Attack_Base* pInstance = new CValtan_BT_Attack_Attack_Base;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack_Base::Free()
{
	__super::Free();
}