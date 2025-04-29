#ifndef _STD2D
#define _STD2D

#include "value.fx"

#define TEXTURE         g_tex_0
#define NORM_TEXTURE    g_Ntex_0

// Vertex Shader
struct VS_IN
{
    float3 vPos : POSITION; // Semantic, InputLayout
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    // System Value Semantic
    // SV_Position : 레스터라이져에 NDC 좌표를 전달할 때 사용하는 Semantic
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    float4 vWorldPos : POSITION;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
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

PS_OUT PS_Std2D(VS_OUT _in)
{
    PS_OUT _Out;
    
    _Out.RT_Color = TEXTURE.Sample(g_sam_1, _in.vUV);
    _Out.RT_Normal = NORM_TEXTURE.Sample(g_sam_1, _in.vUV);
    _Out.RT_WorldPos = _in.vWorldPos;
          
    if (_Out.RT_Color.a == 0.f)
        discard;
    
    // 그레이 스케일
    if (g_int_0)
    {
        _Out.RT_Color.rgb = (_Out.RT_Color.r + _Out.RT_Color.g + _Out.RT_Color.b) / 3.f;
    }
    
    return _Out;
}


PS_OUT PS_Std2D_AlphaBlend(VS_OUT _in)
{
    PS_OUT _Out;
    
    _Out.RT_Color = TEXTURE.Sample(g_sam_1, _in.vUV);
    _Out.RT_Normal = NORM_TEXTURE.Sample(g_sam_1, _in.vUV);
    _Out.RT_WorldPos = _in.vWorldPos;
    
    _Out.RT_Color.a = _Out.RT_Color.a * g_float_0; // 알파값 보정치 (그라데이션용)
    
    return _Out;
}

#endif