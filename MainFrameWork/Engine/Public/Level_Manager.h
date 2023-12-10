#pragma once

#include "Base.h"
#include "Lock.h"
/* Open_Level*/
/* ���� ���ӳ����� Ȱ���ϰ� �ִ� ������ü�� �ּҸ� �����Ѵ�. */
/* ���� ��ü�ÿ� ���� ������ �ı��Ѵ�. + ���� ���������� �������� �ڿ����� �����Ѵ�. */

/* �����ϰ� �ִ� ������ ƽ�� �ݺ������� ȣ�����ش�. */


BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;


	USE_LOCK
public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	HRESULT Render_Debug();


	void Set_Loading(_bool bLoading) { m_bLoading = bLoading; }
	void Set_LoadingNext(_uint iNextLevel) { m_iLoadingNext = iNextLevel; }

	class CLevel* Get_CurrLevel() { return m_pCurrentLevel; }

	_uint Get_CurrLevelIndex();
public:
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);

private:
	_uint						m_iCurrentLevelIndex = { 0 };
	_uint						m_iNextLevelIndex = { 0 };

	class CLevel*				m_pCurrentLevel = { nullptr };
	class CLevel*				m_pReserveLevel = { nullptr };


	_bool						m_bLoading = false;
	_uint						m_iLoadingNext = 0;
public:
	virtual void Free() override;
};

END