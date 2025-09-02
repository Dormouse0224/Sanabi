#pragma once
#include "Engine\CFSM_State.h"

class Stage1_BGM :
    public CFSM_State
{
public:
    Stage1_BGM();
    Stage1_BGM(const Stage1_BGM& _Other);
    ~Stage1_BGM();
    CLONE(Stage1_BGM);

private:


public:
    virtual void Tick() override;
    virtual void Begin() override;
    virtual void End() override;
};