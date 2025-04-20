#include "pch.h"
#include "PlayerScript.h"

#include "Engine/CPhysxActor.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"

PlayerScript::PlayerScript()
	: CScript()
	, m_bInit(false)
	, m_bAirborne(false)
	, m_bWallContact(false)
	, m_bWallLeft(false)
	, m_bWallContactCheck(false)
	, m_Damaged(false)
	, m_Immune(false)
	, m_HP(4)
	//, m_SavePoint(nullptr)
{
}

PlayerScript::PlayerScript(const PlayerScript& _Origin)
	: CScript(_Origin)
	, m_bInit(false)
	, m_bAirborne(false)
	, m_bWallContact(false)
	, m_bWallLeft(false)
	, m_Immune(false)
	, m_HP(4)
	//, m_SavePoint(nullptr)
{
	//if (_Origin.m_SavePoint)
	//	m_SavePoint = _Origin.m_SavePoint->Clone();
}

PlayerScript::~PlayerScript()
{
	//if (CLevelMgr::GetInst()->GetCurrentLevel() == nullptr || CLevelMgr::GetInst()->GetCurrentLevel() != m_SavePoint)
	//	delete m_SavePoint;
}

void PlayerScript::Tick()
{
	if (!m_bInit && CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
		Init();

	m_bAirborne = m_bAirborneCheck;
	m_bAirborneCheck = true;
	m_bWallContact = m_bWallContactCheck;
	m_bWallContactCheck = false;

}

void PlayerScript::Init()
{
	m_bInit = true;

	// 충돌 이벤트 설정
	GetOwner()->PhysxActor()->SetContactBegin(this, (PxContactFunc)(&PlayerScript::ContactBegin));
	GetOwner()->PhysxActor()->SetContactTick(this, (PxContactFunc)(&PlayerScript::ContacTick));
	GetOwner()->PhysxActor()->SetContactEnd(this, (PxContactFunc)(&PlayerScript::ContactEnd));

	//// 시작 위치 세이브포인트로 등록
	//SetSavePoint(CLevelMgr::GetInst()->GetCurrentLevel()->Clone());
}

void PlayerScript::ContactBegin(CGameObject* _Other, const PxContactPair pair)
{
	int a = 0;
}

void PlayerScript::ContacTick(CGameObject* _Other, const PxContactPair pair)
{
	if (_Other->GetLayerIdx() == static_cast<int>(LAYER::Tile))
	{
		vector<PxContactPairPoint> vecPoint;
		vecPoint.resize(pair.contactCount);
		pair.extractContacts(vecPoint.data(), vecPoint.size());
		for (auto& point : vecPoint)
		{
			if (point.normal.y > 0.7f)
				m_bAirborneCheck = false;
			else if (point.normal.y < 0.3f && point.normal.y > -0.3f)
			{
				m_bWallContactCheck = true;
				if (point.normal.x > 0.7f)
					m_bWallLeft = true;
				else if (point.normal.x < -0.7f)
					m_bWallLeft = false;
			}
		}
	}
}

void PlayerScript::ContactEnd(CGameObject* _Other, const PxContactPair pair)
{
	int a = 0;
}

void PlayerScript::GiveDamage(int _Dmg)
{
	if (m_Immune)
		return;

	m_Damaged = true;
	m_HP -= _Dmg;
}