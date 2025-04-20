#include "pch.h"
#include "Bullet_State_Idle.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTaskMgr.h"
#include "Engine/CTimeMgr.h"

Bullet_State_Idle::Bullet_State_Idle()
	: CFSM_State()
	, m_DestroySelf(false)
	, m_Timer(0.f)
{
}

Bullet_State_Idle::Bullet_State_Idle(const Bullet_State_Idle& _Other)
	: CFSM_State(_Other)
	, m_DestroySelf(false)
	, m_Timer(0.f)
{
}

Bullet_State_Idle::~Bullet_State_Idle()
{
}

void Bullet_State_Idle::Tick()
{
	m_Timer += DT;
	if (!m_DestroySelf && m_Timer > 5.f)
	{
		m_DestroySelf = true;
		CTaskMgr::GetInst()->AddTask(TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)m_Owner->GetOwner(), NULL);
	}
}

void Bullet_State_Idle::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\MajorBulletA.flip", 10, true);
}

void Bullet_State_Idle::End()
{
}
