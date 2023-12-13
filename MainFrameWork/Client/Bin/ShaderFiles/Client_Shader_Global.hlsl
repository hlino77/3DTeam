#ifndef _CLIENT_SHADER_GLOBAL_HLSL_
#define _CLIENT_SHADER_GLOBAL_HLSL_

cbuffer GlobalBuffer
{
    matrix ViewMatrix;
    matrix ProjMatrix;
    matrix ViewProj;
    matrix ViewInv;
};

cbuffer TransformBuffer
{
    matrix WorldMatrix;
};

// ���� ���� ���� �ű� ��

float3 CameraPosition()
{
    return ViewInv._41_42_43;
}
// 

#endif