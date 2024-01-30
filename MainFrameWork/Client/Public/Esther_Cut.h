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
class CPartObject;
END

BEGIN(Client)

class CPlayer;
class CEsther;

class CEsther_Cut abstract : public CGameObject
{
public:
	enum class ESTHERTYPE { SA, WY, BT, _END };
	enum class MODEL_PART { FACE, FACE_DEFAULT, FACE_S_ANGRY, FACE_ANGRY, BODY, _END };

protected:
	CEsther_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Cut(const CEsther_Cut& rhs);
	virtual ~CEsther_Cut() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(CPlayer* pLeader, void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	void					Set_OwnerEsther(CEsther* pEsther) { m_pOwnerEsther = pEsther; }
	CEsther*				Get_OwnerEshter() { return m_pOwnerEsther; }

	void					Set_LeaderPlayer(CPlayer* pPlayer) { m_pLeaderPlayer = pPlayer; }

	

	CShader*				Get_ShaderCom() { return m_pShaderCom; }

	void					Set_AnimationSpeed(_float fSpeed) { m_fAnimationSpeed = fSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

public:
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist = 1.5f, _bool bRootRot = false, _bool bReverse = false, _bool bUseY = false);

	const wstring&			Get_VoiceSoundKey() { return m_VoiceSoundKey; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound) { m_VoiceSoundKey = VoiceSound; }
	void					Set_VoiceSoundKey(const wstring& VoiceSound, _float fDelay) { m_VoiceSoundKey = VoiceSound; m_fVoiceSoundDelay = fDelay; }
	_bool					Stop_VoiceSound();

	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

	_bool					Is_Finished() { return m_IsFinished; }

	virtual void			Reset() PURE;
	virtual void			Ready() PURE;
	virtual void			Check_Finish() PURE;

protected:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_Parts();

protected:
	_bool							m_IsFinished = { false };

	_bool							m_bDebugRender = { false };

	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;

	std::future<HRESULT>			m_PlayAnimation;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

protected:
	/* �÷��̾� ����*/
	CPlayer* m_pLeaderPlayer = { nullptr };

	/* Esther ���� */
	CEsther*						m_pOwnerEsther = { nullptr };
	_uint							m_iAnimIndex = { 0 };

	/* ���� */
	CModel* m_pModelPartCom[(_uint)MODEL_PART::_END] = { nullptr };
	CPartObject* m_pPart = { nullptr };

	/* �ø� ����ü */
	wstring			m_VoiceSoundKey;
	_float			m_fVoiceSoundDelay;

public:
	virtual void Free();

};

END

