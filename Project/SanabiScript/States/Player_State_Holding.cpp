#include "pch.h"
#include "Player_State_Holding.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CKeyMgr.h"
#include "Engine/CPhysxActor.h"

#include "Scripts/MonsterScript.h"
#include "Scripts/PlayerScript.h"


Player_State_Holding::Player_State_Holding()
	: CFSM_State()
{
}

Player_State_Holding::Player_State_Holding(const Player_State_Holding& _Other)
	: CFSM_State(_Other)
{
}

Player_State_Holding::~Player_State_Holding()
{
}

void Player_State_Holding::Tick()
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

	// 위치 고정을 위해 지속 갱신
	m_Owner->GetOwner()->Transform()->SetRelativePos(m_DestPos);
	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0));

	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::RELEASE)
	{
		m_MS->SetDead(true);
	}
}

void Player_State_Holding::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_ExcHolding_Front.flip", 10, false);
	m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_ExcHolding_Front.flip", 10, false);

	// 위치 고정
	m_Owner->GetOwner()->Transform()->SetRelativePos(m_DestPos);
	m_Owner->GetOwner()->PhysxActor()->SetGravity(false);
	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0));

	Vec4 data;
	GetConst<Vec4>(0, &data);
	m_DestPos = Vec3(data.x, data.y, data.z);
	m_Distance = data.w;

	DWORD_PTR pTarget;
	if (GetConst<DWORD_PTR>(0, &pTarget))
	{
		m_MS = (MonsterScript*)((CGameObject*)pTarget)->FindScript("class MonsterScript");
		m_MS->SetExcHolded(true);
	}

	m_PS = (PlayerScript*)m_Owner->GetOwner()->FindScript("class PlayerScript");
	m_PS->SetImmune(true);
}

void Player_State_Holding::End()
{
	m_PS->SetImmune(false);

	m_Owner->GetOwner()->PhysxActor()->SetGravity(true);
	m_MS->SetExcHolded(false);
}
