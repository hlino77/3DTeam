#include "Client_Shader_Defines.hlsl"
#include "Client_Shader_InOut.hlsl"
#include "Client_Shader_Global.hlsl"
#include "Client_Shader_Light.hlsl"

VS_OUT_FXTEX VS_MAIN_FXTEX( /* ���� */FXTEX_IN In)
{
    VS_OUT_FXTEX Out = (VS_OUT_FXTEX) 0;

    vector vRight = WorldMatrix._11_12_13_14;
    vector vUp = WorldMatrix._21_22_23_24;

    Out.vPosition = mul(float4(In.vPosition, 1.f), WorldMatrix);
	Out.vPSize = float2(In.vPSize.x * length(vRight), In.vPSize.y * length(vUp));

	return Out;	
}

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
    //float4		vNormal : NORMAL;
	float2		vTexcoord : TEXCOORD0;
    //float4		vProjPos : TEXCOORD1;
};

/* �������� ������ �����Ѵ�. */
[maxvertexcount(20)]
void GS_MAIN_FXTEX(point VS_OUT_FXTEX In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];

	/* �޾ƿ� ������ ���������Ͽ� �簢���� �����ϱ����� ���� �������� ����ž�. */
    float3  vLook = CameraPosition() - In[0].vPosition.xyz;

	float3	vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].vPSize.x * 0.5f;
	float3	vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y * 0.5f;

	matrix		matVP;

	matVP = ViewProj;

	Out[0].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexcoord = float2(0.0f, 0.f);

	Out[1].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexcoord = float2(1.0f, 0.f);

	Out[2].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexcoord = float2(1.0f, 1.0f);

	Out[3].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexcoord = float2(0.0f, 1.0f);
	
	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[3]);
	OutStream.RestartStrip();
}

/* w������ ����. ������ ������ȯ. */
/* ����Ʈ�����̽�(��������ǥ)�� ��ġ�� ��ȯ�Ѵ�. */
/* �����Ͷ����� : ������ �ѷ����� �ȼ��� ������ �����Ѵ�. */
/* �ȼ������� ���������� ����Ѵ�. */

/* ���޹��� �ȼ��� ������ �̿��Ͽ� �ȼ��� �������� ���� ��������. */
float4 PS_MAIN_FXTEX(GS_OUT In) : SV_TARGET0
{
    float2 vNewUV = In.vTexcoord;
    float  fMask = 0.f;
    float3 vEmissive = float3(0.f, 0.f, 0.f);
     
    if (EPSILON < NoisMaskEmisDslv.x)   // Noise
    {
        vNewUV = g_NoiseTexture.Sample(LinearSampler, In.vTexcoord).rg;
        vNewUV += (vNewUV - 0.5f) * 2.f;
    }   
    if (EPSILON < NoisMaskEmisDslv.y)   // Mask
    {
        fMask = g_MaskTexture.Sample(LinearSampler, vNewUV).r;
        clip(fMask);
    }
    
    float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vColor.a = fMask;
    
    if (EPSILON < NoisMaskEmisDslv.z)	// Emissive
    {
        float3 vEmissive = g_EmissiveTexture.Sample(LinearSampler, vNewUV).rgb;
        vColor.rgb += vEmissive;
    }
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
    
    return vColor;
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
        GeometryShader = compile gs_5_0 GS_MAIN_FXTEX();
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FXTEX();
        ComputeShader = NULL;
    }
}




