#pragma once
#include "ToolBase.h"
#include "Hasher.h"
#include "Client_Defines.h"
#include "Npc.h"
#include "Npc_Part.h"

BEGIN(Engine)
class CUtils;
class CModel;
class CTransform;
class CPartObject;
class CGameObject;
END

BEGIN(Client)

class CMannequin;
class CLevel_Tool_Npc;
class CNpc;
class CNpc_Part;

class CNpcTool : public CToolBase
{
	using Super = CToolBase;

private:

private:
	CNpcTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNpcTool() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr)	override;
	virtual HRESULT Tick(_float fTimeDelta)		override;
	virtual HRESULT LateTick(_float fTimeDelta)	override;
	virtual HRESULT	DebugRender()						override;

private:
	void	Input();
	_bool	Get_CellPos(Vec3& vPos);
	void	Pick_Npc();
	void	Set_DebugRender_Npc();

private:
	HRESULT	InfoView(_float fTimeDelta);
	/* NPC �������� */
	void	Npc_List();
	HRESULT	Load_Npc();
	HRESULT	Start_Load_Npc(const wstring& strPath);
	void	Delete_Npc();

	HRESULT ModelView(_float fTimeDelta);
	/* ����Ʈ */
	void	Select_Npc(_float fTimeDelta);
	void	Start_Pos(_float fTimeDelta);
	void	Name(_float fTimeDelta);
	void	Type(_float fTimeDelta);
	void	Shape(_float fTimeDelta);
	/* ���� */
	void	Edit_Npc(_float fTimeDelta);
	void	Transform_Matrix(_float fTimeDelta);
	void	Move(_float fTimeDelta);
	void	Talk(_float fTimeDelta);
	void	Animaition(_float fTimeDelta);
	void	Weapon(_float fTimeDelta);
	void	LeftWeapon(_float fTimeDelta);
	void	RightWeapon(_float fTimeDelta);
	void	HeadBody(_float fTimeDelta);
	void	Head(_float fTimeDelta);
	void	Body(_float fTimeDelta);

	/* ���� �� ���� */
	void	Create_Npc(_float fTimeDelta);
	void	Create_None_Npc();
	HRESULT	Save_Npc(_float fTimeDelta);

	void	Clear_Info();
private:
	HRESULT	Ready_Sort_Part();

private:
	CUtils* m_pUtils = nullptr;
	CMannequin* m_pMannequin = nullptr;
	CTransform* m_pTransformCom = { nullptr };
	CTransform* m_pTransformLeftPartCom = { nullptr };
	CTransform* m_pTransformRIghtPartCom = { nullptr };

	CGameObject* m_pCurrentPlayer = nullptr;
	CPartObject* m_pCurrentWeapon = nullptr;

private:
	CNpc::NPCDESC	m_NpcCreateDesc;
	vector<CNpc::NPCDESC> m_vecNpcDesc;

private:
	vector<CGameObject*>	m_vecNpcs;
	_int					m_iCurNpc;
	_bool					m_bDebugRender = { false };


private: /* ����Ʈ NPC ���� */
	Vec3	m_vStartPos;
	_int	m_iSelectNpType = -1;
	_bool	m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::_END] = { false };
	_int	m_iLastCheck = -1;
	CNpc::NPCSHAPE m_eNpcShape;
	_char	m_szNpcTag[MAX_PATH];
	_char	m_szNpcName[MAX_PATH];
	wstring	m_strGroup;

	_int	m_iSelectSol = -1;
	_int	m_iSelectSP = -1;

	_bool	m_bSelected = { false };

private: /* NPC Ʈ������ ���� */
	Vec3	m_vNpcScale;
	Vec3	m_vNpcRot;
	Vec3	m_vNpcPos;
	_bool	m_bTransformChange = { false };

private: /* NPC �̵� ����*/
	_bool	m_IsMove = { false };
	_bool	m_IsMovePatrol = { false };
	Vec3	m_vMovePos;
	vector<Vec3> m_vecMovePos;
	_int	m_iCurMove;
	vector<string> m_vecforMoveList;

private: /* NPC ���� ���� */
	unordered_map<wstring, CComponent*> m_mapHead;
	unordered_map<wstring, CComponent*> m_mapBody;
	_char	m_szHeadPartName[MAX_PATH];
	_char	m_szBodyPartName[MAX_PATH];
	_int	m_iCurHeadIndex = { 0 };
	_int	m_iCurBodyIndex = { 0 };
	_int	m_iSelectModelPart = { (_uint)CNpc::PART::_END };


private: /* NPC �������� ����*/
	unordered_map<wstring, CComponent*> m_mapWp;
	_int	m_iCurWpLeftIndex = { -1 };
	_int	m_iCurWpRightIndex = { -1 };
	_char	m_szLeftWpName[MAX_PATH];
	_char	m_szRightWpName[MAX_PATH];
	Matrix	m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::_END];
	_bool	m_bSetWpLeftPart = { false };
	_bool	m_bSetWpRightPart = { false };

	_int	m_iSelectPart = { (_uint)CNpc::WEAPON_PART::_END };
	_bool	m_Check_NpcPart[(_uint)CNpc::WEAPON_PART::_END] = { false };
	_int	m_iWPLeftLastCheck = -1;
	_int	m_iWPRightLastCheck = -1;

	Vec3	m_vWpLeftScale = { 1.f, 1.f, 1.f };
	Vec3	m_vWpLeftRot;
	Vec3	m_vWpLeftPos;

	Vec3	m_vWpRightScale = { 1.f, 1.f, 1.f };
	Vec3	m_vWpRightRot;
	Vec3	m_vWpRightPos;

private: /* NPC �ִϸ��̼� ����*/
	_char	m_szAnimationName[MAX_PATH];
	_char	m_szCurrAnimName[MAX_PATH];
	_uint	m_iCurrAnimation = 0;
	_bool	m_bAnimationPlay = true;
	_float	m_fDuration = { 0.f };

private: /* Npc ��ũ���� ���� */
	_bool			m_IsTalk = { false };
	_float			m_fTalkStartTime = { 0.f };
	_int			m_iTalkSequence = { -1 };
	vector<wstring> m_vecTalks;
	vector<string>	m_vecSelectTalk;

	_char			m_szTalk[MAX_PATH];
	_int			m_iCurTalk;

public:
	static class CNpcTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool_Npc* pLevel_Tool);
	virtual void Free() override;
};

END