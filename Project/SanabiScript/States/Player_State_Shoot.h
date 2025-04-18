#pragma once
#include "Engine\CFSM_State.h"

class Player_State_Shoot :
    public CFSM_State
{
public:
    Player_State_Shoot();
    Player_State_Shoot(const Player_State_Shoot& _Other);
    ~Player_State_Shoot();
    CLONE(Player_State_Shoot);
private:
    CGameObject* m_Grab;
    Vec2 m_CursorPos;
    float m_Timer;
    float m_MaxLen;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
    void HitCheck();
};

