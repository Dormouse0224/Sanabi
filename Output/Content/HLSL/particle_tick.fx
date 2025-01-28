#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"

RWStructuredBuffer<tParticle> g_Buffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_Spawn : register(u1);



#define MAX_PARTICLE g_int_0

[numthreads(256, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (MAX_PARTICLE <= _ID.x)
        return;
    
    // 스레드가 담당한 파티클이 비활성화 상태라면
    if (!g_Buffer[_ID.x].Active)
    {
        int Input = g_Spawn[0].SpawnCount - 1;
        int Origin = 0;
        
        // Atomic 함수, 스레드가 하나씩 순차적으로 실행
        InterlockedExchange(g_Spawn[0].SpawnCount, Input, Origin);
        
        // 최초 진입한 스레드만 Origin 값을 1을 가져온다.
        if (Origin == 1)
        {
            g_Buffer[_ID.x].Active = true;
        }
    }
    
    //g_Buffer[_ID.x].WorldPos += g_Buffer[_ID.x].Velocity * g_DT;
}

#endif