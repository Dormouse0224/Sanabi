#pragma once
#include "Engine\CFSM_State.h"

class PlayerScript;

class Player_State_Damaged :
    public CFSM_State
{
public:
    Player_State_Damaged();
    Player_State_Damaged(const Player_State_Damaged& _Other);
    ~Player_State_Damaged();
    CLONE(Player_State_Damaged);

private:
    PlayerScript* m_PS;
    int IsLeft;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

