#pragma once
#include "CRenderComponent.h"
#include "CParticleTickCS.h"

class CStructuredBuffer;

class CParticleRender :
    public CRenderComponent
{
private:
    CStructuredBuffer*      m_ParticleBuffer;
    CStructuredBuffer*      m_SpawnBuffer;
    CStructuredBuffer*      m_ModuleBuffer;

    Ptr<CParticleTickCS>    m_TickCS;
    UINT                    m_MaxParticle;      // 파티클 최대 개수

    tParticleModule         m_Module;           // 파티클 제어 설정값
    bool                    m_ModuleChanged;    // 모듈 데이터 변경점 여부
    float                   m_AccTime;          // 누적시간 체크
    int                     m_SpawnCount;
    bool                    m_Active;
    bool                    m_Gravity;

    Ptr<CTexture>           m_ParticleTex;

    map<UINT, CGameObject*> m_mapParticleObj;

public:
    void SetSpawnRate(float _Rate) { m_Module.SpawnRate = _Rate; m_ModuleChanged = true; }
    void SetSpawnCount(float _Count) { m_SpawnCount = _Count; }
    void SetParticleTex(const Ptr<CTexture>& _Tex) { m_ParticleTex = _Tex; }
    float GetSpawnRate() { return m_Module.SpawnRate; m_ModuleChanged = true; }
    void SetStartColor(Vec4 _Color) { m_Module.StartColor = _Color; m_ModuleChanged = true; }
    void SetEndColor(Vec4 _Color) { m_Module.EndColor = _Color; m_ModuleChanged = true; }
    void SetMinScale(Vec3 _Scale) { m_Module.MinScale = _Scale; m_ModuleChanged = true; }
    void SetMaxScale(Vec3 _Scale) { m_Module.MaxScale = _Scale; m_ModuleChanged = true; }
    void SetMinSpeed(float _Speed) { m_Module.MinSpeed = _Speed; m_ModuleChanged = true; }
    void SetMaxSpeed(float _Speed) { m_Module.MaxSpeed = _Speed; m_ModuleChanged = true; }
    void SetMinLife(float _MinLife) { m_Module.MinLife = _MinLife; m_ModuleChanged = true; }
    void SetMaxLife(float _MaxLife) { m_Module.MaxLife = _MaxLife; m_ModuleChanged = true; }
    // 0 : Box, 1 : Sphere
    void SetSpawnShape(int _Shape) { m_Module.SpawnShape = _Shape; m_ModuleChanged = true; }
    void SetSpawnShapeScale(Vec3 _Scale) { m_Module.SpawnShapeScale = _Scale; m_ModuleChanged = true; }
    void SetSpawnDir(Vec3 _Dir) { m_Module.SpawnDir = _Dir; m_ModuleChanged = true; }
    void SetSpawnDirRandomize(float _Randomize) { m_Module.SpawnDirRandomize = _Randomize; m_ModuleChanged = true; }
    // 0 : Local, 1 : World;
    void SetSpaceType(int _Type) { m_Module.SpaceType = _Type; m_ModuleChanged = true; }
    void SetActiveState(bool _b) { m_Active = _b; }
    void SetGravityState(bool _b) { m_Gravity = _b; }





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

