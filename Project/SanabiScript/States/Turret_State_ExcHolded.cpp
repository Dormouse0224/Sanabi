#include "pch.h"
#include "Turret_State_ExcHolded.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"

Turret_State_ExcHolded::Turret_State_ExcHolded()
	: CFSM_State()
{
}

Turret_State_ExcHolded::Turret_State_ExcHolded(const Turret_State_ExcHolded& _Other)
	: CFSM_State(_Other)
{
}

Turret_State_ExcHolded::~Turret_State_ExcHolded()
{
}

void Turret_State_ExcHolded::Tick()
{
}

void Turret_State_ExcHolded::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\TurretBody_ExcHolded.flip", 10, true);
	m_Owner->GetOwner()->FindChild(L"TurretGun")->FlipbookRender()->Stop();
}

void Turret_State_ExcHolded::End()
{
}
