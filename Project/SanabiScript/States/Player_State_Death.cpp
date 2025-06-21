#include "pch.h"
#include "Player_State_Death.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTimeMgr.h"
#include "Engine/CTaskMgr.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CRenderMgr.h"
#include "Engine/CCamera.h"

#include "Scripts/PlayerScript.h"

Player_State_Death::Player_State_Death()
	: CFSM_State()
{
}

Player_State_Death::Player_State_Death(const Player_State_Death& _Other)
	: CFSM_State(_Other)
{
}

Player_State_Death::~Player_State_Death()
{
}

void Player_State_Death::Tick()
{
	if (m_Owner->GetOwner()->FlipbookRender()->IsFinish())
		m_FlipFin = true;

	if (m_FlipFin)
		m_Timer += DT;

	if (m_Timer > 2.f && m_FlipFin)
	{
		m_FlipFin = false;
		
		CTaskMgr::GetInst()->AddTask(TASK_TYPE::CHANGE_LEVEL, (DWORD_PTR)CLevelMgr::GetInst()->GetPlayStartLevel(), (DWORD_PTR)true);
	}
}

void Player_State_Death::Begin()
{
	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0));
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Death.flip", 5, false);
	m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Stop();

	// ÁÜÀÎ
	CRenderMgr::GetInst()->GetCurrentCam()->SetZoom(2.f);
}

void Player_State_Death::End()
{
	m_FlipFin = false;
	m_Timer = 0.f;

	// ÁÜ¾Æ¿ô
	CRenderMgr::GetInst()->GetCurrentCam()->SetZoom(1.f);
}
