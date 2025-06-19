#include "pch.h"
#include "Player_State_Shoot.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CKeyMgr.h"
#include "Engine/CPhysxMgr.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CAssetMgr.h"
#include "Engine/CTimeMgr.h"
#include "Engine/CRenderMgr.h"
#include "Engine/CCamera.h"

#include "Scripts/PlayerScript.h"

Player_State_Shoot::Player_State_Shoot()
	: CFSM_State()
	, m_Grab(nullptr)
	, m_CursorPos(0.f, 0.f)
	, m_Timer(0.f)
	, m_MaxLen(500)
{
	// ���� ��ü ����
	m_Grab = new CGameObject;
	m_Grab->SetName(L"Player_Grab");
	m_Grab->AddComponent(new CTransform);
	m_Grab->AddComponent(new CFlipbookRender);
	m_Grab->Transform()->SetRelativeScale(100, 100, 1);
	m_Grab->Transform()->SetIndependentScale(true);
	m_Grab->FlipbookRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	m_Grab->FlipbookRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"EA_FlipbookMtrl"));
	m_Grab->FlipbookRender()->AddFlipbook(CAssetMgr::GetInst()->Load<CFlipbook>(L"Flipbook\\SNBGrab_Flying.flip"));
	m_Grab->FlipbookRender()->Play(L"Flipbook\\SNBGrab_Flying.flip", 0, false);
}


Player_State_Shoot::Player_State_Shoot(const Player_State_Shoot& _Other)
	: CFSM_State(_Other)
	, m_Grab(_Other.m_Grab->Clone())
	, m_CursorPos(0.f, 0.f)
	, m_Timer(0.f)
	, m_MaxLen(1000.f)
{
}


Player_State_Shoot::~Player_State_Shoot()
{
	if (!m_Grab->GetParent())
		delete m_Grab;
}

void Player_State_Shoot::Tick()
{
	// �¿� ���� ����
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

	// ���� ����������� ȸ�� ����
	Vec3 Dir = m_Grab->Transform()->GetRelativePos();
	Dir.Normalize();
	if (Dir.Length() > 0)
	{
		Vec3 axis = XMVector3Cross(Vec3(0, 1, 0), Dir);
		float angle = acosf(XMVectorGetX(XMVector3Dot(Vec3(0, 1, 0), Dir)));
		Vec4 quat = XMQuaternionRotationAxis(axis.Normalize(), angle);
		m_Grab->Transform()->SetRelativeRot(quat);
	}

	// ���� ���ٰ� ���ƿ���
	m_Timer += DT;
	float roundTime = 0.3f;
	if (m_Timer < roundTime)
	{
		Vec2 screen = GetScreenFromWorld(m_Owner->GetOwner()->Transform()->GetWorldTrans(), CRenderMgr::GetInst()->GetCurrentCam());
		Vec2 norm = m_CursorPos - screen;
		norm.Normalize();
		float len = m_MaxLen - abs((m_MaxLen * 2 / roundTime) * m_Timer - m_MaxLen);
		Vec2 dir(norm.x * len * ((0.5f - !!i) * 2.f), -norm.y * len);
		m_Grab->Transform()->SetRelativePos(dir.x, dir.y, m_Owner->GetOwner()->Transform()->GetWorldTrans().z);
	}
	else
		SetConst<int>(2, 1);
}

void Player_State_Shoot::Begin()
{
	PlayerScript* pPlayer = static_cast<PlayerScript*>(m_Owner->GetOwner()->FindScript("class PlayerScript"));
	if (pPlayer->GetAirborne())
	{
		// ����
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_ShotAIRFront.flip", 10, false);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Shoot.flip", 10, false);
	}
	else
	{
		// ����
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_ShotGNDFront.flip", 10, false);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->FlipbookRender()->Play(L"Flipbook\\SNBArm_Shoot.flip", 10, false);
		m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0.f, 0.f, 0.f));
	}

	// ����ĳ�������� ��Ʈ üũ
	HitCheck();

	// ���� ��ü ���� �� �÷��̾ �ڽ����� �߰�
	m_Owner->GetOwner()->AddChild(m_Grab);

	m_CursorPos = CKeyMgr::GetInst()->GetMousePos();
}

void Player_State_Shoot::End()
{
	// ���� ��ü �ڽĿ��� ����
	m_Owner->GetOwner()->DeleteChild(m_Grab);
	
	// ��� ������, Ÿ�̸� �ʱ�ȭ
	SetConst<int>(1, 0);	// �浹 ��� ���� (0 : �浹X, 1 : Ÿ�� �浹, 2 : �� �浹)
	SetConst<int>(2, 0);	// ���� ����
	m_Timer = 0.f;
}

void Player_State_Shoot::HitCheck()
{
	Vec3 v3 = m_Owner->GetOwner()->Transform()->GetWorldTrans();
	PxVec3 shooter(v3.x, v3.y, v3.z);
	Vec2 vec = GetScreenFromWorld(m_Owner->GetOwner()->Transform()->GetWorldTrans(), CRenderMgr::GetInst()->GetCurrentCam());;
	Vec3 origin(vec.x, vec.y, 0.f);
	vec = CKeyMgr::GetInst()->GetMousePos();
	Vec3 target(vec.x, vec.y, 0.f);
	Vec3 norm = target - origin;
	norm.Normalize();
	PxVec3 unit(norm.x, -norm.y, norm.z);
	PxRaycastBuffer hitbuff;

	PxQueryFilterData filterData;
	filterData.flags = PxQueryFlag::eSTATIC;
	//filterData.data.word0 = COLLISION_LAYER::ePLAYER;
	//filterData.data.word1 = COLLISION_LAYER::eMONSTER | COLLISION_LAYER::eLANDSCAPE;

	bool check = CPhysxMgr::GetInst()->GetScene()->raycast(shooter, unit, m_MaxLen, hitbuff, PxHitFlag::eDEFAULT, filterData);
	SetConst<int>(1, 0);
	if (check && hitbuff.hasBlock)
	{
		CGameObject* pTarget = CPhysxMgr::GetInst()->FindBodyOwner(hitbuff.block.actor);
		if (pTarget->GetLayerIdx() == static_cast<int>(LAYER::Tile))
		{
			SetConst<int>(1, 1);
			SetConst<DWORD_PTR>(0, (DWORD_PTR)pTarget);
		}
		else if (pTarget->GetLayerIdx() == static_cast<int>(LAYER::Enemy))
		{
			SetConst<int>(1, 2);
			SetConst<DWORD_PTR>(0, (DWORD_PTR)pTarget);
		}
		SetConst<Vec4>(0, Vec4(hitbuff.block.position.x, hitbuff.block.position.y, hitbuff.block.position.z, hitbuff.block.distance));
	}
}
