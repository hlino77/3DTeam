#pragma once
#include "Component.h"
#include "Lock.h"

BEGIN(Engine)

class ENGINE_DLL CStateMachine final : public CComponent
{
private:
	CStateMachine(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CStateMachine(const CStateMachine& rhs);
	virtual ~CStateMachine() = default;


	USE_LOCK
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Tick_State(_float fTimeDelta);
	void LateTick_State(_float fTimeDelta);

public:
	HRESULT Add_State(const wstring& strStateTag, class CState* pState);
	HRESULT Change_State(const wstring& strStateTag);

	const wstring& Get_CurState() { return m_strCurState; }
	const wstring& Get_PreState() { return m_strPreState; }
	class CState* Find_State(const wstring& strStateTag);
	
private:
	class CState* m_pCurrState = nullptr;
	unordered_map<wstring, class CState*> m_States;

private:
	

private:
	wstring m_strCurState;
	wstring m_strPreState;

public:
	static CStateMachine* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(CGameObject * pObject, void* pArg) override;
	virtual void Free();
};

END

