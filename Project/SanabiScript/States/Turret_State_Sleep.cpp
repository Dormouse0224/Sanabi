#include "pch.h"
#include "Turret_State_Sleep.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"

Turret_State_Sleep::Turret_State_Sleep()
	: CFSM_State()
{
}

Turret_State_Sleep::Turret_State_Sleep(const Turret_State_Sleep& _Other)
	: CFSM_State(_Other)
{
}

Turret_State_Sleep::~Turret_State_Sleep()
{
}

void Turret_State_Sleep::Tick()
{
}

void Turret_State_Sleep::Begin()
{
}

void Turret_State_Sleep::End()
{
}
