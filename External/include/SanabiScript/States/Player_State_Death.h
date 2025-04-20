#pragma once
#include "Engine\CFSM_State.h"

class Player_State_Death :
    public CFSM_State
{
public:
    Player_State_Death();
    Player_State_Death(const Player_State_Death& _Other);
    ~Player_State_Death();
    CLONE(Player_State_Death);

private:
    bool m_FlipFin;
    float m_Timer;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();

};

