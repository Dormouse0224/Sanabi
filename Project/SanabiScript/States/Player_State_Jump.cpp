#include "pch.h"
#include "Player_State_Jump.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CPhysxActor.h"

Player_State_Jump::Player_State_Jump()
	: CFSM_State()
	, m_bFalling(false)
	, m_bFallStart(false)
{
}

Player_State_Jump::~Player_State_Jump()
{
}

void Player_State_Jump::Tick()
{
	if ((!m_bFalling && m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity().y < 0) || (m_bFallStart && !m_bFalling))
	{
		m_bFallStart = false;
		m_bFalling = true;
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Falling.flip", 10, true);
	}
}

void Player_State_Jump::Begin()
{
	if (m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity().y > 0)
	{
		// 위쪽으로 점프중인 경우
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Jumping.flip", 10, true);
	}
	else
	{
		// 낙하중인 경우
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_FallStart.flip", 10, false);
		m_bFallStart = true;
	}
}

void Player_State_Jump::End()
{
}
