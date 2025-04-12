#pragma once
#include "Engine\CFSM_State.h"

class MainTitle_Bg_State_Loop :
    public CFSM_State
{
public:
    MainTitle_Bg_State_Loop();
    ~MainTitle_Bg_State_Loop();
    CLONE(MainTitle_Bg_State_Loop);

private:

public:
    virtual void Tick();
    virtual void Begin();
    virtual void End();
};