#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

class CController_MG final : public CPlayer_Controller
{
public:

private:
	CController_MG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CController_MG(const CController_MG& rhs);
	virtual ~CController_MG() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;

public:
	/* �Ƶ� ���� �Լ� */
	_uint			Is_MG_Identity();
	const _int&		Get_IdenGage() { return m_iIdentityGage; }
	const _int&		Get_IdenMaxGauge() { return m_iMaxGage; }

public:
	virtual HRESULT	Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	void			Set_Attack_Desc(_uint iIndex) { m_AttackDesc = m_Attack_Desces[iIndex]; }
	CPlayer_Skill*  Get_PlayerSkill_MG(SKILL_KEY eKey);

public:
	virtual void	Get_AttackMessage() { Attack(); }
	void			Get_MG_IdentityMessage();
	void			Get_MG_IdenSkillMessage(_float fCoolTime) { m_fCoolTime[SKILL_KEY::Z] = fCoolTime; m_iIdentityGage -= 10; }

	void			Increase_IdenGage(_uint iGage);

private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack() override;
	virtual void	SkillAttack(SKILL_KEY eKey, Vec3 vPos) override;
	virtual void	Hit(CGameObject* pHitObject) override;
	virtual void	Skill_CoolTime(const _float& fTimeDelta) override;

private:
	PROJECTILE_DESC	m_Attack_Desces[10];

	_int	m_iIdentityGage = { 0 };
	_int	m_iMaxGage = { 300 };
	_float  m_fTimdeAcc = { 0.f };

	_uint	m_iBubble = { 0 };

public:
	static CController_MG* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END