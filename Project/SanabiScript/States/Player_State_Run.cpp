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
	int i = 0;
	if (GetConst<int>(0, &i) && i != 0)
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 180, 0);
	else
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 0, 0);

	PxVec3 CurrentVel = m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity();
	if ((i == 0 && CurrentVel.x < 0) || (i != 0 && CurrentVel.x > 0))
		SetConst<int>(0, (i == 0));

	// 예: 이동 입력 처리 (간단한 예시)
	Vec3 moveVel(0.0f);
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::PRESSED) { moveVel.x -= 1.0f; }
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::PRESSED) { moveVel.x += 1.0f; }

	if (moveVel.Normalize().Length() > 0.0f)
	{
		float speed = 200.0f;
		moveVel = moveVel * speed;
		m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(moveVel.x, CurrentVel.y, 0.f));
	}
}

void Player_State_Run::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Running.flip", 10, true);
}

void Player_State_Run::End()
{
}
