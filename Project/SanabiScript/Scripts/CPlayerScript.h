#pragma once
#include "Engine\CScript.h"

class CPlayerScript :
    public CScript
{
public:
    CPlayerScript();
    ~CPlayerScript();
    CLONE(CPlayerScript);
private:
    bool    m_Idle;
    bool    m_Run;

public:
    virtual void Tick() override;
    virtual void BeginOverlap(CGameObject* _Other) override;
    virtual void Overlap(CGameObject* _Other) override;
    virtual void EndOverlap(CGameObject* _Other) override;

public:
    virtual int Load(fstream& _Stream) override { return S_OK; }
    virtual int Save(fstream& _Stream) override { return S_OK; }

protected:
    static CScript* Instantiate();
};

