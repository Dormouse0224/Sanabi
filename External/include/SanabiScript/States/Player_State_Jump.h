#pragma once
#include "Engine\CFSM_State.h"

class Player_State_Jump :
    public CFSM_State
{
public:
    Player_State_Jump();
    ~Player_State_Jump();
    CLONE(Player_State_Jump);
private:
    bool m_bIdleflip;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

