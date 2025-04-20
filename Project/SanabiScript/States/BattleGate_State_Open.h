#pragma once
#include "Engine\CFSM_State.h"

class BattleGateScript;

class BattleGate_State_Open :
    public CFSM_State
{
public:
    BattleGate_State_Open();
    BattleGate_State_Open(const BattleGate_State_Open& _Other);
    ~BattleGate_State_Open();
    CLONE(BattleGate_State_Open);
private:
    bool m_Opening;
    BattleGateScript* m_Gate;

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

