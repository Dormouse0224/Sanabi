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

	Vec3 currentPose = m_Owner->GetOwner()->Transform()->GetRelativePos();

	// 예: 이동 입력 처리 (간단한 예시)
	Vec3 moveDir(0.0f);
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::PRESSED) { moveDir.x -= 1.0f; }
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::PRESSED) { moveDir.x += 1.0f; }

	if (moveDir.Normalize().Length() > 0.0f)
	{
		float speed = 50.0f;
		Vec3 newPos = currentPose + moveDir * speed * DT;
		m_Owner->GetOwner()->Transform()->SetRelativePos(newPos);
	}
}

void Player_State_Run::Begin()
{
	// Idle 플립북 재생
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Running.flip", 10, true);
}

void Player_State_Run::End()
{
}
