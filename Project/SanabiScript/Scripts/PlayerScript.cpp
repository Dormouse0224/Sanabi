#include "pch.h"
#include "PlayerScript.h"

#include "Engine/CPhysxActor.h"

PlayerScript::PlayerScript()
	: CScript()
	, m_bInit(false)
	, m_bAirborne(false)
	, m_bWallContact(false)
	, m_bWallLeft(false)
{
}

PlayerScript::PlayerScript(const PlayerScript& _Origin)
	: CScript(_Origin)
	, m_bInit(false)
	, m_bAirborne(false)
	, m_bWallContact(false)
	, m_bWallLeft(false)
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::Tick()
{
	if (!m_bInit)
		Init();

	m_bAirborne = m_bAirborneCheck;
	m_bAirborneCheck = true;
	m_bWallContact = m_bWallContactCheck;
	m_bWallContactCheck = false;

}

void PlayerScript::Init()
{
	GetOwner()->PhysxActor()->SetContactBegin(this, (ContactFunc)(&PlayerScript::ContactBegin));
	GetOwner()->PhysxActor()->SetContactTick(this, (ContactFunc)(&PlayerScript::ContacTick));
	GetOwner()->PhysxActor()->SetContactEnd(this, (ContactFunc)(&PlayerScript::ContactEnd));
	m_bInit = true;
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

