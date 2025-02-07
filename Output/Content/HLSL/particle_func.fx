#ifndef _PARTICLE_FUNC
#define _PARTICLE_FUNC

#include "value.fx"


#define PI 3.1415926535f

float3 GetRandom(float _NomalizedThreadID, in Texture2D _NoiseTex)
{
    float2 vUV = (float2) 0.f;
        
    vUV.x = _NomalizedThreadID + g_AccTime * 0.1f;
    vUV.y = 0.5f + sin(vUV.x * 100.f * PI) * 0.5f;
        
    float3 vRandom = _NoiseTex.SampleLevel(g_sam_0, vUV, 0);
    return vRandom;
}


void ParticleInit(inout tParticle _Particle, in tParticleModule _Module
                , in Texture2D _NoiseTex, int _IsNoiseBind, float _NomalizedThreadID)
{
    // NoiseTexture �� ������ �������� ���� 0 ���� ����.    
      
    
    // ��ƼŬ ���� ��ġ ����    
    // Box
    if (0 == _Module.SpawnShape)
    {
        float3 vRandom = GetRandom(_NomalizedThreadID, _NoiseTex);
        
        // ���� ������ -0.5 ~ 0.5f �� ���� ��, �������� ũ�⸦ ���ؼ� ��ġ���� ���Ѵ�.
        float3 vSpanwPos = (vRandom - 0.5f) * _Module.SpawnShapeScale;
        _Particle.LocalPos.xyz = vSpanwPos;
        
        // �� ��ǥ�� ���� ������ �������͸� ���� �� �ӷ��� ���Ͽ� ���� �ӵ� ���.
        float3 vSpawnVel = float3(sin(vRandom.y * PI) * cos(vRandom.x * 4.f * PI), sin(vRandom.y * PI) * sin(vRandom.x * 4.f * PI), cos(vRandom.y * PI));
        float vSpeed = vRandom.z * (_Module.MaxSpeed - _Module.MinSpeed) + _Module.MinSpeed;
        _Particle.Velocity.xyz = vSpawnVel * 100.f;

    }
    
    // Sphere
    else if (1 == _Module.SpawnShape)
    {
        
    }
  
    // World ������
    if (1 == _Module.SpaceType)
    {
        _Particle.LocalPos.xyz += _Module.ObjectWorldPos;
    }
        
    // ��ƼŬ Life ���� ����
    _Particle.Age = 0.f;
    float3 vRandom = GetRandom(_NomalizedThreadID + 0.1f, _NoiseTex);
    _Particle.Life = clamp(vRandom.r, _Module.MinLife, _Module.MaxLife);
        
    // ��ƼŬ �ʱ� �� ����
    _Particle.Color = _Module.StartColor;
    
    // ��ƼŬ �ʱ� ũ�� ����
    _Particle.WorldScale.xyz = float3(10.f, 10.f, 10.f);
}

#endif