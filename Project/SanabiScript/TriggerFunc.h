#pragma once

#include "Engine/CFSM_State.h"

// Main Title
bool MainTitle_Bg_Start_Loop(CFSM_State* _Origin, CFSM_State* _Dest);

// Player
bool Player_Idle_Run(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Idle_Jump(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Idle_Shoot(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Idle_Damaged(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Run_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Run_Jump(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Run_Shoot(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Run_Damaged(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Jump_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Jump_Climb(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Jump_Shoot(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Jump_Damaged(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Climb_Jump(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Climb_Damaged(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Shoot_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Shoot_Swing(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Shoot_ExcWinding(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Shoot_Damaged(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Swing_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Swing_Damaged(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Damaged_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Damaged_Death(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_ExcWinding_Holding(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Holding_Damaged(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Holding_Dash(CFSM_State* _Origin, CFSM_State* _Dest);

bool Player_Dash_Idle(CFSM_State* _Origin, CFSM_State* _Dest);
bool Player_Dash_Shoot(CFSM_State* _Origin, CFSM_State* _Dest);

// BattleGate
bool BattleGate_Open_Close(CFSM_State* _Origin, CFSM_State* _Dest);
bool BattleGate_Close_Open(CFSM_State* _Origin, CFSM_State* _Dest);

// Turret
bool Turret_Sleep_Popup(CFSM_State* _Origin, CFSM_State* _Dest);

bool Turret_Popup_Aiming(CFSM_State* _Origin, CFSM_State* _Dest);

bool Turret_Aiming_Shooting(CFSM_State* _Origin, CFSM_State* _Dest);
bool Turret_Aiming_ExcHolded(CFSM_State* _Origin, CFSM_State* _Dest);

bool Turret_Shooting_Aiming(CFSM_State* _Origin, CFSM_State* _Dest);
bool Turret_Shooting_ExcHolded(CFSM_State* _Origin, CFSM_State* _Dest);

bool Turret_ExcHolded_Aiming(CFSM_State* _Origin, CFSM_State* _Dest);
bool Turret_ExcHolded_Dead(CFSM_State* _Origin, CFSM_State* _Dest);

// Bullet
bool Bullet_Idle_Burst(CFSM_State* _Origin, CFSM_State* _Dest);