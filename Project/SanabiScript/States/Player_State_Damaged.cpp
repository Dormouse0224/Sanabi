#include "pch.h"
#include "Player_State_Damaged.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CPhysxActor.h"

#include "Scripts/PlayerScript.h"

Player_State_Damaged::Player_State_Damaged()
	: CFSM_State()
	, m_PS(nullptr)
	, IsLeft(0)
{
}

Player_State_Damaged::Player_State_Damaged(const Player_State_Damaged& _Other)
	: CFSM_State(_Other)
	, m_PS(nullptr)
	, IsLeft(0)
{
}

Player_State_Damaged::~Player_State_Damaged()
{
}

void Player_State_Damaged::Tick()
{
	// �¿� ���� ����
	if (IsLeft != 0)
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 180, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, 1);
	}
	else
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 0, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, -1);
	}

	// �ִϸ��̼� ���� üũ
	if (m_Owner->GetOwner()->FlipbookRender()->IsFinish())
		SetConst<int>(2, 1);
}

void Player_State_Damaged::Begin()
{
	// �˹�
	Vec3 moveVel(0.0f);
	IsLeft = 0;
	GetConst<int>(0, &IsLeft);
	IsLeft != 0 ? moveVel.x += 100.f : moveVel.x -= 100.f;
	moveVel.y = 200.f;
	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(moveVel.x, moveVel.y, 0.f));

	// ���� ����
	m_PS = (PlayerScript*)m_Owner->GetOwner()->FindScript("class PlayerScript");
	m_PS->SetImmune(true);
	m_PS->SetDamaged(false);

	// �ִϸ��̼�
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Damaged.flip", 5, false);
	m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Damaged.flip", 5, false);
}

void Player_State_Damaged::End()
{
	// ���� ����
	m_PS->SetImmune(false);
	SetConst<int>(2, 0);
}
