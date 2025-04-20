#pragma once
#include "Engine\CFSM_State.h"

class MonsterScript;
class PlayerScript;

class Player_State_Holding :
    public CFSM_State
{
public:
    Player_State_Holding();
    Player_State_Holding(const Player_State_Holding& _Other);
    ~Player_State_Holding();
    CLONE(Player_State_Holding);

private:
    Vec3 m_DestPos;
    float m_Distance;
    MonsterScript* m_MS;
    PlayerScript* m_PS;



public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

