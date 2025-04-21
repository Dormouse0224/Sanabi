#include "pch.h"
#include "BattleGate_State_Close.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CTransform.h"

#include "Scripts/BattleGateScript.h"

BattleGate_State_Close::BattleGate_State_Close()
	: CFSM_State()
	, m_Closing(false)
	, m_Gate(nullptr)
	, m_Collider(nullptr)
{
}

BattleGate_State_Close::BattleGate_State_Close(const BattleGate_State_Close& _Other)
	: CFSM_State(_Other)
	, m_Closing(false)
	, m_Gate(nullptr)
	, m_Collider(nullptr)
{
}

BattleGate_State_Close::~BattleGate_State_Close()
{
}

void BattleGate_State_Close::Tick()
{
	// 게이트 개방 애니메이션이 끝났는지 검사
	if (m_Closing && m_Owner->GetOwner()->FlipbookRender()->IsFinish())
	{
		m_Closing = false;
		if (m_Gate->GetVert())
		{
			m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateVert_Closed.flip", 10, true);
		}
		else
		{
			m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateHoriz_Closed.flip", 10, true);
		}
	}
}

void BattleGate_State_Close::Begin()
{
	m_Gate = (BattleGateScript*)m_Owner->GetOwner()->FindScript("class BattleGateScript");
	m_Closing = true;
	if (m_Gate->GetVert())
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateVert_Closing.flip", 10, false);
	}
	else
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateHoriz_Closing.flip", 10, false);
	}

	// 게이트에 충돌체 생성
	COLLIDER_DESC desc;
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
	desc.FilterLayer_Self = COLLISION_LAYER::eLANDSCAPE;
	desc.FilterLayer_Other = COLLISION_LAYER::ePLAYER | COLLISION_LAYER::eMONSTER | COLLISION_LAYER::ePARTICLE;
	Vec3 scale = m_Owner->GetOwner()->Transform()->GetRelativeScale() / 2.f;
	m_Collider = m_Owner->GetOwner()->PhysxActor()->AddCollider(desc, PxVec3(scale.x, scale.y, 10), PxVec3(0));
}

void BattleGate_State_Close::End()
{
	m_Owner->GetOwner()->PhysxActor()->DeleteCollider(m_Collider);
	m_Collider = nullptr;
}
