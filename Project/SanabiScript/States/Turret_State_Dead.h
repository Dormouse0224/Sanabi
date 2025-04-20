#pragma once
#include "Engine\CFSM_State.h"

class Turret_State_Dead :
    public CFSM_State
{
public:
    Turret_State_Dead();
    Turret_State_Dead(const Turret_State_Dead& _Other);
    ~Turret_State_Dead();
    CLONE(Turret_State_Dead);

private:


public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

