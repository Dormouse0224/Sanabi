#include "pch.h"
#include "Turret_State_Popup.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CPhysxActor.h"

Turret_State_Popup::Turret_State_Popup()
	: CFSM_State()
{
}

Turret_State_Popup::Turret_State_Popup(const Turret_State_Popup& _Other)
	: CFSM_State(_Other)
{
}

Turret_State_Popup::~Turret_State_Popup()
{
}

void Turret_State_Popup::Tick()
{
	if (m_Owner->GetOwner()->FlipbookRender()->IsFinish())
		SetConst<int>(0, 1);
}

void Turret_State_Popup::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\TurretBody_PopUp.flip", 10, false);
	COLLIDER_DESC desc;
	desc.ShapeFlag = PxShapeFlag::eSCENE_QUERY_SHAPE;
	desc.FilterLayer_Self = COLLISION_LAYER::eMONSTER;
	desc.FilterLayer_Other = COLLISION_LAYER::ePLAYER;
	m_Owner->GetOwner()->PhysxActor()->AddCollider(desc, PxVec3(20, 20, 20), PxVec3(0));
}

void Turret_State_Popup::End()
{
	SetConst<int>(0, 0);
}
