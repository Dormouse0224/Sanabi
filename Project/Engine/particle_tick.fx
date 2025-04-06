#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "particle_func.fx"

RWStructuredBuffer<tParticle> g_Buffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_Spawn : register(u1);
StructuredBuffer<tParticleModule> g_Module : register(t20);

#define MAX_PARTICLE g_int_0



[numthreads(256, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (MAX_PARTICLE <= _ID.x)
        return;
    
    g_Buffer[_ID.x].PrevActive = g_Buffer[_ID.x].Active;
    
    // �����尡 ����� ��ƼŬ�� ��Ȱ��ȭ ���¶��
    if (false == g_Buffer[_ID.x].Active)
    {
        while (g_Spawn[0].SpawnCount)
        {
            int SpawnCount = g_Spawn[0].SpawnCount;
            int Input = SpawnCount - 1;
            int Origin = 0;
        
            // Atomic �Լ�, �����尡 �ϳ��� ���������� ����
            // dest �� compareval �� ��ġ�ϸ�, val�� dest�� �ְ� dest �� �ִ� ���� originval�� ����
            InterlockedCompareExchange(g_Spawn[0].SpawnCount, SpawnCount, Input, Origin);
            //InterlockedExchange(g_Spawn[0].SpawnCount, Input, Origin);
        
            // InterlockedCompareExchange ���� ������ ������� ���� dest ��(SpawnCount) �� originval �� �ް�, ������ ������� �״�� 0
            if (Origin == SpawnCount)
            {
                g_Buffer[_ID.x].Active = true;
                break;
            }
        }
        
        // Ȱ��ȭ�� ��ƼŬ �ʱ�ȭ �۾�
        if (g_Buffer[_ID.x].Active)
        {
            float NormalizedThreadID = (float) _ID.x / (float) MAX_PARTICLE;
            ParticleInit(g_Buffer[_ID.x], g_Module[0], g_tex_0, g_btex_0, NormalizedThreadID);
        }
    }
    else
    {
        // ��ƼŬ�� ������ ���ϸ� ��Ȱ��ȭ ��Ų��.
        g_Buffer[_ID.x].Age += g_DT;
        if (g_Buffer[_ID.x].Life < g_Buffer[_ID.x].Age)
        {
            g_Buffer[_ID.x].Active = false;
            return;
        }
        
        // Local Ÿ�� ��ƼŬ�� ��� ��ġ ���� ����
        if (g_Module[0].SpaceType == 0)
        {
            g_Buffer[_ID.x].LocalPos += g_Buffer[_ID.x].Velocity * g_DT;
        }
        
    }
}

#endif