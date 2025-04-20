#pragma once
#include "Engine\CFSM_State.h"

class Turret_State_ExcHolded :
    public CFSM_State
{
public:
    Turret_State_ExcHolded();
    Turret_State_ExcHolded(const Turret_State_ExcHolded& _Other);
    ~Turret_State_ExcHolded();
    CLONE(Turret_State_ExcHolded);

private:


public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

