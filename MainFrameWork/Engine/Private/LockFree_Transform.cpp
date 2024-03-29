#include "LockFree_Transform.h"


CLockFree_Transform::CLockFree_Transform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CTransform(pDevice, pContext)
{

}

CLockFree_Transform::CLockFree_Transform(const CLockFree_Transform & rhs)
	: CTransform(rhs)
{

}

Vec3 CLockFree_Transform::Get_State(const STATE& eState)
{
	return Vec3(m_WorldMatrix.m[eState][0], m_WorldMatrix.m[eState][1], m_WorldMatrix.m[eState][2]);
}

Matrix& CLockFree_Transform::Get_WorldMatrix()
{
	return m_WorldMatrix;
}

Matrix CLockFree_Transform::Get_WorldMatrix_TP()
{
	return m_WorldMatrix.Transpose();
}

Matrix CLockFree_Transform::Get_WorldMatrixInverse()
{
	return m_WorldMatrix.Invert();
}

void CLockFree_Transform::Set_State(const STATE& eState, const Vec3& vState)
{
	m_WorldMatrix.m[eState][0] = vState.x;
	m_WorldMatrix.m[eState][1] = vState.y;
	m_WorldMatrix.m[eState][2] = vState.z;
}

void CLockFree_Transform::Set_WorldMatrix(const Matrix& matWorld)
{
	m_WorldMatrix = matWorld;
}

HRESULT CLockFree_Transform::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CLockFree_Transform::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CLockFree_Transform::Go_Straight(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CLockFree_Transform::STATE_LOOK);
	vLook.Normalize();

	vPosition += vLook * fSpeed * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::Go_Backward(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CLockFree_Transform::STATE_LOOK);
	vLook.Normalize();

	vPosition -= vLook * fSpeed * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::Go_Left(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CLockFree_Transform::STATE_RIGHT);
	vLook.Normalize();

	vPosition -= vLook * fSpeed * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::Go_Right(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CLockFree_Transform::STATE_RIGHT);
	vLook.Normalize();

	vPosition += vLook * fSpeed * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::Go_Up(_float fTimeDelta)
{
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	Vec3 vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	vUp.Normalize();

	vPosition += vUp * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::Go_Down(_float fTimeDelta)
{
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	Vec3 vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	vUp.Normalize();

	vPosition -= vUp * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::Set_Scale(Vec3 vScaleInfo)
{
	Vec3 vRight = Get_State(CLockFree_Transform::STATE_RIGHT);
	vRight.Normalize();

	Vec3 vUp = Get_State(CLockFree_Transform::STATE_UP);
	vUp.Normalize();

	Vec3 vLook = Get_State(CLockFree_Transform::STATE_LOOK);
	vLook.Normalize();


	Set_State(CLockFree_Transform::STATE_RIGHT, vRight * vScaleInfo.x);
	Set_State(CLockFree_Transform::STATE_UP, vUp * vScaleInfo.y);
	Set_State(CLockFree_Transform::STATE_LOOK, vLook * vScaleInfo.z);
}

Vec3 CLockFree_Transform::Get_Scale()
{
	return Vec3(Get_State(CLockFree_Transform::STATE_RIGHT).Length(), Get_State(CLockFree_Transform::STATE_UP).Length(), Get_State(CLockFree_Transform::STATE_LOOK).Length());
}

void CLockFree_Transform::Turn(Vec3 vAxis, _float fTimeDelta)
{
	Quaternion vRot = Quaternion::CreateFromAxisAngle(vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);
	Matrix matRot = Matrix::CreateFromQuaternion(vRot);
	
	Matrix matWorld = m_WorldMatrix * matRot;


	Set_State(CLockFree_Transform::STATE_RIGHT, Vec3(matWorld.m[0]));
	Set_State(CLockFree_Transform::STATE_UP, Vec3(matWorld.m[1]));
	Set_State(CLockFree_Transform::STATE_LOOK, Vec3(matWorld.m[2]));
}

void CLockFree_Transform::Turn_Axis(Vec3 vAxis, _float fRadian)
{
	Matrix		RotationMatrix = Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(vAxis, fRadian));

	Vec3 vPos = m_WorldMatrix.Translation();

	m_WorldMatrix *= RotationMatrix;
	m_WorldMatrix.Translation(vPos);
}

void CLockFree_Transform::Rotation(Vec3 vAxis, _float fRadian)
{
	Matrix        RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	RotationMatrix.CreateFromQuaternion(Quaternion::CreateFromAxisAngle(vAxis, fRadian));
	Vec3 vScale = Get_Scale();

	Vec3 vRight(RotationMatrix.m[0]);
	Vec3 vUp(RotationMatrix.m[1]);
	Vec3 vLook(RotationMatrix.m[2]);

	vRight.Normalize();
	vUp.Normalize();
	vLook.Normalize();

	Set_State(CLockFree_Transform::STATE_RIGHT, vRight * vScale.x);
	Set_State(CLockFree_Transform::STATE_UP, vUp * vScale.y);
	Set_State(CLockFree_Transform::STATE_LOOK, vLook * vScale.z);
}

void CLockFree_Transform::Turn_Rotation_CurrentState(Vec3 vAxis, _float fTimeDelta)
{
	Matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);


	Matrix RotationMatrix = XMMatrixRotationAxis(vAxis, fTimeDelta);


	WorldMatrix = XMMatrixMultiply(RotationMatrix, WorldMatrix);

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

void CLockFree_Transform::My_Rotation(Vec3 vEulers)
{
	Matrix matRotation = Matrix::Identity;
	Quaternion quat = Quaternion::Identity;

	vEulers.x = XMConvertToRadians(vEulers.x);
	vEulers.y = XMConvertToRadians(vEulers.y);
	vEulers.z = XMConvertToRadians(vEulers.z);

	quat = Quaternion::CreateFromYawPitchRoll(vEulers.y, vEulers.x, vEulers.z);

	matRotation = Matrix::CreateFromQuaternion(quat);

	
	for (_uint i = 0; i < 3; ++i)
	{
		Vec3 v(m_WorldMatrix.m[i]);
		v = Vec3::TransformNormal(v, matRotation);

		for (_uint j = 0; j < 3; ++j)
			m_WorldMatrix.m[i][j] = *((_float*)&v + j);
	}
}

Matrix CLockFree_Transform::Q_Rotation(Quaternion qRot, Matrix OriginMatrix)
{
	Matrix matRotation = Matrix::CreateFromQuaternion(qRot);

	for (_uint i = 0; i < 3; ++i)
	{
		Vec3 v(OriginMatrix.m[i]);
		v = Vec3::TransformNormal(v, matRotation);

		for (_uint j = 0; j < 3; ++j)
			OriginMatrix.m[i][j] = *((_float*)&v + j);
	}

	return OriginMatrix;
}

void CLockFree_Transform::Set_MovementSpeed(float fSpeed)
{
	m_TransformDesc.fSpeedPerSec = fSpeed;

}

_float CLockFree_Transform::Get_TargetDegree(Vec3 vTarget)
{
	Vec3 vPos = Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = Get_State(CTransform::STATE_LOOK);
	Vec3 vDir = vTarget - vPos;

	if (vLook == Vec3() || vDir == Vec3())
		return 0.0f;

	vLook.y = 0.f;
	vDir.y = 0.f;
	vLook.Normalize();
	vDir.Normalize();

	_float fDegree = XMConvertToDegrees(acosf(vLook.Dot(vDir)));
	Vec3 vCross = vLook.Cross(vDir);

	if (0.f <= vCross.y)
		return fDegree;
	else
		return -fDegree;
}

void CLockFree_Transform::LookAt(Vec3 vAt)
{
	Vec3		vLook = vAt - Get_State(CLockFree_Transform::STATE_POSITION);

	Vec3		vRight = Vec3(0.0f, 1.0f, 0.0f).Cross(vLook);

	Vec3		vUp = vLook.Cross(vRight);

	Vec3		vScale = Get_Scale();


	Set_State(CLockFree_Transform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CLockFree_Transform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CLockFree_Transform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CLockFree_Transform::LookAt_ForLandObject(Vec3 vAt)
{
	Vec3 vLook = vAt - Get_State(CLockFree_Transform::STATE_POSITION);
	vLook.Normalize();

	Vec3 vUp(0.0f, 1.0f, 0.0f);

	Vec3 vScale = Get_Scale();


	Vec3 vRight = vUp.Cross(vLook);
	vRight.Normalize();
	vRight *= vScale.x;

	vLook = vRight.Cross(vUp);
	vLook.Normalize();
	vLook *= vScale.z;

	vUp *= vScale.y;


	Set_State(CLockFree_Transform::STATE_RIGHT, vRight);
	Set_State(CLockFree_Transform::STATE_LOOK, vLook);
	Set_State(CLockFree_Transform::STATE_UP, vUp);
}

void CLockFree_Transform::Move(Vec3 vTargetPos, _float fTimeDelta, _float fLimitDistance)
{
	Vec3		vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	Vec3		vDirection = vTargetPos - vPosition;

	_float		fDistance = vDirection.Length();

	vDirection.Normalize();

	if(fDistance > fLimitDistance)
		vPosition += vDirection * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

CLockFree_Transform * CLockFree_Transform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLockFree_Transform*			pInstance = new CLockFree_Transform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CLockFree_Transform::Clone(CGameObject* pObject, void * pArg)
{
	CLockFree_Transform*			pInstance = new CLockFree_Transform(*this);
	pInstance->m_pOwner = pObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CLockFree_Transform::Free()
{
	__super::Free();
}

void CLockFree_Transform::Move_Dir(Vec3 vDir, _float fTimeDelta)
{
	vDir.Normalize();
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	vPosition += vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::Move_Dir(Vec3 vDir, _float fTimeDelta, _float fSpeed)
{
	vDir.Normalize();
	Vec3 vPosition = Get_State(CLockFree_Transform::STATE_POSITION);
	vPosition += vDir * fSpeed * fTimeDelta;

	Set_State(CLockFree_Transform::STATE_POSITION, vPosition);
}

void CLockFree_Transform::LookAt_Lerp(Vec3 vAt, _float fSpeed, _float fTimeDelta)
{
	Vec3 vPlayerLook = Get_State(CLockFree_Transform::STATE_LOOK);
	vPlayerLook.Normalize();

	Vec3 vTargetLook = vAt;
	vTargetLook.Normalize();

	Vec3 vUp = Get_State(CLockFree_Transform::STATE_UP);
	vUp.Normalize();


	Vec3 vTargetRight = vUp.Cross(vTargetLook);
	vTargetLook = vTargetRight.Cross(vUp);
	vTargetLook.Normalize();

	_float fRadian = acosf(min(1.0f, vTargetLook.Dot(vPlayerLook)));
	if (fRadian <= fSpeed * fTimeDelta)
	{
		vPlayerLook = vTargetLook;

		Vec3 vRight = vUp.Cross(vPlayerLook);
		vRight.Normalize();

		vPlayerLook = vRight.Cross(vUp);
		vPlayerLook.Normalize();

		vUp = vPlayerLook.Cross(vRight);
		vUp.Normalize();

		Vec3 vScale = Get_Scale();

		vRight *= vScale.x;
		vUp *= vScale.y;
		vPlayerLook *= vScale.z;

		Set_State(STATE::STATE_RIGHT, vRight);
		Set_State(STATE::STATE_UP, vUp);
		Set_State(STATE::STATE_LOOK, vPlayerLook);

		return;
	}
	
	Vec3 vPlayerUp = Get_State(CLockFree_Transform::STATE_UP);
	Vec3 vCrossUp = vPlayerLook.Cross(vTargetLook);

	if (vPlayerUp.Dot(vCrossUp) >= 0)
	{
		Turn_Speed(vUp, fSpeed, fTimeDelta);
	}
	else
	{
		Turn_Speed(vUp, -fSpeed, fTimeDelta);
	}
}

void CLockFree_Transform::LookAt_Lerp_ForLand(Vec3 vAt, _float fSpeed, _float fTimeDelta)
{
	Vec3 vPlayerLook = Get_State(CLockFree_Transform::STATE_LOOK);
	vPlayerLook.y = 0.0f;
	vPlayerLook.Normalize();

	Vec3 vTargetLook = vAt;
	vTargetLook.y = 0.0f;
	vTargetLook.Normalize();

	Vec3 vUp = Get_State(CLockFree_Transform::STATE_UP);
	vUp.Normalize();

	Vec3 vSkyUp(0.0f, 1.0f, 0.0f);


	_float fRadian = acosf(min(1.0f, vTargetLook.Dot(vPlayerLook)));
	if (fRadian <= fSpeed * fTimeDelta)
	{
		vPlayerLook = vTargetLook;

		Vec3 vRight = vSkyUp.Cross(vPlayerLook);
		vRight.Normalize();


		vPlayerLook = vRight.Cross(vUp);
		vPlayerLook.Normalize();

		Vec3 vScale = Get_Scale();

		vRight *= vScale.x;
		vUp *= vScale.y;
		vPlayerLook *= vScale.z;

		Set_State(STATE::STATE_RIGHT, vRight);
		Set_State(STATE::STATE_UP, vUp);
		Set_State(STATE::STATE_LOOK, vPlayerLook);

		return;
	}

	Vec3 vPlayerUp = Get_State(CLockFree_Transform::STATE_UP);
	Vec3 vCrossUp = vPlayerLook.Cross(vTargetLook);

	if (vPlayerUp.Dot(vCrossUp) >= 0)
	{
		Turn_Speed(vSkyUp, fSpeed, fTimeDelta);
	}
	else
	{
		Turn_Speed(vSkyUp, -fSpeed, fTimeDelta);
	}
}

void CLockFree_Transform::SetUp_Lerp(Vec3 vLook, _float fSpeed, _float fTimeDelta)
{
	Vec3 vPlayerLook = Get_State(CLockFree_Transform::STATE_LOOK);
	vPlayerLook.Normalize();


	Vec3 vUp = Get_State(CLockFree_Transform::STATE_UP);
	vUp.Normalize();

	vLook.Normalize();


	_float fDot = vUp.Dot(vLook);

	Vec3 vTargetLook;

	if (fDot == 0.0f)
	{
		_float fLookDot = vPlayerLook.Dot(vLook);
		vTargetLook = vPlayerLook * fLookDot;
	}
	else
	{
		_float fLookDot = vPlayerLook.Dot(vLook);
		vTargetLook = vPlayerLook * fLookDot + vUp * fDot;
	}

	vTargetLook.Normalize();


	Vec3 vRight = Get_State(CLockFree_Transform::STATE_RIGHT);
	vRight.Normalize();

	
	_float fRadian = acosf(min(1.0f, vTargetLook.Dot(vPlayerLook)));
	if (fRadian <= fSpeed * fTimeDelta)
	{
		vPlayerLook = vTargetLook;

		Vec3 vUp = vPlayerLook.Cross(vRight);
		vUp.Normalize();

		Vec3 vScale = Get_Scale();

		vRight *= vScale.x;
		vUp *= vScale.y;
		vPlayerLook *= vScale.z;

		Set_State(STATE::STATE_RIGHT, vRight);
		Set_State(STATE::STATE_UP, vUp);
		Set_State(STATE::STATE_LOOK, vPlayerLook);

		return;
	}

	Vec3 vPlayerRight = Get_State(CLockFree_Transform::STATE_RIGHT);
	Vec3 vCrossRight = vPlayerLook.Cross(vTargetLook);

	if (vPlayerRight.Dot(vCrossRight) >= 0)
	{
		Turn_Speed(vRight, fSpeed, fTimeDelta);
	}
	else
	{
		Turn_Speed(vRight, -fSpeed, fTimeDelta);
	}
}

void CLockFree_Transform::Move_Pos(Vec3 vTargetPos)
{
	Vec3 vPos = Vec3(m_WorldMatrix.m[3]);
	
	vPos += vTargetPos;
	Set_State(STATE::STATE_POSITION, vPos);
}

void CLockFree_Transform::Set_Up(Vec3 vNormal)
{
	Vec3 vLook = Get_State(STATE::STATE_LOOK);
	vLook.Normalize();

	Vec3 vUp = vNormal;

	Vec3 vRight = vUp.Cross(vLook);
	vLook = vRight.Cross(vUp);

	vRight.Normalize();
	vUp.Normalize();
	vLook.Normalize();

	Vec3 vScale = Get_Scale();

	vRight *= vScale.x;
	vUp *= vScale.y;
	vLook *= vScale.z;

	Set_State(CLockFree_Transform::STATE_RIGHT, vRight);
	Set_State(CLockFree_Transform::STATE_UP, vUp);
	Set_State(CLockFree_Transform::STATE_LOOK, vLook);

}

void CLockFree_Transform::Move_ToPos(Vec3 vTargetPos, _float fRotSpeed, _float fSpeed, _float fTimeDelta)
{
	LookAt_Lerp_ForLand(vTargetPos, fRotSpeed, fTimeDelta);
	Move_Dir(vTargetPos, fTimeDelta, fSpeed);
}

void CLockFree_Transform::Turn_Speed(Vec3 vAxis, _float fSpeed, _float fTimeDelta)
{
	Quaternion vRot = Quaternion::CreateFromAxisAngle(vAxis, fSpeed * fTimeDelta);
	Matrix matRot = Matrix::CreateFromQuaternion(vRot);

	Matrix matWorld = m_WorldMatrix * matRot;

	Set_State(CLockFree_Transform::STATE_RIGHT, Vec3(matWorld.m[0]));
	Set_State(CLockFree_Transform::STATE_UP, Vec3(matWorld.m[1]));
	Set_State(CLockFree_Transform::STATE_LOOK, Vec3(matWorld.m[2]));
}

void CLockFree_Transform::LookAt_Dir(Vec3 vDir)
{
	Vec3		vUp = Get_State(STATE::STATE_UP);
	Vec3		vLook = vDir;
	Vec3		vRight = vUp.Cross(vLook);


	vLook = vRight.Cross(vUp);

	vUp.Normalize();
	vLook.Normalize();
	vRight.Normalize();


	Vec3		vScale = Get_Scale();

	Set_State(CLockFree_Transform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CLockFree_Transform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CLockFree_Transform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CLockFree_Transform::LookAt_SetUp(Vec3 vAt)
{
	Vec3		vUp = Get_State(STATE::STATE_UP);
	Vec3		vLook = vAt - Get_State(CLockFree_Transform::STATE_POSITION);

	Vec3		vRight = vUp.Cross(vLook);

	vUp = vLook.Cross(vRight);

	Vec3		vScale = Get_Scale();


	Set_State(CLockFree_Transform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CLockFree_Transform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CLockFree_Transform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}
