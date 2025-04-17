#ifndef _UI
#define _UI

#include "value.fx"

#define ScaleX g_float_1
#define ScaleY g_float_2


// Vertex Shader
struct VS_IN
{
    float3 vPos : POSITION; // Semantic, InputLayout
    float2 vUV : TEXCOORD;
};
struct VS_OUT
{
    // System Value Semantic
    // SV_Position : 레스터라이져에 NDC 좌표를 전달할 때 사용하는 Semantic
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_UI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = float4(_in.vPos.xyz, 1.f);
    output.vUV = _in.vUV;
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

[maxvertexcount(6)]
void GS_UI(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _Stream)
{
    float4 vCenter = _in[0].vPosition;
        
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    float3 vViewPos = mul(vCenter, g_matWV).xyz;
    
    // 빌보드 효과를 구현하기 위해 View 변환 적용 후 정점 생성. (빌보드 미적용 시 World 변환 이후 정점을 생성하면 됨)
    output[0].vPosition.xyz = float3(vViewPos.x - ScaleX / 2.f, vViewPos.y + ScaleY / 2.f, vViewPos.z);
    output[1].vPosition.xyz = float3(vViewPos.x + ScaleX / 2.f, vViewPos.y + ScaleY / 2.f, vViewPos.z);
    output[2].vPosition.xyz = float3(vViewPos.x + ScaleX / 2.f, vViewPos.y - ScaleY / 2.f, vViewPos.z);
    output[3].vPosition.xyz = float3(vViewPos.x - ScaleX / 2.f, vViewPos.y - ScaleY / 2.f, vViewPos.z);
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(float4(output[i].vPosition.xyz, 1.f), g_matProj);
    }
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
        
    _Stream.Append(output[0]);
    _Stream.Append(output[1]);
    _Stream.Append(output[2]);
    _Stream.RestartStrip();

    _Stream.Append(output[0]);
    _Stream.Append(output[2]);
    _Stream.Append(output[3]);
    _Stream.RestartStrip();
    
}

float4 PS_UI(GS_OUT _in) : SV_Target
{
    float4 vSampleColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    vSampleColor.a = vSampleColor.a * g_float_0; // 알파값 보정치 (그라데이션용)
    
    return vSampleColor;
}

#endif