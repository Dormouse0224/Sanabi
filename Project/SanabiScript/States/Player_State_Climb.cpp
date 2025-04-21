#include "pch.h"
#include "Player_State_Climb.h"

#include "Engine/CKeyMgr.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CTimeMgr.h"
#include "Scripts/PlayerScript.h"

Player_State_Climb::Player_State_Climb()
	: CFSM_State()
	, m_bFlipPlay(false)
{
}

Player_State_Climb::~Player_State_Climb()
{
}

void Player_State_Climb::Tick()
{
	//  상하 이동 애니메이션 전환
	if ((CKeyMgr::GetInst()->GetKeyState(Keyboard::S) == Key_state::PRESSED) && !m_bFlipPlay)
	{
		m_bFlipPlay = true;
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_WallClimbDown.flip", 10, true);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_WallClimbDown.flip", 10, true);
	}
	else if ((CKeyMgr::GetInst()->GetKeyState(Keyboard::W) == Key_state::PRESSED) && !m_bFlipPlay)
	{
		m_bFlipPlay = true;
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_WallClimbUp.flip", 10, true);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_WallClimbUp.flip", 10, true);
	}
	else if ((CKeyMgr::GetInst()->GetKeyState(Keyboard::W) == Key_state::NONE) && (CKeyMgr::GetInst()->GetKeyState(Keyboard::S) == Key_state::NONE) && m_bFlipPlay)
	{
		m_bFlipPlay = false;
		m_Owner->GetOwner()->FlipbookRender()->Pause(true);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Pause(true);
	}

	// 좌우 방향 설정
	PlayerScript* pScript = static_cast<PlayerScript*>(m_Owner->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetWallLeft())
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 180, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, 1);
	}
	else
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 0, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, -1);
	}

	// 상하 이동 조작
	Vec3 moveVel(0.0f);
	PxVec3 CurrentVel = m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity();
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::W) == Key_state::PRESSED) { moveVel.y += 100.f; }
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::S) == Key_state::PRESSED) { moveVel.y -= 300.f; }

	// 점프 조작
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::SPACE) == Key_state::TAP)
	{
		moveVel.y += 300.f;
		pScript->GetWallLeft() ? moveVel.x += 100.f : moveVel.x -= 100.f;
		m_bJumpOut = true;
	}

	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(moveVel.x, moveVel.y, 0.f));
}

void Player_State_Climb::Begin()
{
	// 중력 제거
	m_Owner->GetOwner()->PhysxActor()->SetGravity(false);
	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0.f, 0.f, 0.f));
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_WallClimbUp.flip", 10, true);
	m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_WallClimbUp.flip", 10, true);

	m_bFlipPlay = false;
	m_Owner->GetOwner()->FlipbookRender()->Pause(true);
	m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Pause(true);
}

void Player_State_Climb::End()
{
	// 중력 적용
	m_Owner->GetOwner()->PhysxActor()->SetGravity(true);
	if (m_bJumpOut)
	{
		Vec3 moveVel(0.0f);
		PlayerScript* pScript = static_cast<PlayerScript*>(m_Owner->GetOwner()->FindScript("class PlayerScript"));
		moveVel.y += 500.f;
		pScript->GetWallLeft() ? moveVel.x += 100.f : moveVel.x -= 100.f;
		m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(moveVel.x, moveVel.y, 0.f));
	}
	m_bJumpOut = false;
}
