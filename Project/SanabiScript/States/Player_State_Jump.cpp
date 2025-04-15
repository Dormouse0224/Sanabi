#include "pch.h"
#include "Player_State_Jump.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"

Player_State_Jump::Player_State_Jump()
{
}

Player_State_Jump::~Player_State_Jump()
{
}

void Player_State_Jump::Tick()
{
	if (m_bIdleflip)
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 180, 0);
}

void Player_State_Jump::Begin()
{
	// Idle ÇÃ¸³ºÏ Àç»ý
	m_Owner->GetOwner()->FlipbookRender()->Play(0, 10, true);
}

void Player_State_Jump::End()
{
}
