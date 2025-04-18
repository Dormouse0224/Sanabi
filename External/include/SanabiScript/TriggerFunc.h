#pragma once

#include "Engine/CFSM_State.h"

bool MainTitle_Bg_Start_Loop(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Idle_Run(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Idle_Jump(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Idle_Shoot(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Run_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Run_Jump(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Run_Shoot(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Jump_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Jump_Climb(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Jump_Shoot(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Climb_Jump(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Shoot_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Shoot_Swing(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Swing_Idle(CFSM_State* _Origin, CFSM_State* _Dest);