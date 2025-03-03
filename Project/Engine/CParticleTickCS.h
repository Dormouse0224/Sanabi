#pragma once
#include "CComputeShader.h"

#include "CTexture.h"

class CParticleTickCS :
    public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_SpawnBuffer;
    CStructuredBuffer* m_ModuleBuffer;
    AssetPtr<CTexture>       m_NoiseTex;

public:
    void SetParticleBuffer(CStructuredBuffer* _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnBuffer(CStructuredBuffer* _Buffer) { m_SpawnBuffer = _Buffer; }
    void SetModuleBuffer(CStructuredBuffer* _Buffer) { m_ModuleBuffer = _Buffer; }
    void SetNoiseTex(AssetPtr<CTexture> _Noise) { m_NoiseTex = _Noise; }

public:
    virtual int Binding() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

public:
    CParticleTickCS();
    ~CParticleTickCS();
};

