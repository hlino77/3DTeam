#include "StateMachine.h"
#include "State.h"
CStateMachine::CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent(pDevice, pContext)
{
}

CStateMachine::CStateMachine(const CStateMachine& rhs)
    : CComponent(rhs)
	, m_States(rhs.m_States)
{

}

HRESULT CStateMachine::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStateMachine::Initialize(void* pArg)
{
    return S_OK;
}

void CStateMachine::Tick_State(_float fTimeDelta)
{
	if (nullptr == m_pCurrState)
		return;

	m_pCurrState->Tick_State(fTimeDelta);
}

void CStateMachine::LateTick_State(_float fTimeDelta)
{
}

HRESULT CStateMachine::Add_State(const wstring& strStateTag, CState* pState)
{
	auto iter = m_States.find(pState->Get_StateName());

	if (iter != m_States.end())
		return E_FAIL;

	m_States.emplace(pState->Get_StateName(), pState);

	if (nullptr == m_pCurrState)
	{
		if (FAILED(Change_State(pState->Get_StateName())))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStateMachine::Change_State(const wstring& strStateTag)
{
	WRITE_LOCK

	CState* pState = Find_State(strStateTag);
	if (nullptr == pState)
		return E_FAIL;

	if (nullptr != m_pCurrState)
	{
		m_pCurrState->Exit_State();
		m_strPreState = m_pCurrState->Get_StateName();
	}
		
	
	m_pCurrState = pState;
	m_strCurState = m_pCurrState->Get_StateName();
	m_pCurrState->Enter_State();

	return S_OK;
}

CState* CStateMachine::Find_State(const wstring& strStateTag)
{
	auto iter = m_States.find(strStateTag);

	if (iter == m_States.end())
		return nullptr;

	return iter->second;
}

CStateMachine* CStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStateMachine* pInstance = new CStateMachine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CStateMachine::Clone(CGameObject* pObject, void* pArg)
{
	CStateMachine* pInstance = new CStateMachine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CStateMachine::Free()
{
	for (auto& State : m_States)
		Safe_Delete(State.second);
	m_States.clear();

	__super::Free();
}
