#ifndef _CLIENT_SHADER_INOUT_HLSL_
#define _CLIENT_SHADER_INOUT_HLSL_

struct STATIC_IN
{
    float3  vPosition : POSITION;
    float3  vNormal : NORMAL;
    float3  vTangent : TANGENT;
    float2  vTexUV : TEXCOORD0;
};

struct STATIC_INSTANCE_IN
{
    float3		vPosition : POSITION;
    float3		vNormal : NORMAL;
    float2		vTexUV : TEXCOORD0;
    float3		vTangent : TANGENT;

    matrix		matWorld : INST;
};

struct FXMESH_IN
{
    float3  vPosition : POSITION;
    float3  vNormal : NORMAL;
    float3  vTangent : TANGENT;
    float2  vTexUV : TEXCOORD0;
};
    
struct FXTEX_IN
{
    float3 vPosition : POSITION;
    float2 vPSize   : PSIZE;
};

struct SKELETAL_IN
{
    float3  vPosition : POSITION;
    float3  vNormal : NORMAL;
    float3  vTangent : TANGENT;
    float2  vTexUV : TEXCOORD0;
    uint4   vBlendIndex : BLENDINDEX;
    float4  vBlendWeight : BLENDWEIGHT;
};

struct SKELETAL_INSTANCE_IN
{
    float3  vPosition : POSITION;
    float3  vNormal : NORMAL;
    float3  vTangent : TANGENT;
    float2  vTexUV : TEXCOORD0;
    uint4   vBlendIndex : BLENDINDEX;
    float4  vBlendWeight : BLENDWEIGHT;

    uint    iID : INSTANCEID;
    matrix	matWorld : INST;
};

struct PARTICLE_IN
{
    float3 vPosition : POSITION;
    float3 vVelocity : VELOCITY;
    float2 vSize : SIZE;
    float fAge : AGE;
    uint iType : TYPE;
};

struct VS_OUT
{
    float4  vPosition : SV_POSITION;
    float4  vNormal : NORMAL;
    float3  vNormalV : NORMALV;
    float2  vTexUV : TEXCOORD0;
    float4  vProjPos : TEXCOORD1;
    float3  vTangent : TANGENT;
};

struct VS_OUT_INSTANCE
{
    float4  vPosition : SV_POSITION;
    float4  vNormal : NORMAL;
    float3  vNormalV : NORMALV;
    float2  vTexUV : TEXCOORD0;
    float4  vProjPos : TEXCOORD1;
    float3  vTangent : TANGENT;
    float   fRimLight : RIM;
};

struct VS_OUT_SHADOW
{
    float4  vPosition : SV_POSITION;
    float4  vProjPos : TEXCOORD1;
};

struct VS_OUT_FXTEX
{
    float4 vPosition : POSITION;    // SV �ƴ�
    float2 vPSize : PSIZE;
};

struct TARGET_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT_TARGET
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT_PBR
{
    float4  vDiffuse : SV_TARGET0;
    float4  vNormal : SV_TARGET1;
    float4  vNormalV : SV_TARGET2;
    float4  vMetallic : SV_TARGET3;
    float4  vRoughness : SV_TARGET4;
	float4  vEmissive : SV_TARGET5;
};
	
struct PS_OUT_PHONG
{
    float4  vDiffuse : SV_TARGET0;
    float4  vNormal : SV_TARGET1;
    float4  vNormalV : SV_TARGET2;
	
	//float4 vEmissive : SV_TARGET5;
};

struct PS_OUT_EFFECT
{
    float4  vDiffuse : SV_TARGET0;
    float4  vBlur : SV_TARGET1;
};

#endif