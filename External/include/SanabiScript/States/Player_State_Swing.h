#pragma once
#include "Engine\CFSM_State.h"

class Player_State_Swing :
    public CFSM_State
{
public:
    Player_State_Swing();
    Player_State_Swing(const Player_State_Swing& _Other);
    ~Player_State_Swing();
    CLONE(Player_State_Swing);
private:
    CGameObject* m_Anchor;
    PxDistanceJoint* m_Joint;
    Vec3 m_GrabPos;
    float m_GrabDist;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

