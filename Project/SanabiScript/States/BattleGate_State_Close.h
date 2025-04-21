#pragma once
#include "Engine\CFSM_State.h"

class BattleGateScript;

class BattleGate_State_Close :
    public CFSM_State
{
public:
    BattleGate_State_Close();
    BattleGate_State_Close(const BattleGate_State_Close& _Other);
    ~BattleGate_State_Close();
    CLONE(BattleGate_State_Close);

private:
    bool m_Closing;
    BattleGateScript* m_Gate;
    PxShape* m_Collider;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

