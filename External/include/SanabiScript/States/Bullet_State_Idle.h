#pragma once
#include "Engine\CFSM_State.h"

class Bullet_State_Idle :
    public CFSM_State
{
public:
    Bullet_State_Idle();
    Bullet_State_Idle(const Bullet_State_Idle& _Other);
    ~Bullet_State_Idle();
    CLONE(Bullet_State_Idle);

private:
    bool m_DestroySelf;
    float m_Timer;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

