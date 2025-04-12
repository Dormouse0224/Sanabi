#pragma once
#include "CComponent.h"

#include "CComponentMgr.h"

class CScript :
    public CComponent
{
public:
    CScript();
    CScript(const CScript& _Origin);
    ~CScript();
    virtual CScript* Clone() = 0;

public:
    virtual void Tick() = 0;
    virtual void FinalTick() final {}
};

