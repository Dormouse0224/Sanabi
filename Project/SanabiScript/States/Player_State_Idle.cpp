#include "pch.h"
#include "Player_State_Idle.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"

Player_State_Idle::Player_State_Idle()
	: CFSM_State()
	, m_bIdleflip(false)
{
	SetConstVec<int>(vector<int>{0});
}

Player_State_Idle::~Player_State_Idle()
{
}

void Player_State_Idle::Tick()
{
	if (!m_bIdleflip && m_Owner->GetOwner()->FlipbookRender()->IsFinish())
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Idle.flip", 10, true);
		m_bIdleflip = true;
	}


	int i = 0;
	if (GetConst<int>(0, &i) && i != 0)
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 180, 0);
}

void Player_State_Idle::Begin()
{
	if (m_Owner->GetPrevState() != nullptr && m_Owner->GetPrevState()->GetName() == L"class Player_State_Run")
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_RunStop.flip", 10, true);
		m_bIdleflip = false;
	}
	else
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Idle.flip", 10, true);
		m_bIdleflip = true;
	}
}

void Player_State_Idle::End()
{
}
