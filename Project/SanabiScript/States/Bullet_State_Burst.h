#pragma once
#include "Engine\CFSM_State.h"

class Bullet_State_Burst :
    public CFSM_State
{
public:
    Bullet_State_Burst();
    Bullet_State_Burst(const Bullet_State_Burst& _Other);
    ~Bullet_State_Burst();
    CLONE(Bullet_State_Burst);

private:
    bool m_DestroySelf;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

