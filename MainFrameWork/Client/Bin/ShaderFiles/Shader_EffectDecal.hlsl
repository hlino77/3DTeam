#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"
#include "Client_Shader_Effect.hlsl"

matrix g_ViewMatrixInv;
matrix g_ProjMatrixInv;

VS_OUT_FXDECAL VS_MAIN_FXDECAL( /* ���� */FXDECAL_IN In)
{
    VS_OUT_FXDECAL Out = (VS_OUT_FXDECAL) 0;

    matrix matWVP;
	
    matWVP = mul(WorldMatrix, ViewProj);
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

	return Out;	
}

cbuffer TransformInverse
{
    matrix WorldInv;
};

Texture2D g_NormalDepthTarget;
Texture2D g_NormalTarget;
Texture2D g_PropertiesTarget;

PS_OUT_EFFECT PS_MAIN_FXDECAL(VS_OUT_FXDECAL In)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;
    
    float2 vTexUV;
    vTexUV.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    // Decal Box�� x,y ��ġ�� UV��ǥ�� ��ȯ.
	
    if (0.f != g_PropertiesTarget.Sample(PointSampler, vTexUV).z)
        discard;
    
    float4 vNormalDepth = g_NormalDepthTarget.Sample(PointSampler, vTexUV);
    float4 vNormal = g_NormalTarget.Sample(PointSampler, vTexUV);
    
    //vNormal.xyz = vNormal.xyz * 2.f - 1.f;
    //if (dot(vNormal.xyz, float3(0.f, 1.f, 0.f)) < 0.86f)
    //    discard;
    
    float fViewZ = vNormalDepth.w * 1200.f;
	// Decal Box x,y ��ġ �������� ���� ���� �����´�.
    
    // ĳ���� �ȼ��� ����
    //if (vNormalDepth.z == 1.f)
    //    discard;    

    float4 vProjPos = 0.f;
    vProjPos.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
    vProjPos.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
    vProjPos.z = (vNormal.w) * fViewZ;
    vProjPos.w = fViewZ;

    float4 vViewPos = mul(vProjPos, g_ProjMatrixInv);
    float4 vWorldPos = mul(vViewPos, g_ViewMatrixInv);
    float4 vLocalPos = mul(vWorldPos, WorldInv);
    
    float3 vObjectAbsPos = abs(vLocalPos.xyz);
    
    clip(0.5f - vObjectAbsPos);
    // ������ �������� 0.5 �̱⶧����, 0.5�� �� �������� ���� ���̹Ƿ� �ڸ�.

	// ��Į�ڽ� ���۰� -0.5 ~0.5 ���̹Ƿ�, 0.5�� �����༭ UV��ǥ�� �������.
    float2 vDecalUV = vLocalPos.xz + 0.5f;

    float2 vNewUV = vDecalUV;
    
    float  fMask = 1.f;
    float3 vEmissive = float3(0.f, 0.f, 0.f);

    if (EPSILON < NoisMaskEmisDslv.x)   // Noise
    {
        vNewUV = g_NoiseTexture.Sample(LinearSampler, vNewUV).rg;
        vNewUV += (vNewUV - 0.5f) * 2.f;
    }
    
    if (EPSILON < NoisMaskEmisDslv.y)   // Mask
    {
        fMask = g_MaskTexture.Sample(LinearSampler, vNewUV).r;
        clip(fMask - 0.01f);
    }

    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, vNewUV);
    
    vColor += vColor_Offset;

    vColor *= vColor_Mul * fMask;

    if (vColor.r + 0.01f < vColor_Clip.r && vColor.g + 0.01f < vColor_Clip.g && vColor.b + 0.01f < vColor_Clip.b)
        discard;

    clip(vColor.a - vColor_Clip.a);

	if (EPSILON < NoisMaskEmisDslv.w)	// Dissolve
    {
        float fDissolve = g_DissolveTexture.Sample(LinearSampler, vNewUV).x;
        
	    //Discard the pixel if the value is below zero
        clip(fDissolve - g_fDissolveAmount);
	    //Make the pixel emissive if the value is below ~f
        //if (fDissolve - g_fDissolveAmount < 0.25f)/*0.08f*/
        //{
        //    vEmissive = float3(0.3f, 0.3f, 0.3f);
        //}
    }
    
    //if (any(vColor_Offset))
    
    Out.vColor = vColor;
    
    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        Out.vEmissive = Out.vColor * fIntensity_Bloom;
    }
    
    return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
        SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		/* ���� ���̴��� ���ؼ� ���� � �������� �����ϰ� � �Լ��� ȣ���Ͽ� �ش� ���̴��� �����Ǵ����� �����Ѵ�. */
        VertexShader = compile vs_5_0 VS_MAIN_FXDECAL();
        GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXDECAL();
        ComputeShader = NULL;
    }

}
