#pragma once
#include "Engine\CFSM_State.h"

class Turret_State_Shooting :
    public CFSM_State
{
public:
    Turret_State_Shooting();
    Turret_State_Shooting(const Turret_State_Shooting& _Other);
    ~Turret_State_Shooting();
    CLONE(Turret_State_Shooting);

private:
    float m_Timer;
    float m_Interval;
    int m_Count;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

