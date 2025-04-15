#pragma once
#include "Engine\CFSM_State.h"

class Player_State_Idle :
    public CFSM_State
{
public:
    Player_State_Idle();
    ~Player_State_Idle();
    CLONE(Player_State_Idle);
private:
    bool m_bIdleflip;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

