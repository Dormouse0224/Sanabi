#ifndef _FLIPBOOK
#define _FLIPBOOK

#include "value.fx"

#define IS_ATLAS_TEX    g_int_0
#define IS_SINGLE_TEX   g_int_1

#define TEXTURE         g_tex_0

#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Background      g_vec2_2
#define Offset          g_vec2_3

struct VS_IN
{
    float3 vPos : POSITION; // Semantic, InputLayout   
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Flipbook(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Flipbook(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (1 == IS_ATLAS_TEX)
    {
        float2 vUV = LeftTop + (Slice - Background) * 0.5f + (Background * _in.vUV) - Offset;
        
        if (vUV.x < LeftTop.x || (LeftTop.x + Slice.x) < vUV.x
            || vUV.y < LeftTop.y || (LeftTop.y + Slice.y) < vUV.y)
        {
            discard;
        }
        
        vColor = TEXTURE.Sample(g_sam_1, vUV);
    }
    else if (1 == IS_SINGLE_TEX)
    {
        float2 vUV = _in.vUV;
        vColor = TEXTURE.Sample(g_sam_1, vUV);
    }
    
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}


#endif