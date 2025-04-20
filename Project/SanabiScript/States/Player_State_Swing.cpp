#include "pch.h"
#include "Player_State_Swing.h"

#include "Engine/CGameObject.h"
#include "Engine/CTransform.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CAssetMgr.h"
#include "Engine/CFSM.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CKeyMgr.h"
#include "Engine/CTimeMgr.h"

Player_State_Swing::Player_State_Swing()
	: CFSM_State()
	, m_Anchor(nullptr)
	, m_Joint(nullptr)
	, m_GrabPos()
	, m_GrabDist()
{
	// 갈고리 객체 생성
	m_Anchor = new CGameObject;
	m_Anchor->SetName(L"Player_Anchor");
	m_Anchor->AddComponent(new CTransform);
	m_Anchor->AddComponent(new CPhysxActor);
	m_Anchor->AddComponent(new CFlipbookRender);
	m_Anchor->Transform()->SetRelativeScale(100, 100, 1);
	m_Anchor->Transform()->SetIndependentScale(true);
	m_Anchor->Transform()->SetIndependentTrans(true);
	m_Anchor->FlipbookRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	m_Anchor->FlipbookRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"EA_FlipbookMtrl"));
	m_Anchor->FlipbookRender()->AddFlipbook(CAssetMgr::GetInst()->Load<CFlipbook>(L"Flipbook\\SNBGrab_Grabed.flip"));
	m_Anchor->FlipbookRender()->Play(L"Flipbook\\SNBGrab_Grabed.flip", 0, false);
}

Player_State_Swing::Player_State_Swing(const Player_State_Swing& _Other)
	: CFSM_State(_Other)
	, m_Anchor(_Other.m_Anchor->Clone())
	, m_Joint(nullptr)
	, m_GrabPos()
	, m_GrabDist()
{
}

Player_State_Swing::~Player_State_Swing()
{
	if (!m_Anchor->GetParent())
		delete m_Anchor;

	if (m_Joint)
	{
		m_Joint->release();
		m_Joint = nullptr;
	}
}

void Player_State_Swing::Tick()
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

	PxVec3 CurrentVel = m_Owner->GetOwner()->PhysxActor()->GetLinearVelocity();
	if ((i == 0 && CurrentVel.x < 0) || (i != 0 && CurrentVel.x > 0))
		SetConst<int>(0, (i == 0));

	// 좌우 이동 조작
	Vec3 moveVel(0.0f);
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::PRESSED) { moveVel.x -= 300.f * DT; }
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::PRESSED) { moveVel.x += 300.f * DT; }

	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(CurrentVel.x + moveVel.x, CurrentVel.y + moveVel.y, 0.f));

	if (float dist = m_Joint->getMaxDistance(); dist > 200.f)
		m_Joint->setMaxDistance(dist - 200.f * DT);
	
}

void Player_State_Swing::Begin()
{
	Vec4 grabinfo;
	GetConst<Vec4>(0, &grabinfo);
	m_GrabPos = Vec3(grabinfo.x, grabinfo.y, grabinfo.z);
	m_GrabDist = max(grabinfo.w, 50.f);

	if (grabinfo.w > 200.f)
	{
		Vec3 dir = m_GrabPos - m_Owner->GetOwner()->Transform()->GetWorldPos();
		dir.Normalize();
		m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(dir.x, 0.f, 0.f) * 500.f);
	}

	// 그랩 지점에 고정점 생성, 플레이어와 PxDistanceJoint 로 연결
	m_Anchor->Transform()->SetRelativePos(m_GrabPos);
	m_Anchor->PhysxActor()->SetRigidBody(RIGID_TYPE::STATIC);
	m_Owner->GetOwner()->AddChild(m_Anchor);

	m_Joint = PxDistanceJointCreate(*CPhysxMgr::GetInst()->GetPhysics(), m_Anchor->PhysxActor()->GetRigidBody(), PxTransform(PxVec3(0))
		, m_Owner->GetOwner()->PhysxActor()->GetRigidBody(), PxTransform(PxVec3(0)));
	m_Joint->setMinDistance(10.f);
	m_Joint->setMaxDistance(m_GrabDist);
	m_Joint->setDistanceJointFlag(PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, true);
	m_Joint->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);

	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_Swing.flip", 10, false);
}

void Player_State_Swing::End()
{
	m_Owner->GetOwner()->DeleteChild(m_Anchor);
	CPhysxMgr::GetInst()->RemoveRigidBody(m_Anchor);

	m_Joint->release();
	m_Joint = nullptr;
}
