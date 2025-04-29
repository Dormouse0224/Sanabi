#ifndef _FLIPBOOK
#define _FLIPBOOK

#include "value.fx"

#define IS_ATLAS_TEX    g_int_0
#define IS_SINGLE_TEX   g_int_1

#define TEXTURE         g_tex_0
#define NORM_TEXTURE    g_Ntex_0

#define LeftTop         g_vec2_0
#define Slice           g_vec2_1

struct VS_IN
{
    float3 vPos : POSITION; // Semantic, InputLayout   
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float4 vWorldPos : POSITION;
};

VS_OUT VS_Flipbook(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    return output;
}

struct PS_OUT
{
    float4 RT_Color : SV_Target0;
    float4 RT_Normal : SV_Target1;
    float4 RT_WorldPos : SV_Target2;
};

PS_OUT PS_Flipbook(VS_OUT _in)
{
    PS_OUT _Out;
    
    _Out.RT_Color = float4(1.f, 0.f, 1.f, 1.f); // 기본 색상 마젠타로 초기화
    
    if (1 == IS_ATLAS_TEX)
    {
        float2 vUV = LeftTop + (Slice * _in.vUV);
        
        if (vUV.x < LeftTop.x || (LeftTop.x + Slice.x) < vUV.x
            || vUV.y < LeftTop.y || (LeftTop.y + Slice.y) < vUV.y)
        {
            discard;
        }
        
        _Out.RT_Color = TEXTURE.Sample(g_sam_1, vUV);
        _Out.RT_Normal = NORM_TEXTURE.Sample(g_sam_1, vUV);
        _Out.RT_WorldPos = _in.vWorldPos;

    }
    else if (1 == IS_SINGLE_TEX)
    {
        float2 vUV = _in.vUV;
        _Out.RT_Color = TEXTURE.Sample(g_sam_1, vUV);
        _Out.RT_Normal = NORM_TEXTURE.Sample(g_sam_1, vUV);
        _Out.RT_WorldPos = _in.vWorldPos;
    }
    
    if (0.f == _Out.RT_Color.a)
        discard;
    
    return _Out;
}


#endif