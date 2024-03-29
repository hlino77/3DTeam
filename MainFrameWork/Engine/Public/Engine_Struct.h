#ifndef Engine_Struct_h__
#define Engine_Struct_h__
#include "Engine_Enum.h"
#include <stack>

namespace Engine
{
	typedef struct TLSDesc
	{
		uint32 LThreadId = 0;
		std::stack<int32>	LLockStack;
		SendBufferChunkRef LSendBufferChunk;
	}TLSDESC;

	typedef struct tagKeyFrame
	{
		float		fTime;

		SimpleMath::Vector3	vScale;
		SimpleMath::Vector4	vRotation;
		SimpleMath::Vector3	vPosition;

	}KEYFRAME;

	/* 빛의 정보를 담아놓기 위한 구조체. */
	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_POINT, TYPE_DIRECTIONAL, TYPE_END };

		TYPE			eType;

		SimpleMath::Vector4		vDirection;

		SimpleMath::Vector4		vPosition;
		float			fRange;

		SimpleMath::Vector4		vDiffuse;
		SimpleMath::Vector4		vAmbient;
		SimpleMath::Vector4		vSpecular;

	}LIGHTDESC;



	typedef struct tagMaterialDesc
	{
		char			strName[MAX_PATH];

		SimpleMath::Vector4		vAmbient;
		SimpleMath::Vector4		vDiffuse;
		SimpleMath::Vector4		vSpecular;
		SimpleMath::Vector4		vEmissive;
		class CTexture* pTexture[AI_TEXTURE_TYPE_MAX];
	}MATERIALDESC;

	typedef struct tagLineIndices16
	{
		unsigned short		_0, _1;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_0, _1;
	}LINEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	///////////////
	/// VtxType ///
	///////////////
	// D3DDECLUSAGE
	typedef struct ENGINE_DLL tagVertexTexture
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector2		vTexture;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertexTrail
	{
		SimpleMath::Vector3		vPosition = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		SimpleMath::Vector2		vTexture = SimpleMath::Vector2(0.0f, 0.0f);
		_float					fAlpha = 0.0f;

		static const unsigned int iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXTRAIL;

	typedef struct ENGINE_DLL tagVertexNormalTexture
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vNormal;
		SimpleMath::Vector2		vTexture;

		static const unsigned int iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexModel
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vNormal;
		SimpleMath::Vector2		vTexture;
		SimpleMath::Vector3		vTangent;

		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vNormal;
		SimpleMath::Vector2		vTexture;
		SimpleMath::Vector3		vTangent;
		XMUINT4					vBlendIndex; /* 이 정점에 영향을 주는 뼈의 인덱스 네개. */
		SimpleMath::Vector4		vBlendWeight; /* 영향르 주고 있는 각 뼈대의 영향 비율 */

		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexParticle
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vVelocity;
		XMFLOAT2	vSize;
		_float		fAge;
		_int		iType;

		static const unsigned int				iNumElements = 5;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPARTICLE;

	typedef struct ENGINE_DLL tagVertexCubeTexture
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vTexture;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXPOINT;

	typedef struct ENGINE_DLL tagVertex_Point_Instance	// Instancing
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
		// Instance
		XMFLOAT4X4		matWorld;

		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXINSTANCE_POINT;

	typedef struct ENGINE_DLL tagVertexColor
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector4		vColor;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXCOL;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	}GRAPHIC_DESC;

	typedef struct tagMyHierarchyNode
	{
		char cName[MAX_PATH];
		char cParent[MAX_PATH];
		int  iDepth;
		SimpleMath::Matrix matTransform;
	}DATA_NODE;

	typedef struct tagMyMaterialData
	{
		char cNames[AI_TEXTURE_TYPE_MAX][MAX_PATH];
	}DATA_MATERIAL;

	typedef struct tagMyBoneData
	{
		char		cNames[MAX_PATH];
		SimpleMath::Matrix		matOffset;
	}DATA_BONE;

	typedef struct tagMyMeshData
	{
		char				cName[MAX_PATH];
		int					iMaterialIndex;

		int					NumVertices;
		// VTXMODEL* pNonAnimVertices;
		// VTXANIMMODEL* pAnimVertices;

		int					iNumPrimitives;
		// FACEINDICES32* pIndices;

		int					iNumBones;
		DATA_BONE* pBones;

	} DATA_MESH;

	typedef struct tagMyChannelData
	{
		char szName[MAX_PATH];
		int iNumKeyFrames;
		KEYFRAME* pKeyFrames;
	} DATA_CHANNEL;

	typedef struct tagAnimData
	{
		char szName[MAX_PATH];

		bool bLoop;
		int	iNumChannels;

		float fDuration;
		float fTickPerSecond;

		DATA_CHANNEL* pChannel;
	} DATA_ANIM;

	typedef struct tagSceneData
	{

		int iNodeCount;
		DATA_NODE* pNodesData;

		int iMaterialCount;
		DATA_MATERIAL* pMaterialData;

		int iMeshCount;
		DATA_MESH* pMeshData;

		int iNumAnimations;
		DATA_ANIM* pAnimData;

	} DATA_SCENE;

	typedef struct ENGINE_DLL tagTriangleDesc
	{
		Vec3 vPos0, vPos1, vPos2;
		Vec3 vNormal;
		_float fDist;
	}TRIAGLEDESC;

	typedef struct ENGINE_DLL tagVertex_Instance
	{
		Matrix			matWorld;

	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_PointEffect_Instance
	{
		XMFLOAT4		vTranslation;
		XMFLOAT4		vDirection;
		XMFLOAT4		vScale;
		XMFLOAT4		vColor;
		XMFLOAT4		vBlurColor;

	}VTXINSTANCE_POINTEFFECT;

	typedef struct ENGINE_DLL tagVertex_Model_Instance
	{
		XMFLOAT3        vPosition;
		XMFLOAT3        vNormal;
		XMFLOAT2        vTexcoord;
		XMFLOAT3        vTangent;

		Matrix			matWorld;

		static const unsigned int                iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC    Elements[iNumElements];
	}VTXINSTANCE_MODEL;



	typedef struct ENGINE_DLL tagVertexAnimModel_Instance
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vNormal;
		SimpleMath::Vector2		vTexture;
		SimpleMath::Vector3		vTangent;
		XMUINT4					vBlendIndex; /* 이 정점에 영향을 주는 뼈의 인덱스 네개. */
		SimpleMath::Vector4		vBlendWeight; /* 영향르 주고 있는 각 뼈대의 영향 비율 */

		_uint					iID;
		Matrix					matWorld;

		static const unsigned int iNumElements = 11;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXINSTANCE_ANIMMODEL;

	typedef struct ENGINE_DLL tagVertex_Rect_Instance
	{
		XMFLOAT4        vTranslation;
		XMFLOAT4        vScale;

		static const unsigned int                iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC    Elements[iNumElements];
	}VTXINSTANCE_RECT;

	typedef struct ENGINE_DLL tagVertex_LineCircle_Instance
	{
		static const unsigned int				iNumElements = 5;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_LINECIRCLE_INSTANCE;


	typedef struct ENGINE_DLL tagVertex_Trail_Instance
	{
		static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_TRAIL_INSTANCE;

	typedef struct ENGINE_DLL tagVertex_ModelEffect_Declaration
	{
		static const unsigned int				iNumElements = 10;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_MODELEFFECT_DECLARATION;

	typedef struct ENGINE_DLL tagProjectileDesc
	{
		class CGameObject* pAttackOwner;

		_bool	IsMove = { false };
		_float	fMoveSpeed = { 1.f };

		_uint	eUseCollider;
		_uint	eLayer_Collider;

		Vec3	vAttackPos = { 0.f, 0.f, 0.f };
		Matrix  AttackMatrix;

		_float	fAttackTime = { 0.05f };

		_float	fRadius = { 1.f };
		Vec3	vOffset;

		Vec3	vChildScale;
		Vec3	vChildOffset;

		/* 투사체 정보 */
		_uint	iDamage = { 0 };
		_uint	iStagger = { 0 };
		_uint	iStatusEffect = { 0 };
		_float	fStatusDuration = { 0.f };
		_float	fRepulsion = { 0.f };
		_bool	bUseProjPos = { false };
		_bool	bUseFactor = { true };
		_bool	IsCounter = { false };

		/* 스폰투사체 정보 */
		_bool	IsSpawner = { false };
		_bool	IsColliSpawn = { false };
		_bool	bRansdomSpawn = { false };
		Vec4	vRandSpawnFigure;

		_uint	eUse_SpawnCollider;
		_uint	eLayer_SpawnCollider;

		_int	iSpawnAmount = { -1 };
		_float	fSpawnTime = { 0.0f };
		_float  fSpawnAttackTime = { 0.0f };

		_float	fSpawnRadius = { 1.f };
		Vec3	vSpawnOffset;


	}PROJECTILE_DESC;

	typedef struct ENGINE_DLL MyLerpFloatInfo
	{
		// Time
		_float		fStartTime = 0.f;
		_float		fEndTime = 0.f;
		_float		fCurTime = 0.f;

		// Value
		_float		fStartValue = 0.f;
		_float		fTargetValue = 0.f;
		_float		fCurValue = 0.f;

		_bool		bActive = false;

		LERP_MODE	eMode = LERP_MODE::DEFAULT; 

		void Init_Lerp(const _float& _fTime, const _float _fStartValue, const _float& _fTargetValue, const LERP_MODE& _eMode = LERP_MODE::DEFAULT)
		{
			bActive = true;
			fCurTime = 0.f;
			fCurValue = 0.f;
			eMode = _eMode;

			Set_Lerp(_fTime, _fStartValue, _fTargetValue);
		}

		void Set_Lerp(const _float& _fTime, const _float _fStartValue, const _float& _fTargetValue)
		{
			fEndTime = _fTime;
			fStartValue = fCurValue = _fStartValue;
			fTargetValue = _fTargetValue;
		}

		// Mode
		_float Update_Lerp(const _float& fTimeDelta)
		{
			if (false == bActive)
			{
				return fTargetValue;
			}

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				bActive = false;
				fCurTime = fEndTime;
				fCurValue = fTargetValue;

				return fCurValue;
			}

			_float t = fCurTime / fEndTime;

			switch (eMode)
			{
			case LERP_MODE::DEFAULT:
			{
			}
			break;
			case LERP_MODE::EASE_OUT:
			{
				t = sinf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EASE_IN:
			{
				t = 1.f - cosf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EXPONENTIAL:
			{
				t = t * t;
			}
			break;
			case LERP_MODE::SMOOTHSTEP:
			{
				t = t * t * (3.f - 2.f * t);

			}
			break;
			case LERP_MODE::SMOOTHERSTEP:
			{
				t = t * t * t * (t * (6.f * t - 15.f) + 10.f);
			}
			break;
			}

			return fCurValue = Lerp_Float(fStartValue, fTargetValue, t);
		}

		_float Lerp_Float(const _float& _f1, const _float& _f2, const _float _fTime) { return (1 - _fTime) * _f1 + (_fTime * _f2); }

	}LERP_FLOAT;

	typedef struct ENGINE_DLL MyLerpVec3Info
	{
		_float		fStartTime = 0.f;
		_float		fEndTime = 0.f;
		_float		fCurTime = 0.f;

		Vec3		vStartVec;
		Vec3		vTargetVec;
		Vec3		vCurVec;

		_bool		bActive = false;

		LERP_MODE	eMode = LERP_MODE::DEFAULT;

		void Init_Lerp(const _float& _fTime, const Vec3 _vStartValue, const Vec3 _vTargetValue, const LERP_MODE& _eMode = LERP_MODE::DEFAULT)
		{
			bActive = true;
			fCurTime = 0.f;
			vCurVec = Vec3::Zero;
			eMode = _eMode;

			Set_Lerp(_fTime, _vStartValue, _vTargetValue);
		}

		void Set_Lerp(const _float& _fTime, const Vec3 _vStartValue, const Vec3 _vTargetValue)
		{
			fEndTime = _fTime;
			vStartVec = vCurVec = _vStartValue;
			vTargetVec = _vTargetValue;
		}

		Vec3 Update_Lerp(const _float& fTimeDelta)
		{
			if (false == bActive)
			{
				return vTargetVec;
			}

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				bActive = false;
				fCurTime = fEndTime;
				vCurVec = vTargetVec;

				return vCurVec;
			}

			_float t = fCurTime / fEndTime;

			switch (eMode)
			{
			case LERP_MODE::DEFAULT:
			{
			}
			break;
			case LERP_MODE::EASE_OUT:
			{
				t = sinf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EASE_IN:
			{
				t = 1.f - cosf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EXPONENTIAL:
			{
				t = t * t;
			}
			break;
			case LERP_MODE::SMOOTHSTEP:
			{
				t = t * t * (3.f - 2.f * t);
			}
			break;
			case LERP_MODE::SMOOTHERSTEP:
			{
				t = t * t * t * (t * (6.f * t - 15.f) + 10.f);
			}
			break;
			}

			return vCurVec = XMVectorLerp(vStartVec, vTargetVec, t);

		}
	}LERP_VEC3;
}

#endif // Engine_Struct_h__
