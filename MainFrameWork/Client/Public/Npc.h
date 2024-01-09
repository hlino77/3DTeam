#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "PartObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CUI_SpeechBubble;

class CNpc : public CGameObject
{
public:
	enum class NPCTYPE { FUNCTION, DECO, _END };
	enum class NPCSHAPE { MA01, MA02, MA03, MA04, MA05, FE01, FE02, FE03, FE04, FE05, SOL, SP, _END };
	enum class PART { FACE, BODY, _END };
	enum class WEAPON_PART { RIGHT, LEFT, _END };
	enum class WEAPON_OFFSET { SCALE, ROTATION, POSITION, _END };

public:
	typedef struct Npc_Desc
	{
		_uint			iCurLevel;

		_uint			iNpcType;
		wstring			strNpcTag = { TEXT("") };
		wstring			strNpcName = { TEXT("") };
		Matrix			matStart;

		_uint			iNpcShape;

		wstring			strNpcMq;
		wstring			strNpcHead;
		wstring			strNpcBody;

		wstring			strIdleAnim = { TEXT("") };
		wstring			strActAnim = { TEXT("") };
		_float			fChangeAnimTime = { 0.f };

		_bool			IsMove = { false };
		_float			fMoveLength = { 0.f };

		_bool			IsTalk = { false };
		_float			fTalkStartTime = { 0.f };
		vector<wstring> vecTalks;

		_bool			bUseWeaponPart = { false };
		wstring			strLeftPart = { TEXT("") };
		Vec3			vLeftOffset[(_uint)WEAPON_OFFSET::_END];
		wstring			strRightPart = { TEXT("") };
		Vec3			vRightOffset[(_uint)WEAPON_OFFSET::_END];

	}NPCDESC;

protected:
	CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc(const CNpc& rhs);
	virtual ~CNpc() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();
	virtual HRESULT			Render_Debug();

public:
	CShader* Get_ShaderCom() { return m_pShaderCom; }

	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }

	void					Set_AnimationSpeed(_float fSpeed) { m_fAnimationSpeed = fSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

public:
	void					Set_State(const wstring& szName);
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bRootRot = false, _bool bReverse = false, _bool bUseY = false);
	const	wstring& Get_State() { return m_pStateMachine->Get_CurState(); }
	const	wstring& Get_PreState() { return m_pStateMachine->Get_PreState(); }

	void					Go_Straight(_float fSpeed, _float fTimeDelta);
	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void					Set_Picking(_bool bPicking) { m_bPicking = bPicking; }

	void					Body_Collision(CGameObject* pObject);

	const wstring& Get_NpcName() { return m_strNpcName; }
	void					Set_NpckName(const wstring& szNpcName) { m_strNpcName = szNpcName; }


	const wstring& Get_VoiceSoundKey() { return m_VoiceSoundKey; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound) { m_VoiceSoundKey = VoiceSound; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound, _float fDelay) { m_VoiceSoundKey = VoiceSound; m_fVoiceSoundDelay = fDelay; }
	_bool					Stop_VoiceSound();

	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

public:
	void					Set_Several_Weapon_RenderState(WEAPON_PART ePart, _bool Is_Render);

	virtual HRESULT			Ready_Coliders() { return S_OK; }

	void					Show_SpeechBuble(const wstring& szChat);

protected:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_Parts();
	HRESULT					Ready_SpeechBuble();

	void					CullingObject();

protected:
	void					Check_ChangeAnim(const _float& fTimeDelta);


protected:
	_float							m_fMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

protected:
	/* NPC 정보 */
	NPCTYPE			m_eNpcType;
	wstring			m_strNpcName;
	Matrix			m_matStart;
	Vec3			m_vStartPos;

	NPCSHAPE		m_eNpcShape;
	wstring			m_strNpcMq;
	wstring			m_strNpcHead;
	wstring			m_strNpcBody;

	wstring			m_strIdleAnim = { TEXT("") };
	wstring			m_strActAnim = { TEXT("") };
	_float			m_fChangeAnimTime = { 0.f };
	_float			m_fChangeAnimAcc = { 0.f };
	_int			m_iIdleAnimIndex = { -1 };
	_int			m_iActAnimIndex = { -1 };

	_bool			m_IsMove = { false };
	_bool			m_IsReach = { false };
	_float			m_fMoveLength = { 0.f };
	Vec3			m_vMovePos;

	_bool			m_IsTalk = { false };
	_float			m_fTalkStartTime = { 0.f };
	_float			m_fTalkTimeAcc = { 0.f };
	vector<wstring> m_vecTalks;

	/* 파츠 */
	CModel*			m_pNpcPartCom[(_uint)PART::_END] = { nullptr };
	CPartObject*	m_pWeaponPart[(_uint)WEAPON_PART::_END] = { nullptr };

	_bool			m_bUseWeaponPart = { false };
	wstring			m_strLeftPart = { TEXT("") };
	Vec3			m_vLeftOffset[(_uint)WEAPON_OFFSET::_END];
	wstring			m_strRightPart = { TEXT("") };
	Vec3			m_vRightOffset[(_uint)WEAPON_OFFSET::_END];

	_int			m_iLeftBoneIndex = { -1 };
	_int			m_iRightBoneIndex = { -1 };

	/* 컬링 절두체 */
	BoundingSphere	m_tCullingSphere;

	wstring			m_VoiceSoundKey;
	_float			m_fVoiceSoundDelay;

protected:
	/* 머리색 설정 */
	Vec4	m_vHairColor_1 = { 0.f, 0.f, 0.f, 0.f };
	Vec4	m_vHairColor_2 = { 0.f, 0.f, 0.f, 0.f };
	_int	m_IsHair = { -1 };

	CUI_SpeechBubble* m_pSpeechBuble = nullptr;

public:
	virtual void Free();

};

END

