#pragma once
#include "Engine\CFSM_State.h"

class MainTitle_Bg_State_Start :
    public CFSM_State
{
public:
    MainTitle_Bg_State_Start();
    ~MainTitle_Bg_State_Start();
    CLONE(MainTitle_Bg_State_Start);

private:

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};

