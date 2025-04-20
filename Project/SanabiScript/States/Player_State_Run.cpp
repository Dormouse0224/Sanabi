#include "pch.h"
#include "Player_State_Run.h"

#include "Engine/CKeyMgr.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CTimeMgr.h"

Player_State_Run::Player_State_Run()
{
	SetConstVec<int>(vector<int>{0});
}

Player_State_Run::~Player_State_Run()
{
}

void Player_State_Run::Tick()
{
	// 좌우 방향 설정
	int LookLeft = 0;
	if (GetConst<int>(0, &LookLeft) && LookLeft != 0)
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
	if ((LookLeft == 0 && CurrentVel.x < 0) || (LookLeft != 0 && CurrentVel.x > 0))
		SetConst<int>(0, (LookLeft == 0));

	// 좌우 이동 조작
	Vec3 moveVel(0.0f);
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::PRESSED) { moveVel.x -= 200.f; }
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::PRESSED) { moveVel.x += 200.f; }

	// 점프 조작
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::SPACE) == Key_state::TAP) { moveVel.y += 400.f; }

	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(moveVel.x, CurrentVel.y + moveVel.y, 0.f));


}

void Player_State_Run::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Running.flip", 10, true);
	m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Running.flip", 10, true);
}

void Player_State_Run::End()
{
}
