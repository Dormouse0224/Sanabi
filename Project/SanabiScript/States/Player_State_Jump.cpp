#include "pch.h"
#include "Player_State_Jump.h"

#include "Engine/CKeyMgr.h"
#include "Engine/CTimeMgr.h"

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
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Falling.flip", 10, true);
	}

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

	PxVec3 CurrentVel = m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity();
	if ((i == 0 && CurrentVel.x < 0) || (i != 0 && CurrentVel.x > 0))
		SetConst<int>(0, (i == 0));

	// 좌우 이동 조작
	Vec3 moveVel(0.0f);
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::PRESSED) { moveVel.x -= 300.f * DT; }
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::PRESSED) { moveVel.x += 300.f * DT; }

	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(CurrentVel.x + moveVel.x, CurrentVel.y + moveVel.y, 0.f));
}

void Player_State_Jump::Begin()
{
	if (m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity().y > 0)
	{
		// 위쪽으로 점프중인 경우
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Jumping.flip", 10, true);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Jumping.flip", 10, true);
	}
	else
	{
		// 낙하중인 경우
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_FallStart.flip", 10, false);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_FallStart.flip", 10, false);
		m_bFallStart = true;
	}
}

void Player_State_Jump::End()
{
	m_bFalling = false;
	m_bFallStart = false;
}
