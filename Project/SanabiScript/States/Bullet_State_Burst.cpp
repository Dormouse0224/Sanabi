#include "pch.h"
#include "Bullet_State_Burst.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CTaskMgr.h"

Bullet_State_Burst::Bullet_State_Burst()
	: CFSM_State()
	, m_DestroySelf(false)
{
}

Bullet_State_Burst::Bullet_State_Burst(const Bullet_State_Burst& _Other)
	: CFSM_State(_Other)
	, m_DestroySelf(false)
{
}

Bullet_State_Burst::~Bullet_State_Burst()
{
}

void Bullet_State_Burst::Tick()
{
	if (!m_DestroySelf && m_Owner->GetOwner()->FlipbookRender()->IsFinish())
	{
		m_DestroySelf = true;
		CTaskMgr::GetInst()->AddTask(TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)m_Owner->GetOwner(), NULL);
	}
}

void Bullet_State_Burst::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BulletVfx_Burst.flip", 10, false);
	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0));
}

void Bullet_State_Burst::End()
{
}
