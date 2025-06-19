#include "pch.h"
#include "Turret_State_Shooting.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTimeMgr.h"
#include "Engine/CAssetMgr.h"
#include "Engine/CTransform.h"
#include "Engine/CTaskMgr.h"
#include "Engine/CLevelMgr.h"

#include "Scripts/BulletScript.h"

Turret_State_Shooting::Turret_State_Shooting()
	: CFSM_State()
	, m_Timer(0.f)
	, m_Interval(0.2f)
	, m_Count(0)
{
}

Turret_State_Shooting::Turret_State_Shooting(const Turret_State_Shooting& _Other)
	: CFSM_State(_Other)
	, m_Timer(0.f)
	, m_Interval(0.2f)
	, m_Count(0)
{
}

Turret_State_Shooting::~Turret_State_Shooting()
{
}

void Turret_State_Shooting::Tick()
{
	CGameObject* pPlayer = CLevelMgr::GetInst()->FindObject(LAYER::Player, L"Player");
	if (pPlayer)
	{
		// 플레이어 방향으로 터렛 총 부분 회전 적용
		Vec3 Dir = pPlayer->Transform()->GetWorldTrans() - m_Owner->GetOwner()->Transform()->GetWorldTrans();
		Dir.Normalize();
		Vec3 TurretDir = m_Owner->GetOwner()->Transform()->GetWorldDir(DIR::UP);
		Vec3 axis = XMVector3Cross(TurretDir, Dir);
		float angle = acosf(XMVectorGetX(XMVector3Dot(TurretDir, Dir)));
		Vec4 quat = XMQuaternionRotationAxis(axis.Normalize(), angle);
		m_Owner->GetOwner()->FindChild(L"TurretGun")->Transform()->SetRelativeRot(quat);
	}
	
	// 일정 간격마다 총알 프리펩으로 객체 생성
	m_Timer += DT;
	if (m_Timer > m_Interval)
	{
		++m_Count;
		m_Timer -= m_Interval;
		AssetPtr<CPrefab> bulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"Prefab\\Bullet.prefab");
		CGameObject* pBullet = bulletPref->Instantiate();
		pBullet->Transform()->SetRelativePos(m_Owner->GetOwner()->Transform()->GetWorldTrans());
		pBullet->Transform()->SetRelativeRot(m_Owner->GetOwner()->FindChild(L"TurretGun")->Transform()->GetRelativeRotEuler());
		BulletScript* pBS = (BulletScript*)pBullet->FindScript("class BulletScript");
		pBS->SetDamage(1);
		Vec3 nDir = m_Owner->GetOwner()->FindChild(L"TurretGun")->Transform()->GetWorldDir(DIR::UP);
		nDir.Normalize();
		pBS->SetVelocity(nDir * 200.f);
		CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pBullet, NULL);
	}

	if (m_Count >= 2)
		SetConst<int>(0, 1);
}

void Turret_State_Shooting::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\TurretBody_Shooting.flip", 10, true);
	m_Owner->GetOwner()->FindChild(L"TurretGun")->FlipbookRender()->Play(L"Flipbook\\TurretGun_Shooting.flip", 10, true);
}

void Turret_State_Shooting::End()
{
	m_Timer = 0;
	m_Count = 0;
	SetConst<int>(0, 0);
}
