#include "pch.h"
#include "Turret_State_Dead.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"

Turret_State_Dead::Turret_State_Dead()
	: CFSM_State()
{
}

Turret_State_Dead::Turret_State_Dead(const Turret_State_Dead& _Other)
	: CFSM_State(_Other)
{
}

Turret_State_Dead::~Turret_State_Dead()
{
}

void Turret_State_Dead::Tick()
{
}

void Turret_State_Dead::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\TurretBody_Dead.flip", 10, true);
	m_Owner->GetOwner()->FindChild(L"TurretGun")->FlipbookRender()->Stop();
}

void Turret_State_Dead::End()
{
}
