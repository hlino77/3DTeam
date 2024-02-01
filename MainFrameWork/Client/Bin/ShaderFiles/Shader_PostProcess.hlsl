#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_Light.hlsl"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

matrix      g_PreCamViewMatrix, g_CamProjMatrix, g_CamViewMatrix;
matrix      g_ViewMatrixInv, g_ProjMatrixInv;
matrix      g_ProjViewMatrixInv;

Texture2D	g_PrePostProcessTarget;
Texture2D   g_SSRTarget;
Texture2D	g_DecalOneBlendTarget;
Texture2D	g_DecalAlphaBlendTarget;
Texture2D	g_EffectOneBlendTarget;
Texture2D	g_EffectAlphaBlendTarget;
Texture2D   g_NormalTarget;
Texture2D   g_NormalDepthTarget;
Texture2D	g_ShadeTarget;
Texture2D	g_BloomTarget;
Texture2D	g_BlurTarget;
Texture2D	g_DistortionTarget;
Texture2D	g_BlendedTarget;
Texture2D   g_MotionBlurTarget;
Texture2D   g_RadialBlurTarget;

Texture2D   g_ProcessingTarget;

Texture2D   g_PostProcessedTarget;

Texture2D   g_Texture;

float2      g_PixelSize;
int         g_KernelSize; //Ŀ���� �̹��� ó������ ���� �Ǵ� �������� �Ҹ��� ���� ��� �Ǵ� ����ũ
float       g_CenterWeight;
float	    g_WeightAtt;

//Chromatic Aberration
cbuffer ChromaticBlur
{
    float3 vChromaticPosition;
    float fChromaticIntensity;
}

//Motion Blur
float       g_fMotionBlurIntensity = 0.f;

//Radial Blur
cbuffer RadialBlur
{
    float3 vBlurWorldPosition;
    float fRadialBlurIntensity;
}

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{	
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(/* ���� */VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

float4 PS_MAIN_POSTPROCESS(PS_IN In) : SV_TARGET0
{    // �ϴ� �״�� ����
    return g_ProcessingTarget.Sample(LinearSampler, In.vTexcoord);
}

cbuffer ScreenTone
{
    float g_fGrayScale = 1.f;
    float g_fContrastValue = 1.f;
    float g_fSaturation = 1.f;
}

float4 PS_MAIN_SCREENTONE(PS_IN In) : SV_TARGET0
{
    float4 vColor = g_BlendedTarget.Sample(LinearSampler, In.vTexcoord);
	
    vColor *= g_fSaturation;
	
    float avg = (vColor.r + vColor.g + vColor.b) / 3.0;
    float4 vNewColor;
    vNewColor.a = 1.0f;
    vNewColor.rgb = avg * (1.0 - g_fGrayScale) + vColor.rgb * g_fGrayScale;

    return 0.5f + g_fContrastValue * (vNewColor - 0.5f);
}

float Tonemap_ACES(float x)
{
    // Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

float4 PS_MAIN_BLENDEFFECT(PS_IN In) : SV_TARGET0
{
    float fDistortion = g_DistortionTarget.Sample(LinearClampSampler, In.vTexcoord).x;
    float4 vColor = g_PrePostProcessTarget.Sample(LinearSampler, In.vTexcoord + float2(fDistortion, fDistortion));
    
    float4 vSSR = g_SSRTarget.Sample(LinearSampler, In.vTexcoord + float2(fDistortion, fDistortion));
    
    if (EPSILON < vSSR.a)
    {
        vColor = float4(vSSR.rgb * vSSR.a + vColor.rgb * (1.f - vSSR.a), 1.f);
    }
    
    float4 vDecalOneBlend = g_DecalOneBlendTarget.Sample(LinearSampler, In.vTexcoord);
    float4 vDecalAlphaBlend = g_DecalAlphaBlendTarget.Sample(LinearSampler, In.vTexcoord);
	
	float4 vEffectOneBlend = g_EffectOneBlendTarget.Sample(LinearSampler, In.vTexcoord);
	float4 vEffectAlphaBlend = g_EffectAlphaBlendTarget.Sample(LinearSampler, In.vTexcoord);
	
    if (EPSILON < vDecalOneBlend.a)
	{
        vColor = float4(vDecalOneBlend.rgb * 1.f + vColor.rgb * 1.f, 1.f);
    }
	
    if (EPSILON < vDecalAlphaBlend.a)
    {
        vColor = float4(vDecalAlphaBlend.rgb * vDecalAlphaBlend.a + vColor.rgb * (1.f - vDecalAlphaBlend.a), 1.f);
    }

    if (EPSILON < vEffectOneBlend.a)
	{
        vColor = float4(vEffectOneBlend.rgb * 1.f + vColor.rgb * 1.f, 1.f);
    }
	
    if (EPSILON < vEffectAlphaBlend.a)
    {
        vColor = float4(vEffectAlphaBlend.rgb * vEffectAlphaBlend.a + vColor.rgb * (1.f - vEffectAlphaBlend.a), 1.f);
    }
	
    float4 vBloom = g_BloomTarget.Sample(LinearSampler, In.vTexcoord);

    //float fBrightness = dot(vBloom.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    //vBloom *= Tonemap_ACES(fBrightness);

    vColor += vBloom;
    
    vColor = pow(vColor, 1.f / 2.2f);
    
    return vColor;
}

float4 PS_MAIN_CHROMATIC(PS_IN In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    matrix matVP = mul(g_CamViewMatrix, g_CamProjMatrix);
    float4 vChromaticCenter = mul(float4(vChromaticPosition, 1.f), matVP);
    vChromaticCenter /= vChromaticCenter.w;

    vChromaticCenter.x = vChromaticCenter.x * 0.5f + 0.5f;
    vChromaticCenter.y = vChromaticCenter.y * -0.5f + 0.5f;
	
    float2 center = float2(vChromaticCenter.x, vChromaticCenter.y);
    
    float2 BlurDir = In.vTexcoord - center;

    vColor.r = g_ProcessingTarget.Sample(LinearClampSampler, In.vTexcoord).r;
    vColor.g = g_ProcessingTarget.Sample(LinearClampSampler, In.vTexcoord - BlurDir * fChromaticIntensity * 0.5f).g;
    vColor.b = g_ProcessingTarget.Sample(LinearClampSampler, In.vTexcoord - BlurDir * fChromaticIntensity).b;

	//vColor *= (1.0 - g_BlurStrength * 0.5); //���ϸ� �׳� �����

    return vColor;
}

float4 PS_MAIN_MOTIONBLUR(PS_IN In) : SV_TARGET0
{
    float4 vNormalDepth = g_NormalDepthTarget.Sample(LinearSampler, In.vTexcoord);
    float4 vNormal = g_NormalTarget.Sample(LinearSampler, In.vTexcoord);
		   
    float fViewZ = vNormalDepth.w * 1200.f;
		    
    float4 vPixelWorldPos, vPixelPos;

    vPixelWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vPixelWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vPixelWorldPos.z = vNormal.w;
    vPixelWorldPos.w = 1.0f;

    vPixelPos = vPixelWorldPos;
    vPixelWorldPos *= fViewZ;

    vPixelWorldPos = mul(vPixelWorldPos, g_ProjMatrixInv);
    vPixelWorldPos = mul(vPixelWorldPos, g_ViewMatrixInv);
    //vPixelWorldPos = mul(vPixelWorldPos, g_ProjViewMatrixInv);
    
    matrix matVP = mul(g_PreCamViewMatrix, g_CamProjMatrix);

    float4 vPrePixelPos = mul(vPixelWorldPos, matVP);
    vPrePixelPos /= vPrePixelPos.w;

    float2 vPixelVelocity = ((vPixelPos - vPrePixelPos) * 0.5f).xy;
    float2 texCoord = In.vTexcoord;

    float4 vColor = vector(0.f, 0.f, 0.f, 0.f);

    for (int i = -10; i < 10; ++i)
    {
        texCoord += vPixelVelocity * g_fMotionBlurIntensity * i;
        float4 currentColor = g_ProcessingTarget.Sample(LinearClampSampler, texCoord);
        vColor += currentColor;
    }

    return vColor * 0.05f/*/ 20.f*/;
}

float4 PS_MAIN_RADIALBLUR(PS_IN In) : SV_TARGET0
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);

    float fBlurStart = 1.f;
	
    matrix matVP = mul(g_CamViewMatrix, g_CamProjMatrix);
    float4 vBlurCenter = mul(float4(vBlurWorldPosition, 1.f), matVP);
    vBlurCenter /= vBlurCenter.w;

    vBlurCenter.x = vBlurCenter.x * 0.5f + 0.5f;
    vBlurCenter.y = vBlurCenter.y * -0.5f + 0.5f;
	
    float2 center = float2(vBlurCenter.x, vBlurCenter.y); //�߽���<-���콺�� ��ġ�� �޾ƿ��� ���콺�� �߽����� ����
	
    // g_RadialBlurTarget.Sample(LinearClampSampler, In.vTexcoord);
    
    In.vTexcoord.xy -= center;

    float fPrecompute = fRadialBlurIntensity * (1.0f / 19.f);
    int iDivision = 0;
	
    for (uint i = 0; i < 20; ++i)
    {
        float scale = fBlurStart + (float(i) * fPrecompute);
        float2 uv = In.vTexcoord.xy * scale + center;
		
        if (0.f > uv.x || 1.f < uv.x)
            continue;
		
        if (0.f > uv.y || 1.f < uv.y)
            continue;
        vColor += g_ProcessingTarget.Sample(LinearClampSampler, uv);
        ++iDivision;
    }

    vColor /= (float) iDivision;
	
    //float2 vBlurDir = In.vTexUV.xy - center;
	
    //for (int i = 0; i < 10; ++i)
    //{
    //    float4 currentColor = g_OriginalRenderTexture.Sample(ClampSampler, In.vTexUV + vBlurDir * g_fRadialBlurStrength*0.05f * i);
    //    vColor += currentColor;
    //}

    return vColor;
}

technique11 DefaultTechnique
{
	pass PostProcess // 0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POSTPROCESS();
    }

	pass ScreenTone // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SCREENTONE();
    }

    pass BlendEffect // 3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLENDEFFECT();
    }

    pass ChromaticAberration // 4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_CHROMATIC();
    }

    pass MotionBlur // 5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MOTIONBLUR();
    }

    pass RadialBlur // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
      //SetDepthStencilState(DSS_None_ZTestWrite_True_StencilTest, 1);
      //SetDepthStencilState(DSS_None_ZTest_And_Write, 0);    
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        HullShader = NULL;
        DomainShader = NULL;
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_RADIALBLUR();
    }


}




