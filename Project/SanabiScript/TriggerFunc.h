#pragma once

#include "Engine/CFSM_State.h"

bool MainTitle_Bg_Start_Loop(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Idle_Run(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Run_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
