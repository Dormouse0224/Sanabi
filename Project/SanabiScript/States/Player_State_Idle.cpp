#include "pch.h"
#include "Player_State_Idle.h"

#include "Engine/CKeyMgr.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CPhysxActor.h"

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
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Idle.flip", 10, false);
		m_bIdleflip = true;
	}

	PxVec3 CurrentVel = m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity();

	// 점프 조작
	Vec3 moveVel(0.0f);
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::SPACE) == Key_state::TAP) { moveVel.y += 400.f; }

	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0, CurrentVel.y + moveVel.y, 0.f));

	// 좌우 방향 설정
	int i = 0;
	if (GetConst<int>(0, &i) && i != 0)
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 180, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, 1);
	}
	else
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 0, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, -1);
	}
}

void Player_State_Idle::Begin()
{
	if (m_Owner->GetPrevState() != nullptr && m_Owner->GetPrevState()->GetName() == L"class Player_State_Run")
	{
		// 달리다가 멈춘경우
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_RunStop.flip", 10, false);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_RunStop.flip", 10, false);
		m_bIdleflip = false;
	}
	else
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Idle.flip", 10, true);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Idle.flip", 10, false);
		m_bIdleflip = true;
	}
}

void Player_State_Idle::End()
{
}
