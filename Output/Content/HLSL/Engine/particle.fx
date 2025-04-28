#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"

#define OBJECT_POS g_vec4_0

StructuredBuffer<tParticle> m_Buffer : register(t20);
StructuredBuffer<tParticleModule> m_Module : register(t21);

struct VS_IN
{
    uint InstID : SV_InstanceID;
};


struct VS_OUT
{
    float InstID : FOG;
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.InstID = _in.InstID;
    return output;
}

struct GS_OUT
{
    float4 vColor : COLOR;
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// 1. ������ �����ϱ�
// 2. ��Ȱ��ȭ �� ��ƼŬ�� �ǳʶٱ�
[maxvertexcount(6)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _Stream)
{
    uint InstID = (uint) _in[0].InstID;
    if (false == m_Buffer[InstID].Active)
        return;
        
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    float3 vWorldPos = m_Buffer[InstID].LocalPos;
    
    // Local �������� ��쿡��
    if (0 == m_Module[0].SpaceType)
    {
        vWorldPos += m_Module[0].ObjectWorldPos;
    }
       
    float3 vViewPos = mul(float4(vWorldPos, 1.f), g_matView).xyz;
    
    float3 vScale = m_Buffer[InstID].WorldScale.xyz;
    
    // Viewspace ���� ��ƼŬ ��ġ(point mesh)�� rect mesh�� Ȯ��
    // ������ ȿ���� �����ϱ� ���� View ��ȯ ���� �� ���� ����. (������ ������ �� World ��ȯ ���� ������ �����ϸ� ��) - Viewspace Scale
    output[0].vPosition.xyz = float3(-vScale.x / 2.f, vScale.y / 2.f, 0);
    output[1].vPosition.xyz = float3(vScale.x / 2.f, vScale.y / 2.f, 0);
    output[2].vPosition.xyz = float3(vScale.x / 2.f, -vScale.y / 2.f, 0);
    output[3].vPosition.xyz = float3(-vScale.x / 2.f, -vScale.y / 2.f, 0);
    
    // ����������� ���� (������ ��ȿȭ��) - Viewspace Rotation
    if (m_Module[0].VelocityAllignment != 0)
    {
        float3 vU = normalize(mul(float4(m_Buffer[InstID].Velocity.xyz, 0), g_matView).xyz);
        float3 vR = normalize(cross(vU, float3(0, 0, 1)));
        float3 vF = normalize(cross(vR, vU));
        
        float3x3 ViewRotMat = { vR, vU, vF };

        for (int i = 0; i < 4; ++i)
        {
            output[i].vPosition.xyz = mul(output[i].vPosition.xyz, ViewRotMat);
        }
    }
    
    // - Viewspace Transform
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos;
    }
    
    
        for (int i = 0; i < 4; ++i)
        {
            output[i].vPosition = mul(float4(output[i].vPosition.xyz, 1.f), g_matProj);
        }
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    float4 Color = (m_Module[0].EndColor - m_Module[0].StartColor) * (m_Buffer[InstID].Age / m_Buffer[InstID].Life) + m_Module[0].StartColor;
    output[0].vColor = Color;
    output[1].vColor = Color;
    output[2].vColor = Color;
    output[3].vColor = Color;
        
    _Stream.Append(output[0]);
    _Stream.Append(output[1]);
    _Stream.Append(output[2]);
    _Stream.RestartStrip();

    _Stream.Append(output[0]);
    _Stream.Append(output[2]);
    _Stream.Append(output[3]);
    _Stream.RestartStrip();
    
}

float4 PS_Particle(GS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    vColor *= _in.vColor;
    return vColor;
}

#endif