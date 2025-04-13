#ifndef _TILERENDER
#define _TILERENDER

#include "value.fx"

struct TileInfo
{
    float2 vLeftTop;
    
    int padding[2];
};

StructuredBuffer<TileInfo> g_TileInfo : register(t20);

#define ATLAS_TEX g_tex_0

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
};

VS_OUT VS_TileRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    
    //_in.vPos.xy += float2(0.5f, -0.5f);
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV * float2(MAX_COL, MAX_ROW);
    
    return output;
}


float4 PS_TileRender(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
     
    int2 ColRow = (int2) floor(_in.vUV);
    float2 vSampleUV = frac(_in.vUV);
        
    int idx = ColRow.y * MAX_COL + ColRow.x;
    if (g_TileInfo[idx].vLeftTop.x == -1.f)
        vColor = float4(0.f, 0.f, 0.f, 0.f);
    else
        vColor = ATLAS_TEX.Sample(g_sam_1, (g_TileInfo[idx].vLeftTop + (TileSize * vSampleUV)) / AtlasSize);
    
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}


#endif