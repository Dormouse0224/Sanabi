#pragma once
#include "Engine\CFSM_State.h"

class PlayerScript;

class Player_State_Dash :
    public CFSM_State
{
public:
    Player_State_Dash();
    Player_State_Dash(const Player_State_Dash& _Other);
    ~Player_State_Dash();
    CLONE(Player_State_Dash);

private:
    PlayerScript* m_PS;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

