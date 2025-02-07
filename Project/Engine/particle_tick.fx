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
    
    // 스레드가 담당한 파티클이 비활성화 상태라면
    if (false == g_Buffer[_ID.x].Active)
    {
        while (g_Spawn[0].SpawnCount)
        {
            int SpawnCount = g_Spawn[0].SpawnCount;
            int Input = SpawnCount - 1;
            int Origin = 0;
        
            // Atomic 함수, 스레드가 하나씩 순차적으로 실행
            InterlockedCompareExchange(g_Spawn[0].SpawnCount, SpawnCount, Input, Origin);
            //InterlockedExchange(g_Spawn[0].SpawnCount, Input, Origin);
        
            // 최초 진입한 스레드만 Origin 값을 1을 가져온다.
            if (Origin == SpawnCount)
            {
                g_Buffer[_ID.x].Active = true;
                break;
            }
        }
        
        // 활성화된 파티클 초기화 작업
        if (g_Buffer[_ID.x].Active)
        {
            float NormalizedThreadID = (float) _ID.x / (float) MAX_PARTICLE;
            ParticleInit(g_Buffer[_ID.x], g_Module[0], g_tex_0, g_btex_0, NormalizedThreadID);
        }
    }
    else
    {
        // 파티클의 수명이 다하면 비활성화 시킨다.
        g_Buffer[_ID.x].Age += g_EngineDT;
        if (g_Buffer[_ID.x].Life < g_Buffer[_ID.x].Age)
        {
            g_Buffer[_ID.x].Active = false;
            return;
        }
        
        
    }
}

#endif