#pragma once
#include "CRenderComponent.h"
#include "CParticleTickCS.h"

class CStructuredBuffer;

class CParticleRender :
    public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_SpawnBuffer;


    Ptr<CParticleTickCS>    m_TickCS;
    UINT                    m_MaxParticle;      // 파티클 최대 개수

    tParticleModule         m_Module;           // 파티클 제어 설정값
    float                   m_AccTime;          // 누적시간 체크


public:
    // 초당 파티클 생성량 설정
    void SetSpawnRate(float _Rate) { m_Module.SpawnRate = _Rate; }
    float GetSpawnRate() { return m_Module.SpawnRate; }

public:
    virtual void FinalTick() override;
    virtual void Render() override;

private:
    void CreateMtrl();
    void CalcSpawnCount();


public:
    CLONE(CParticleRender);
    CParticleRender();
    ~CParticleRender();
};

