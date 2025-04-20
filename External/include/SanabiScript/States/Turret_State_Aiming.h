#pragma once
#include "Engine\CFSM_State.h"

class Turret_State_Aiming :
    public CFSM_State
{
public:
    Turret_State_Aiming();
    Turret_State_Aiming(const Turret_State_Aiming& _Other);
    ~Turret_State_Aiming();
    CLONE(Turret_State_Aiming);

private:
    float m_Timer;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

