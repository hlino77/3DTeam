#pragma once
#include "Component.h"
#include "Transform.h"
#include "RigidBody.h"
#include "State.h"

BEGIN(Engine)

class ENGINE_DLL CPlayer_Controller abstract : public CComponent
{
public:
	enum SKILL_KEY { SPACE, Q, W, E , R, A, S, D , F, _END };

public:
	typedef struct tagControllerDesc
	{
		CGameObject*	pOwner = { nullptr };
		CTransform*		pOwnerTransform = { nullptr };
		CRigidBody*		pOwnerRigidBody = { nullptr };
	}CONTROLL_DESC;

protected:
	CPlayer_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Controller(const CPlayer_Controller& rhs);
	virtual ~CPlayer_Controller() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual void	DebugRender();

public:
	_bool		Is_Tap(KEY eKey);
	_bool		Is_Hold(KEY eKey);
	_bool		Is_HoldorTap(KEY eKey);
	_bool		Is_Away(KEY eKey);

public:
	_bool		Is_Idle();
	_bool		Is_Run();
	_bool		Is_Skill();
	_bool		Is_Interect();
	_bool		Is_Dash();
	_bool		Is_Attack();

	void		Get_MoveMessage(Vec3 vPos)					 { m_vNextMove = vPos;	m_bStop = false; }
	void		Get_StopMessage()							 { m_vNextMove = Vec3(); m_bStop = true;}
	void		Get_LerpLookMessage(Vec3 vAt, _float fSpeed = 20.f) { m_vNextMove = vAt; m_fLerpLook_Speed = fSpeed, m_bStop = true; }
	void		Get_LookMessage(Vec3 vAt)					 { Look(vAt); }
	void		Get_AttackMessage()							 { Attack(); }
	void		Get_SkillMessage(SKILL_KEY eKey)			 { Skill(eKey); }
	void		Get_HitMessage()							 { Hit(); }
	void		Get_DashMessage(Vec3 vPos, _float fCoolTime) { Look(vPos); m_fCoolTime[SKILL_KEY::SPACE] = fCoolTime; }

public:
	_bool		Is_Stop() { return m_bMoveStop; }

	/* ��ų �Լ� */
public:
	HRESULT					Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	HRESULT					Set_SkillSuccess(SKILL_KEY eKey, _bool IsSuccess);

public:
	const _bool&			Is_SkillSuccess(SKILL_KEY eKey);

	SKILL_KEY				Get_Selected_Skill() { return m_eSelectedSkill; }
	class CPlayer_Skill*	Get_PlayerSkill(SKILL_KEY eKey) { return m_pSkills[eKey]; }
	const wstring&		    Get_SkillStartName(SKILL_KEY eKey);

public:
	/* �������� ������ */
	_bool			Pick(_uint screenX, _uint screenY, Vec3 & pickPos, _float & distance);

protected:
	virtual void	Move(const _float& fTimeDelta);
	virtual void	Look_Lerp(const _float& fTimeDelta);
	virtual void	Look(Vec3 vAt);
	virtual void	Input(const _float & fTimeDelta);
	virtual void	Attack();
	virtual void	Skill(SKILL_KEY eKey);
	virtual void	Hit();
	virtual void	Skill_CoolTime(const _float& fTimeDelta);

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CGameObject*			m_pOwner = nullptr;
	CTransform*				m_pOwnerTransform = nullptr;
	CRigidBody*				m_pOwnerRigidBody = nullptr;

	/* �÷��̾� Tick ������ */
	_bool					m_bStop = { false };

	/* �÷��̾� ������ ����*/
	Vec3					m_vPrePos;
	Vec3					m_vNextMove;
	_bool					m_bMoveStop = { false };
	_float					m_fLerpLook_Speed = { 20.f };


	/* ��ų */
	class CPlayer_Skill*	m_pSkills[SKILL_KEY::_END] = { nullptr };
	SKILL_KEY				m_eSelectedSkill = { SKILL_KEY::_END };
	/* �� Ÿ�� */
	_float					m_fCoolDownAcc[SKILL_KEY::_END] = { 0.f };
	_float					m_fCoolTime[SKILL_KEY::_END] = { -1.f };


	/* SG���� ���� */
	Vec3					m_vMaxLinearSpeed;
	Vec3					m_vLinearSpeed;
	Vec3					m_vMaxAngularSpeed;
	Vec3					m_vAngularSpeed;

public:
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	PURE;
	virtual void Free() override;
};

END