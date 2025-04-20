#pragma once
#include "Engine\CFSM_State.h"

class Turret_State_Sleep :
    public CFSM_State
{
public:
    Turret_State_Sleep();
    Turret_State_Sleep(const Turret_State_Sleep& _Other);
    ~Turret_State_Sleep();
    CLONE(Turret_State_Sleep);

private:


public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

