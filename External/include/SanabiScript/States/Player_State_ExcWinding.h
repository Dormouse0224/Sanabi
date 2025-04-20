#pragma once
#include "Engine\CFSM_State.h"

class PlayerScript;
class CGameObject;

class Player_State_ExcWinding :
    public CFSM_State
{
public:
    Player_State_ExcWinding();
    Player_State_ExcWinding(const Player_State_ExcWinding& _Other);
    ~Player_State_ExcWinding();
    CLONE(Player_State_ExcWinding);

private:
    Vec3 m_DestPos;
    float m_Distance;
    PlayerScript* m_PS;
    CGameObject* m_Grab;
    float m_Timer;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

