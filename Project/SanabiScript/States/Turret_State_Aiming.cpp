#include "pch.h"
#include "Turret_State_Aiming.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CTransform.h"
#include "Engine/CTimeMgr.h"

Turret_State_Aiming::Turret_State_Aiming()
	: CFSM_State()
{
}

Turret_State_Aiming::Turret_State_Aiming(const Turret_State_Aiming& _Other)
	: CFSM_State(_Other)
{
}

Turret_State_Aiming::~Turret_State_Aiming()
{
}

void Turret_State_Aiming::Tick()
{
	CGameObject* pPlayer = CLevelMgr::GetInst()->FindObject(LAYER::Player, L"Player");
	if (pPlayer)
	{
		// 플레이어 방향으로 터렛 총 부분 회전 적용
		Vec3 Dir = pPlayer->Transform()->GetWorldPos() - m_Owner->GetOwner()->Transform()->GetWorldPos();
		Dir.Normalize();
		Vec3 axis = XMVector3Cross(Vec3(0, 1, 0), Dir);
		float angle = acosf(XMVectorGetX(XMVector3Dot(Vec3(0, 1, 0), Dir)));
		Vec4 quat = XMQuaternionRotationAxis(axis.Normalize(), angle);
		m_Owner->GetOwner()->FindChild(L"TurretGun")->Transform()->SetRelativeRot(quat);
	}

	m_Timer += DT;
	if (m_Timer > 2.f)
		SetConst<int>(0, 1);
}

void Turret_State_Aiming::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\TurretBody_Aiming.flip", 10, true);
	m_Owner->GetOwner()->FindChild(L"TurretGun")->FlipbookRender()->Play(L"Flipbook\\TurretGun_Aiming.flip", 10, true);
}

void Turret_State_Aiming::End()
{
	m_Timer = 0;
	SetConst<int>(0, 0);
}
