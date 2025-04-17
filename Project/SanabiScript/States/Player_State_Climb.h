#pragma once
#include "Engine\CFSM_State.h"

class Player_State_Climb :
    public CFSM_State
{
public:
    Player_State_Climb();
    ~Player_State_Climb();
    CLONE(Player_State_Climb);
private:
    bool m_bFlipPlay;
    bool m_bJumpOut;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

