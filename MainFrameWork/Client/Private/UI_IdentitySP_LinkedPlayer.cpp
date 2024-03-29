#include "stdafx.h"
#include "UI_IdentitySP_LinkedPlayer.h"
#include "Player.h"
#include "GameInstance.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"

CUI_IdentitySP_LinkedPlayer::CUI_IdentitySP_LinkedPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_IdentitySP_LinkedPlayer::CUI_IdentitySP_LinkedPlayer(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_IdentitySP_LinkedPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if (nullptr != pArg)
	{
		m_pPlayer = static_cast<CPlayer_Doaga*>(pArg);
		if ((nullptr == m_pPlayer)||(TEXT("SP") != m_pPlayer->Get_ObjectTag()))
			return E_FAIL;
	}

	m_strUITag = TEXT("UI_IdentitySP_LinkedPlayer");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = 850.f;

	//m_pTransform_BubbleGaugeL
	m_pTransform_BubbleGaugeL->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_BubbleGaugeL->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 42.f) - (g_iWinSizeX * 0.5f), -(m_fY + 22.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_BubbleGaugeC
	m_pTransform_BubbleGaugeC->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_BubbleGaugeC->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -(m_fY + 28.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_BubbleGaugeR
	m_pTransform_BubbleGaugeR->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_BubbleGaugeR->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 42.f) - (g_iWinSizeX * 0.5f), -(m_fY + 22.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_SkillL
	m_pTransform_SkillL->Set_Scale(Vec3(44.f * 0.75f, 44.f * 0.75f, 1.f));
	m_pTransform_SkillL->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 50.f) - (g_iWinSizeX * 0.5f), -(m_fY - 30.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_SkillR
	m_pTransform_SkillR->Set_Scale(Vec3(44.f * 0.75f, 44.f * 0.75f, 1.f));
	m_pTransform_SkillR->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 50.f) - (g_iWinSizeX * 0.5f), -(m_fY - 30.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_SkillFrameL
	m_pTransform_SkillFrameL->Set_Scale(Vec3(56.f, 56.f, 1.f));
	m_pTransform_SkillFrameL->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 50.f) - (g_iWinSizeX * 0.5f), -(m_fY - 28.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_SkillFrameR
	m_pTransform_SkillFrameR->Set_Scale(Vec3(56.f, 56.f, 1.f));
	m_pTransform_SkillFrameR->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 50.f) - (g_iWinSizeX * 0.5f), -(m_fY - 28.f) + g_iWinSizeY * 0.5f, 0.2f));
	for (size_t i = 0; i < 3; i++)
	{
		m_fSizeX_Masked[i] = 80.f;
		m_fSizeY_Masked[i] = 80.f;
	}
	//m_pTransform_Masked_EffectL
	m_pTransform_Masked_EffectL->Set_Scale(Vec3(m_fSizeX_Masked[0], m_fSizeY_Masked[0], 1.f));
	m_pTransform_Masked_EffectL->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 44.f) - (g_iWinSizeX * 0.5f), -(m_fY + 22.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_Masked_EffectC
	m_pTransform_Masked_EffectC->Set_Scale(Vec3(m_fSizeX_Masked[1], m_fSizeY_Masked[1], 1.f));
	m_pTransform_Masked_EffectC->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX) - (g_iWinSizeX * 0.5f), -(m_fY + 28.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_Masked_EffectR
	m_pTransform_Masked_EffectR->Set_Scale(Vec3(m_fSizeX_Masked[2], m_fSizeY_Masked[2], 1.f));
	m_pTransform_Masked_EffectR->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 44.f) - (g_iWinSizeX * 0.5f), -(m_fY + 22.f) + g_iWinSizeY * 0.5f, 0.2f));
	
	for (size_t i = 0; i < 3; i++)
	{
		m_fSizeX_MaxEffect[i] = { 167.f * 0.5f };
		m_fSizeY_MaxEffect[i] = { 160.f * 0.5f };
	}
	//m_pTransform_BubbleMaxEffectL
	m_pTransform_BubbleMaxEffectL->Set_Scale(Vec3(m_fSizeX_Masked[2], m_fSizeY_Masked[2], 1.f));
	m_pTransform_BubbleMaxEffectL->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 44.f) - (g_iWinSizeX * 0.5f), -(m_fY + 22.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_BubbleMaxEffectC
	m_pTransform_BubbleMaxEffectC->Set_Scale(Vec3(m_fSizeX_Masked[2], m_fSizeY_Masked[2], 1.f));
	m_pTransform_BubbleMaxEffectC->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX) - (g_iWinSizeX * 0.5f), -(m_fY + 42.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_BubbleMaxEffectR
	m_pTransform_BubbleMaxEffectR->Set_Scale(Vec3(m_fSizeX_Masked[2], m_fSizeY_Masked[2], 1.f));
	m_pTransform_BubbleMaxEffectR->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 44.f) - (g_iWinSizeX * 0.5f), -(m_fY + 22.f) + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_fMaxBubbleGauge = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenMaxGauge();
	m_iCurrBubbleCount = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_CurrMarble();

	switch (m_iCurrBubbleCount)
	{
	case 0:
		m_fCurrBubbleGauge[0] = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenGage();
		m_fBubbleRatio = (_float)m_fCurrBubbleGauge[0] / (_float)m_fMaxBubbleGauge;
		break;
	case 1:
		m_fCurrBubbleGauge[0] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[1] = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenGage();
		m_fBubbleRatio = (_float)m_fCurrBubbleGauge[1] / (_float)m_fMaxBubbleGauge;
		break;
	case 2:
		m_fCurrBubbleGauge[0] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[1] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[2] = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenGage();
		m_fBubbleRatio = (_float)m_fCurrBubbleGauge[2] / (_float)m_fMaxBubbleGauge;
		break;
	case 3:
		m_fCurrBubbleGauge[0] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[1] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[2] = m_fMaxBubbleGauge;
		m_fBubbleRatio = 1.f;
		break;
	}
	
    return S_OK;
}

void CUI_IdentitySP_LinkedPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_IdentitySP_LinkedPlayer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Update_BubbleGauge(fTimeDelta);
	Update_BubbleCount(fTimeDelta);
	Update_BubblePopAnim(fTimeDelta);
	Update_BubbleMaxEffext(fTimeDelta);
}

HRESULT CUI_IdentitySP_LinkedPlayer::Render()
{
	if (FAILED(Bind_ShaderResources_BubbleGaugeL()))
		return E_FAIL;
	m_pShaderCom->Begin(21);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BubbleGaugeC()))
		return E_FAIL;
	m_pShaderCom->Begin(21);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BubbleGaugeR()))
		return E_FAIL;
	m_pShaderCom->Begin(21);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_SkillL()))
		return E_FAIL;	
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_SkillR()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_SkillFrameL()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_SkillFrameR()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_MaskedShineL()))
		return E_FAIL;
	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_MaskedShineC()))
		return E_FAIL;
	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_MaskedShineR()))
		return E_FAIL;
	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BubbleMaxEffectR()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BubbleMaxEffectC()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BubbleMaxEffectL()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_IdentitySP_LinkedPlayer::Update_BubbleGauge(_float fTimeDelta)
{
	m_iCurrBubbleCount = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_CurrMarble();
	switch (m_iCurrBubbleCount)
	{
	case 0:
		m_fCurrBubbleGauge[0] = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenGage();
		if (m_fMaxBubbleGauge == m_fCurrBubbleGauge[0])
			m_fRatioAngle = XM_PI;
		else
		{
			m_fBubbleRatio = (m_fCurrBubbleGauge[0] /m_fMaxBubbleGauge);
			m_fRatioAngle = -XM_PI + (2 * XM_PI * m_fBubbleRatio);
		}
		break;
	case 1:
		m_fCurrBubbleGauge[0] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[1] = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenGage();
		if (m_fMaxBubbleGauge == m_fCurrBubbleGauge[1])
			m_fRatioAngle = XM_PI;
		else
		{
			m_fBubbleRatio = (m_fCurrBubbleGauge[1] / m_fMaxBubbleGauge);
			m_fRatioAngle = -XM_PI + (2 * XM_PI * m_fBubbleRatio);
		}
		break;
	case 2:
		m_fCurrBubbleGauge[0] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[1] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[2] = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenGage();
		if (m_fMaxBubbleGauge == m_fCurrBubbleGauge[2])
			m_fRatioAngle = XM_PI;
		else
		{
			m_fBubbleRatio = (m_fCurrBubbleGauge[2] / m_fMaxBubbleGauge);
			m_fRatioAngle = -XM_PI + (2 * XM_PI * m_fBubbleRatio);
		}
		break;
	case 3:
		m_fCurrBubbleGauge[0] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[1] = m_fMaxBubbleGauge;
		m_fCurrBubbleGauge[2] = m_fMaxBubbleGauge;
		m_fBubbleRatio = 1.f;
		m_fRatioAngle = XM_PI;
		break;
	}

	Update_BubbleAnim(fTimeDelta);
}

void CUI_IdentitySP_LinkedPlayer::Update_BubbleAnim(_float fTimeDelta)
{
	for (size_t i = 0; i < 3; i++)
	{
		if ((i < m_iCurrBubbleCount) && (!m_bBubbleMax[i]))
		{
			m_fBubbleAnimFrame[i] += 30.f * fTimeDelta;
		}
		if ((!m_bBubbleMax[i])&&(11.f <= m_fBubbleAnimFrame[i]))
		{
			m_fBubbleAnimFrame[i] = 0.f;
			m_bBubbleMax[i] = true;
			m_fBubbleMaxEffect[i] = true;
			m_fBubbleMaxAlpha[i] = 1.f;
		}
	}
}

void CUI_IdentitySP_LinkedPlayer::Update_BubbleCount(_float fTimeDelta)
{
	if (m_iPreBubbleCount < m_iCurrBubbleCount)
	{
		m_iPreBubbleCount = m_iCurrBubbleCount;
	}
	if (m_iPreBubbleCount > m_iCurrBubbleCount)
	{
		_uint iIdentity = static_cast<CPlayer_Doaga*>(m_pPlayer)->Get_SP_Controller()->Get_IdenSkill();
		switch (iIdentity)
		{
		case CController_SP::MOON :
			if (2 < m_iPreBubbleCount)
			{
				m_bBubbleMax[0] = true;
				m_bBubbleMax[1] = false;
				m_bBubbleMax[2] = false;
				m_bMaskedEffect[2] = true;
				m_bMaskedEffect[1] = true;
				m_fMaskedEffectAlpha[2] = 1.f;
				m_fMaskedEffectAlpha[1] = 1.f;
			}
			else if (2 == m_iPreBubbleCount)
			{
				m_bBubbleMax[0] = false;
				m_bBubbleMax[1] = false;
				m_bMaskedEffect[0] = true;
				m_bMaskedEffect[1] = true;
				m_fMaskedEffectAlpha[1] = 1.f;
				m_fMaskedEffectAlpha[0] = 1.f;
			}
			break;

		case CController_SP::SUN:
			switch (m_iPreBubbleCount)
			{
			case 1:
				m_bBubbleMax[0] = false;
				m_bMaskedEffect[0] = true;
				m_fMaskedEffectAlpha[0] = 1.f;
				m_fBubbleMaxEffect[1] = true;
				m_fBubbleMaxAlpha[1] = 0.f;
				break;

			case 2:
				m_bBubbleMax[0] = true;
				m_bBubbleMax[1] = false;
				m_bMaskedEffect[1] = true;
				m_fMaskedEffectAlpha[1] = 1.f;
				m_fBubbleMaxEffect[2] = true;
				m_fBubbleMaxAlpha[2] = 0.f;
				break;

			case 3:
				m_bBubbleMax[0] = true;
				m_bBubbleMax[1] = true;
				m_bBubbleMax[2] = false;
				m_bMaskedEffect[2] = true;
				m_fMaskedEffectAlpha[2] = 1.f;
				break;
			}
			break;
		}
		m_iPreBubbleCount = m_iCurrBubbleCount;
	}
	if (TEXT("Dead_End") == m_pPlayer->Get_State())
	{
		for (size_t i = 0; i < 3; i++)
		{
			m_bBubbleMax[i] = false;
			m_bMaskedEffect[i] = false;
			m_fMaskedEffectAlpha[i] = 0.f;
		}
	}
}

void CUI_IdentitySP_LinkedPlayer::Update_BubblePopAnim(_float fTimeDelta)
{
	for (size_t i = 0; i < 3; i++)
	{
		if ((m_bMaskedEffect[i]) && (0.f < m_fMaskedEffectAlpha[i]))
		{
			m_fSizeX_Masked[i] += (20.f * fTimeDelta);
			m_fSizeY_Masked[i] += (20.f * fTimeDelta);
			m_fMaskedEffectAlpha[i] -= 2.5f * fTimeDelta;
			if(17.f > m_fBubblePopFrame[i])
				m_fBubblePopFrame[i] += (15.f * fTimeDelta);
			if (17.f < m_fBubblePopFrame[i])
				m_fBubblePopFrame[i] = 0.f;
		}
		else if ((m_bMaskedEffect[i]) && (0.f > m_fMaskedEffectAlpha[i]))
		{
			m_fSizeX_Masked[i] = 80.f;
			m_fSizeY_Masked[i] = 80.f;
			m_fMaskedEffectAlpha[i] = 0.f;
			m_bMaskedEffect[i] = false;
			m_fBubblePopFrame[i] = 1.f;
		}
		switch (i)
		{
		case 0:
			m_pTransform_Masked_EffectL->Set_Scale(Vec3(m_fSizeX_Masked[i], m_fSizeY_Masked[i], 1.f));
			break;
		case 1:
			m_pTransform_Masked_EffectC->Set_Scale(Vec3(m_fSizeX_Masked[i], m_fSizeY_Masked[i], 1.f));
			break;
		case 2:
			m_pTransform_Masked_EffectR->Set_Scale(Vec3(m_fSizeX_Masked[i], m_fSizeY_Masked[i], 1.f));
			break;
		}
	}
}

void CUI_IdentitySP_LinkedPlayer::Update_BubbleMaxEffext(_float fTimeDelta)
{
	for (size_t i = 0; i < 3; i++)
	{
		if (m_fBubbleMaxEffect[i])
		{
			if (1.f >= m_fBubbleMaxAlpha[i])
			{
				m_fBubbleMaxAlpha[i] -= 2.f * fTimeDelta;
				switch (i)
				{
				case 0:
					m_fSizeX_MaxEffect[i] += 20.f * fTimeDelta;
					m_fSizeY_MaxEffect[i] += 20.f * fTimeDelta;
					m_pTransform_BubbleMaxEffectL->Set_Scale(Vec3(m_fSizeX_MaxEffect[i], m_fSizeY_MaxEffect[i], 1.f));
					break;
				case 1 :
					m_fSizeX_MaxEffect[i] += 20.f * fTimeDelta;
					m_fSizeY_MaxEffect[i] += 20.f * fTimeDelta;
					m_pTransform_BubbleMaxEffectC->Set_Scale(Vec3(Vec3(m_fSizeX_MaxEffect[i], m_fSizeY_MaxEffect[i], 1.f)));
					break;
				case 2:
					m_fSizeX_MaxEffect[i] += 20.f * fTimeDelta;
					m_fSizeY_MaxEffect[i] += 20.f * fTimeDelta;
					m_pTransform_BubbleMaxEffectR->Set_Scale(Vec3(Vec3(m_fSizeX_MaxEffect[i], m_fSizeY_MaxEffect[i], 1.f)));
					break;
				}
			}
			if (0.f > m_fBubbleMaxAlpha[i])
			{
				m_fBubbleMaxAlpha[i] = 0.f;
				m_fBubbleMaxEffect[i] = false;
				switch (i)
				{
				case 0:
					m_fSizeX_MaxEffect[i] = { 167.f * 0.5f };
					m_fSizeY_MaxEffect[i] = { 160.f * 0.5f };
					m_pTransform_BubbleMaxEffectL->Set_Scale(Vec3(Vec3(m_fSizeX_MaxEffect[i], m_fSizeY_MaxEffect[i], 1.f)));
					break;
				case 1:
					m_fSizeX_MaxEffect[i] = { 167.f * 0.5f };
					m_fSizeY_MaxEffect[i] = { 160.f * 0.5f };
					m_pTransform_BubbleMaxEffectC->Set_Scale(Vec3(Vec3(m_fSizeX_MaxEffect[i], m_fSizeY_MaxEffect[i], 1.f)));
					break;
				case 2:
					m_fSizeX_MaxEffect[i] = { 167.f * 0.5f };
					m_fSizeY_MaxEffect[i] = { 160.f * 0.5f };
					m_pTransform_BubbleMaxEffectR->Set_Scale(Vec3(Vec3(m_fSizeX_MaxEffect[i], m_fSizeY_MaxEffect[i], 1.f)));
					break;
				}
			}
		}
	}
}

HRESULT CUI_IdentitySP_LinkedPlayer::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	//m_pTexture_BubbleGauge
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Bubble_Gauge"),
		TEXT("Com_Texture_BubbleGaugeL"), (CComponent**)&m_pTexture_BubbleGaugeL)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Bubble_Gauge"),
		TEXT("Com_Texture_BubbleGaugeC"), (CComponent**)&m_pTexture_BubbleGaugeC)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Bubble_Gauge"),
		TEXT("Com_Texture_BubbleGaugeR"), (CComponent**)&m_pTexture_BubbleGaugeR)))
		return E_FAIL;
	//m_pTexture_BubbleAnim
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Bubble_Animation"),
		TEXT("Com_Texture_BubbleAnim"), (CComponent**)&m_pTexture_BubbleAnim)))
		return E_FAIL;
	//m_pTexture_Masked_Effect
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Masked_Effect"),
		TEXT("Com_Texture_Masked_Effect"), (CComponent**)&m_pTexture_Masked_Effect)))
		return E_FAIL;
	//m_pTexture_MoonFall
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_MoonFall"),
		TEXT("Com_Texture_MoonFall"), (CComponent**)&m_pTexture_MoonFall)))
		return E_FAIL;
	//m_pTexture_SunRise
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Sunrise"),
		TEXT("Com_Texture_SunRise"), (CComponent**)&m_pTexture_SunRise)))
		return E_FAIL;
	//m_pSkill_Frame
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Skill_Cap"),
		TEXT("Com_Texture_SkillFrame"), (CComponent**)&m_pTexture_SkillFrame)))
		return E_FAIL;
	//m_pTexture_BubblePop
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_BubblePop"),
		TEXT("Com_Texture_BubblePop"), (CComponent**)&m_pTexture_BubblePop)))
		return E_FAIL;
	//m_pTexture_BubbleMaxEffect
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_ShineEffect"),
		TEXT("Com_Texture_BubbleMaxEffect"), (CComponent**)&m_pTexture_BubbleMaxEffect)))
		return E_FAIL;

	//m_pTransform_BubbleGaugeL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleGaugeL"), (CComponent**)&m_pTransform_BubbleGaugeL)))
		return E_FAIL;
	//m_pTransform_BubbleGaugeC
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleGaugeC"), (CComponent**)&m_pTransform_BubbleGaugeC)))
		return E_FAIL;
	//m_pTransform_BubbleGaugeR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleGaugeR"), (CComponent**)&m_pTransform_BubbleGaugeR)))
		return E_FAIL;
	//m_pTransform_SkillL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillL"), (CComponent**)&m_pTransform_SkillL)))
		return E_FAIL;
	//m_pTransform_SkillR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillR"), (CComponent**)&m_pTransform_SkillR)))
		return E_FAIL;
	//m_pTransform_SkillFrameL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillFrameL"), (CComponent**)&m_pTransform_SkillFrameL)))
		return E_FAIL;
	//m_pTransform_SkillFrameR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillFrameR"), (CComponent**)&m_pTransform_SkillFrameR))) 
		return E_FAIL;
	//m_pTransform_Masked_EffectL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Masked_EffectL"), (CComponent**)&m_pTransform_Masked_EffectL)))
		return E_FAIL;
	//m_pTransform_Masked_EffectC
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Masked_EffectC"), (CComponent**)&m_pTransform_Masked_EffectC)))
		return E_FAIL;
	//m_pTransform_Masked_EffectR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Masked_EffectR"), (CComponent**)&m_pTransform_Masked_EffectR)))
		return E_FAIL;
	//m_pTransform_BubbleMaxEffectL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleMaxEffectL"), (CComponent**)&m_pTransform_BubbleMaxEffectL)))
		return E_FAIL;
	//m_pTransform_BubbleMaxEffectC
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleMaxEffectC"), (CComponent**)&m_pTransform_BubbleMaxEffectC)))
		return E_FAIL;
	//m_pTransform_BubbleMaxEffectR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleMaxEffectR"), (CComponent**)&m_pTransform_BubbleMaxEffectR)))
		return E_FAIL;
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources()
{


    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleGaugeL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BubbleGaugeL->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	if (0 == m_iCurrBubbleCount)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fRatioAngle, sizeof(_float))))
			return E_FAIL;
		m_pTexture_BubbleGaugeL->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0);
	}
	else if (0 < m_iCurrBubbleCount)
	{
		_float fRatio = 3.14f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &fRatio, sizeof(_float))))
			return E_FAIL;
		if (!m_bBubbleMax[0])
			m_pTexture_BubbleAnim->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fBubbleAnimFrame[0]);
		else
			m_pTexture_BubbleGaugeL->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 1);
	}
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleGaugeC()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BubbleGaugeC->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	_float fRatio;
	if (1 > m_iCurrBubbleCount)
	{
		fRatio = -3.14f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &fRatio, sizeof(_float))))
			return E_FAIL;
		m_pTexture_BubbleGaugeC->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0);
	}
	else if (1 == m_iCurrBubbleCount)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fRatioAngle, sizeof(_float))))
			return E_FAIL;
		m_pTexture_BubbleGaugeC->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0);
	}
	else if (1 < m_iCurrBubbleCount)
	{
		fRatio = 3.14f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &fRatio, sizeof(_float))))
			return E_FAIL;
		if (!m_bBubbleMax[1])
			m_pTexture_BubbleAnim->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fBubbleAnimFrame[1]);
		else
			m_pTexture_BubbleGaugeC->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 1);
	}
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleGaugeR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BubbleGaugeR->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	_float fRatio;
	if (2 > m_iCurrBubbleCount)
	{
		fRatio = -3.14f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &fRatio, sizeof(_float))))
			return E_FAIL;
		m_pTexture_BubbleGaugeR->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0);
	}
	else if (2 == m_iCurrBubbleCount)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fRatioAngle, sizeof(_float))))
			return E_FAIL;
		m_pTexture_BubbleGaugeR->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0);
	}
	else if (2 < m_iCurrBubbleCount)
	{
		fRatio = 3.14f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &fRatio, sizeof(_float))))
			return E_FAIL;
		if(!m_bBubbleMax[2])
			m_pTexture_BubbleAnim->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fBubbleAnimFrame[2]);
		else
			m_pTexture_BubbleGaugeR->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 1);
	}
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_MaskedShineL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Masked_EffectL->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fMaskedEffectAlpha[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BubblePop->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fBubblePopFrame[0]);
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_MaskedShineC()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Masked_EffectC->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fMaskedEffectAlpha[1], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BubblePop->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fBubblePopFrame[1]);
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_MaskedShineR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Masked_EffectR->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fMaskedEffectAlpha[2], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BubblePop->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fBubblePopFrame[2]);
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleMaxEffectL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BubbleMaxEffectL->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fBubbleMaxAlpha[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BubbleMaxEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}


HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleMaxEffectC()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BubbleMaxEffectC->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fBubbleMaxAlpha[1], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BubbleMaxEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}


HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleMaxEffectR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BubbleMaxEffectR->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fBubbleMaxAlpha[2], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BubbleMaxEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_SkillL->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (2 > m_iCurrBubbleCount)
		m_fSkillAlpha[0] = 0.6f;
	else
		m_fSkillAlpha[0] = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fSkillAlpha[0], sizeof(_float))))
		return E_FAIL;
	m_pTexture_MoonFall->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_SkillR->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (1 > m_iCurrBubbleCount)
		m_fSkillAlpha[1] = 0.6f;
	else
		m_fSkillAlpha[1] = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fSkillAlpha[1], sizeof(_float))))
		return E_FAIL;
	m_pTexture_SunRise->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillFrameL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_SkillFrameL->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_SkillFrame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillFrameR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_SkillFrameR->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_SkillFrame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

CUI_IdentitySP_LinkedPlayer* CUI_IdentitySP_LinkedPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentitySP_LinkedPlayer* pInstance = new CUI_IdentitySP_LinkedPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentitySP_LinkedPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentitySP_LinkedPlayer::Clone(void* pArg)
{
	CUI_IdentitySP_LinkedPlayer* pInstance = new CUI_IdentitySP_LinkedPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentitySP_LinkedPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentitySP_LinkedPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTexture_BubbleGaugeL);
	Safe_Release(m_pTexture_BubbleGaugeC);
	Safe_Release(m_pTexture_BubbleGaugeR);
	Safe_Release(m_pTexture_Masked_Effect);
	Safe_Release(m_pTexture_MoonFall);
	Safe_Release(m_pTexture_SunRise);
	Safe_Release(m_pTexture_SkillFrame);
	Safe_Release(m_pTexture_BubblePop);
	Safe_Release(m_pTexture_BubbleMaxEffect);

	Safe_Release(m_pTransform_BubbleGaugeL);
	Safe_Release(m_pTransform_BubbleGaugeC);
	Safe_Release(m_pTransform_BubbleGaugeR);
	Safe_Release(m_pTransform_Masked_EffectL);
	Safe_Release(m_pTransform_Masked_EffectC);
	Safe_Release(m_pTransform_Masked_EffectR);
	Safe_Release(m_pTransform_SkillL);
	Safe_Release(m_pTransform_SkillR);
	Safe_Release(m_pTransform_SkillFrameL);
	Safe_Release(m_pTransform_SkillFrameR);
	Safe_Release(m_pTransform_BubbleMaxEffectL);
	Safe_Release(m_pTransform_BubbleMaxEffectC);
	Safe_Release(m_pTransform_BubbleMaxEffectR);
}
