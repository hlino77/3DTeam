#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"
#include "Client_Shader_Effect.hlsl"

matrix g_ViewMatrixInv;
matrix g_ProjMatrixInv;

VS_OUT_FXDECAL VS_MAIN_FXDECAL( /* ���� */FXDECAL_IN In)
{
    VS_OUT_FXTEX Out = (VS_OUT_FXTEX) 0;

    float4 vRight = WorldMatrix._11_12_13_14;
    float4 vUp = WorldMatrix._21_22_23_24;

    Out.vPosition = mul(float4(In.vPosition, 1.f), WorldMatrix);
	Out.vPSize = float2(In.vTexcoord.x * length(vRight), In.vTexcoord.y * length(vUp));

	return Out;	
}

cbuffer TransformInverse
{
    matrix WorldInv;
};

PS_OUT_EFFECT PS_MAIN_FXTEX(VS_OUT_FXDECAL In)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;
    
    // Decal Box�� x,y ��ġ�� UV��ǥ�� ��ȯ.
    float2 vTexUV;
    vTexUV.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
	
	// Decal Box x,y ��ġ �������� ���� ���� ������.
    float4 vDepthDesc = g_DepthTarget.Sample(PointSampler, vTexUV);
    // ĳ���� �ȼ��� ����
    if (vDepthDesc.z == 1.f)
        discard;
    
    float fViewZ = vDepthDesc.y * 1000.f;

    float4 vProjPos = (float4) 0.f;
    vProjPos.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
    vProjPos.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
    vProjPos.z = (vDepthDesc.x) * fViewZ;
    vProjPos.w = fViewZ;

    float4 vViewPos = mul(vProjPos, g_ProjMatrixInv);
    float4 vWorldPos = mul(vViewPos, ViewInv);
    float4 vLocalPos = mul(vWorldPos, WorldInv);
    
    // ������ �������� 0.5 �̱⶧����, 0.5�� �� �������� ���� ���̹Ƿ� �ڸ�.
    float3 fObjectAbsPos = abs(vLocalPos.xyz);
    clip(0.5f - fObjectAbsPos);

	// ��Į�ڽ� ���۰� -0.5 ~0.5 ���̹Ƿ�, 0.5�� �����༭ UV��ǥ�� �������.
    float2 vNewUV = vLocalPos.xz + 0.5f;
    float4 vDiffuse = g_DiffuseTexture.Sample(LinearSampler, vNewUV);
    
    // ������� �� �ڵ� ������ �ǳ�...
    
    vDiffuse.a -= g_fColor_Alpha;
    if (vDiffuse.a <= g_fAlpha_Discard ||
		vDiffuse.r <= g_fBlack_Discard.r && vDiffuse.g <= g_fBlack_Discard.g && vDiffuse.b <= g_fBlack_Discard.b)
        discard;
        
    Out.vColor = float4(0.f, 0.f, 0.f, 0.f);
    Out.vEmissive = float4(0.f, 0.f, 0.f, 0.f);
    
    if (vDiffuse.a >= g_fColor_Add_01_Alpha)
    {
        vDiffuse.rgb += g_fColor_Add_01;
        Out.vEmissive = Caculation_Brightness(vDiffuse);
    }
    else
        vDiffuse.rgb += g_fColor_Add_02;
    
    Out.vColor = vDiffuse;
    
    
    /////////////////////////////////////////////////
    float2 vNewUV = float2(0.f, 0.f);
    
    if (!bUV_Wave)
        vNewUV = (In.vTexcoord + vUV_TileIndex) / vUV_TileCount + vUV_Offset;
    else
    {
        vNewUV = (In.vTexcoord - 0.5f) * 2.f;
        float2 vDir = normalize(vNewUV);
        vNewUV += vDir * vUV_Offset.x;
        vNewUV.x -= int(vNewUV.x);
        vNewUV.y -= int(vNewUV.y);
        vNewUV = vNewUV * 0.5f + 0.5f;
    }
       
    
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
        VertexShader = compile vs_5_0 VS_MAIN_FXTEX();
        GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTEX();
        ComputeShader = NULL;
    }

}
