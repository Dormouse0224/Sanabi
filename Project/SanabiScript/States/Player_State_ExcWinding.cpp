#include "pch.h"
#include "Player_State_ExcWinding.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/CTransform.h"
#include "Engine/CPhysxActor.h"
#include "Engine/CAssetMgr.h"
#include "Engine/CTimeMgr.h"

#include "Scripts/PlayerScript.h"

Player_State_ExcWinding::Player_State_ExcWinding()
	: CFSM_State()
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

Player_State_ExcWinding::Player_State_ExcWinding(const Player_State_ExcWinding& _Other)
	: CFSM_State(_Other)
{
}

Player_State_ExcWinding::~Player_State_ExcWinding()
{
	if (!m_Grab->GetParent())
		delete m_Grab;
}

void Player_State_ExcWinding::Tick()
{
	// �¿� ���� ����
	int LookLeft = 0;
	if (GetConst<int>(0, &LookLeft) && LookLeft != 0)
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 180, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, 1);
	}
	else
	{
		m_Owner->GetOwner()->Transform()->SetRelativeRot(0, 0, 0);
		m_Owner->GetOwner()->FindChild(L"Player_Arm")->Transform()->SetRelativePos(0, 0, -1);
	}

	m_Owner->GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(0));

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

	// ���� �߻�
	m_Timer += DT;
	float roundTime = 0.2f;
	if (m_Timer < roundTime)
	{
		Vec3 dir = m_DestPos - m_Owner->GetOwner()->Transform()->GetWorldTrans();
		float dist = dir.Length();
		dir.Normalize();
		if (LookLeft != 0)
			dir = Vec3(-dir.x, dir.y, dir.z);
		m_Grab->Transform()->SetRelativePos(dir * (dist * m_Timer / roundTime));
	}
	else
		SetConst<int>(2, 1);
}

void Player_State_ExcWinding::Begin()
{
	m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\SNB_ExcWinding.flip", 10, false);

	Vec4 data;
	GetConst<Vec4>(0, &data);
	m_DestPos = Vec3(data.x, data.y, data.z);
	m_Distance = data.w;

	m_PS = (PlayerScript*)m_Owner->GetOwner()->FindScript("class PlayerScript");
	m_PS->SetImmune(true);

	// ���� ��ü ���� �� �÷��̾ �ڽ����� �߰�
	m_Owner->GetOwner()->AddChild(m_Grab);
}

void Player_State_ExcWinding::End()
{
	m_PS->SetImmune(false);

	// ���� ��ü �ڽĿ��� ����
	m_Owner->GetOwner()->DeleteChild(m_Grab);


	SetConst<int>(2, 0);
	m_Timer = 0;
}
