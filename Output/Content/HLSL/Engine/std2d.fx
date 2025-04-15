#ifndef _STD2D
#define _STD2D

#include "value.fx"

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
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vSampleColor = g_tex_0.Sample(g_sam_1, _in.vUV);
          
    if (vSampleColor.a == 0.f)
        discard;
    
    // 그레이 스케일
    if (g_int_0)
    {
        vSampleColor.rgb = (vSampleColor.r + vSampleColor.g + vSampleColor.b) / 3.f;
    }
    
    return vSampleColor;
}


float4 PS_Std2D_AlphaBlend(VS_OUT _in) : SV_Target
{
    float4 vSampleColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    vSampleColor.a = vSampleColor.a * g_float_0;    // 알파값 보정치 (그라데이션용)
    
    return vSampleColor;
}

#endif