#pragma once
#include "Engine\CFSM_State.h"

class Turret_State_Popup :
    public CFSM_State
{
public:
    Turret_State_Popup();
    Turret_State_Popup(const Turret_State_Popup& _Other);
    ~Turret_State_Popup();
    CLONE(Turret_State_Popup);

private:


public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

