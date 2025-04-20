#include "pch.h"
#include "Player_State_Dash.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CKeyMgr.h"
#include "Engine/CRenderMgr.h"
#include "Engine/CPhysxActor.h"

#include "Scripts/PlayerScript.h"

Player_State_Dash::Player_State_Dash()
	: CFSM_State()
{
}

Player_State_Dash::Player_State_Dash(const Player_State_Dash& _Other)
	: CFSM_State(_Other)
{
}

Player_State_Dash::~Player_State_Dash()
{
}

void Player_State_Dash::Tick()
{
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

	if (m_Owner->GetOwner()->FlipbookRender()->IsFinish())
		SetConst<int>(2, 1);

}

void Player_State_Dash::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_ExcDash.flip", 10, false);
	m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_ExcDash.flip", 10, false);

	// 플레이어 기준 커서 방향 계산
	Vec2 vec = GetScreenFromWorld(m_Owner->GetOwner()->Transform()->GetWorldPos(), CRenderMgr::GetInst()->GetCurrentCam());;
	Vec3 origin(vec.x, vec.y, 0.f);
	vec = CKeyMgr::GetInst()->GetMousePos();
	Vec3 target(vec.x, vec.y, 0.f);
	Vec3 norm = target - origin;
	norm.Normalize();
	PxVec3 unit(norm.x, -norm.y, norm.z);

	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(unit * 500.f);

	m_PS = (PlayerScript*)m_Owner->GetOwner()->FindScript("class PlayerScript");
	m_PS->SetImmune(true);
}

void Player_State_Dash::End()
{
	m_PS->SetImmune(false);
	SetConst<int>(2, 0);
}
