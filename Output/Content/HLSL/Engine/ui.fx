#ifndef _UI
#define _UI

#include "value.fx"

#define TEXTURE     g_tex_0
#define ScaleX      g_float_1
#define ScaleY      g_float_2
#define NDC         g_vec2_0


// Vertex Shader
struct VS_IN
{
    float3 vPos : POSITION; // Semantic, InputLayout
};
struct VS_OUT
{
    float4 vPosition : POSITION;
};

VS_OUT VS_UI(VS_IN _in)
{
    // ���� ���̴��� �����ϴ� ������ ������� �����Ƿ� �ּ� ������ �����Ѵ�.
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xyz, 1.f);
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
    // ���� ���̴����� ���� ������ ������� �ʰ�, ���ε��� NDC ��ǥ�� �����ϰ��� �̿��� ������Ʈ�� ���̴����� �簢�� �޽��� ���� �����Ѵ�.
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    // ���ε��� UI�� NDC ��ǥ�� �������� NDC �������� ��ȯ�� ��, �簢�� �޽��� Ȯ��
    // �� ��ǥ�� WVP ��ȯ �� ���ٺ��ұ��� ����� ��ǥ�̹Ƿ�, near ��鿡 ��ġ�ϵ��� Z ���� 0���� �ϰ� �����Ͷ��������� ���ٺ����� �Ͼ�� �ʵ��� W ���� 1�� �����Ѵ�.
    output[0].vPosition = float4(NDC.x - ScaleX / g_Resolution.x, NDC.y + ScaleY / g_Resolution.y, 0, 1.f);
    output[1].vPosition = float4(NDC.x + ScaleX / g_Resolution.x, NDC.y + ScaleY / g_Resolution.y, 0, 1.f);
    output[2].vPosition = float4(NDC.x + ScaleX / g_Resolution.x, NDC.y - ScaleY / g_Resolution.y, 0, 1.f);
    output[3].vPosition = float4(NDC.x - ScaleX / g_Resolution.x, NDC.y - ScaleY / g_Resolution.y, 0, 1.f);
    
    
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
    float4 vSampleColor = TEXTURE.Sample(g_sam_1, _in.vUV);
    vSampleColor.a = vSampleColor.a * g_float_0; // ���İ� ����ġ (�׶��̼ǿ�)
    
    return vSampleColor;
}

#endif