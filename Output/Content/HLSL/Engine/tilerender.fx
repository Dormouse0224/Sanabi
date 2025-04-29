#ifndef _TILERENDER
#define _TILERENDER

#include "value.fx"

struct TileInfo
{
    float2 vLeftTop;
    
    int padding[2];
};

StructuredBuffer<TileInfo> g_TileInfo : register(t20);

#define ATLAS_TEX       g_tex_0
#define NORM_TEXTURE    g_Ntex_0

#define TileSize   g_vec2_0
#define AtlasSize  g_vec2_1

#define MAX_COL   g_int_0
#define MAX_ROW   g_int_1


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

VS_OUT VS_TileRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    
    //_in.vPos.xy += float2(0.5f, -0.5f);
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV * float2(MAX_COL, MAX_ROW);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    return output;
}

struct PS_OUT
{
    float4 RT_Color : SV_Target0;
    float4 RT_Normal : SV_Target1;
    float4 RT_WorldPos : SV_Target2;
};


PS_OUT PS_TileRender(VS_OUT _in)
{
    PS_OUT _Out;
    
    _Out.RT_Color = float4(1.f, 0.f, 1.f, 1.f);
    _Out.RT_Normal = float4(0.f, 0.f, 1.f, 1.f);
    _Out.RT_WorldPos = float4(0.f, 0.f, 0.f, 1.f);
     
    int2 ColRow = (int2) floor(_in.vUV);
    float2 vSampleUV = frac(_in.vUV);
        
    int idx = ColRow.y * MAX_COL + ColRow.x;
    if (g_TileInfo[idx].vLeftTop.x == -1.f)
    {
        _Out.RT_Color = float4(0.f, 0.f, 0.f, 0.f);
    }
    else
    {
        _Out.RT_Color = ATLAS_TEX.Sample(g_sam_1, (g_TileInfo[idx].vLeftTop + (TileSize * vSampleUV)) / AtlasSize);
        _Out.RT_Normal = NORM_TEXTURE.Sample(g_sam_1, (g_TileInfo[idx].vLeftTop + (TileSize * vSampleUV)) / AtlasSize);
        _Out.RT_WorldPos = _in.vWorldPos;
    }
    
    //if (0.f == _Out.RT_Color.a)
    //    discard;
    
    return _Out;
}


#endif