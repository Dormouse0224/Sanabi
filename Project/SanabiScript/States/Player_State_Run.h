#pragma once
#include "Engine\CFSM_State.h"

class Player_State_Run :
    public CFSM_State
{
public:
    Player_State_Run();
    ~Player_State_Run();
    CLONE(Player_State_Run);
private:

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

