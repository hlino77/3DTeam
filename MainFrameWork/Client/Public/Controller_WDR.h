#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

class CController_WDR final : public CPlayer_Controller
{
public:

private:
	CController_WDR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CController_WDR(const CController_WDR& rhs);
	virtual ~CController_WDR() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;

public:
	virtual _bool		Is_EstherSkill() override;

	/* �Ƶ� ���� �Լ� */
	void			Check_Iden_State(_float fTimeDelta);
	_bool			Is_Identity();
	const _bool&	Is_In_Identity() { return m_IsIdentity; }
	const _float&	Get_IdenGage() { return m_fIdentityGage; }
	const _float&	Get_MaxGage() { return m_fMaxGage; }
	const _uint&	Get_MarbleCnt() { return m_iMarbleCnt; }
	const	_float& Get_IdenSkillCool() { return m_fIdenSkillCool; }

public:
	virtual HRESULT	Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	void			Set_Attack_Desc(_uint iIndex) { m_AttackDesc = m_Attack_Desces[iIndex]; }

public:
	virtual void	Get_AttackMessage(Vec3 vPos = Vec3()) { Attack(vPos); }
	virtual void	Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos = Vec3());
	virtual void	Get_GrabMessage(CGameObject* pGrabber);
	virtual void	Get_GrabEndMessage();
	virtual void	Get_DeadMessage();
	virtual void	Get_EshterGageUseMessage() override;

	void			Get_WDR_IdentityMessage();
	void			Get_WDR_IdenSkillMessage(_float fCoolTime) { m_fCoolTime[SKILL_KEY::Z] = m_fIdenSkillCool; m_fIdentityGage -= 5.f; }
	void			Get_AddMarbleMessage(_uint iMarbleCnt) { m_IsAddMarble = true;  m_iReserveMarbleCnt = iMarbleCnt; }
	void			Get_ResetMarbleMessage() { m_IsAddMarble = false;  m_iReserveMarbleCnt = 0; };
	void			Get_UseMarbleMessage();
	void			Get_HitMarbleMessage();

public:

private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack(Vec3 vPos) override;
	virtual void	SkillAttack(SKILL_KEY eKey, Vec3 vPos) override;
	virtual void	Skill_CoolTime(const _float& fTimeDelta) override;

	virtual void	EstherSkill(_uint iIndex) override;
	virtual void	Esther_Refill(_float fTimeDelta) override;

private:
	PROJECTILE_DESC	m_Attack_Desces[10];

	_bool	m_IsIdentity = { false };
	_float	m_fIdentityGage = { 0.f };
	_float	m_fMaxGage = { 30.f };
	_float  m_fTimdeAcc = { 0.f };

	_uint	m_iMarbleCnt = { 0 };

	_bool	m_IsAddMarble = { false };
	_uint	m_iReserveMarbleCnt = { 0 };

	_float	m_fIdenSkillCool = { 5.f };

public:
	static CController_WDR* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END