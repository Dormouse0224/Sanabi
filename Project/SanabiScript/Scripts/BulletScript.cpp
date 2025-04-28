#include "pch.h"
#include "BulletScript.h"

#include "Engine/CLevelMgr.h"
#include "Engine/CTransform.h"
#include "Engine/CPhysxActor.h"

#include "Scripts/PlayerScript.h"

BulletScript::BulletScript()
	: CScript()
{
}

BulletScript::BulletScript(const BulletScript& _Origin)
	: CScript(_Origin)
{
}

BulletScript::~BulletScript()
{
}

void BulletScript::Tick()
{
	if (!m_Init && CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
		Init();

	if (!m_Dead && m_Velocity.Length() > 0)
	{
		// ���� �������� ȸ�� ����
		Vec3 Dir = m_Velocity;
		Dir.Normalize();
		Vec3 axis = XMVector3Cross(Vec3(0, 1, 0), Dir);
		float angle = acosf(XMVectorGetX(XMVector3Dot(Vec3(0, 1, 0), Dir)));
		Vec4 quat = XMQuaternionRotationAxis(axis.Normalize(), angle);
		GetOwner()->Transform()->SetRelativeRot(quat);
	}
	else if (m_Dead)
	{
		// ���� ����Ʈ ȸ�� ����
		Vec3 Dir = -m_Velocity;
		Dir.Normalize();
		Vec3 axis = XMVector3Cross(Vec3(0, 1, 0), Dir);
		float angle = acosf(XMVectorGetX(XMVector3Dot(Vec3(0, 1, 0), Dir)));
		Vec4 quat = XMQuaternionRotationAxis(axis.Normalize(), angle);
		GetOwner()->Transform()->SetRelativeRot(quat);
	}

}

int BulletScript::Save(fstream& _Stream)
{
	return S_OK;
}

int BulletScript::Load(fstream& _Stream)
{
	return S_OK;
}

void BulletScript::Render_Script()
{
}

void BulletScript::SetVelocity(Vec3 _v3)
{
	m_Velocity = _v3;
	GetOwner()->PhysxActor()->SetLinearVelocity(PxVec3(_v3.x, _v3.y, _v3.z));
}

void BulletScript::Init()
{
	m_Init = true;
	GetOwner()->PhysxActor()->SetTriggerBegin(this, (PxTriggerFunc)&BulletScript::TriggerBegin);
}

void BulletScript::TriggerBegin(CGameObject* _Other)
{
	// �浹 �̺�Ʈ �Լ�
	if (m_Dead)
		return;


	GetOwner()->Transform()->SetRelativeScale(GetOwner()->Transform()->GetRelativeScale() * 2);

	CGameObject* pPlayer = CLevelMgr::GetInst()->FindObject(LAYER::Player, L"Player");
	if (_Other == pPlayer)
	{
		PlayerScript* pPS = (PlayerScript*)pPlayer->FindScript("class PlayerScript");
		if (pPS)
		{
			pPS->GiveDamage(m_Damage);
			m_Dead = true;
		}
	}
	else if (_Other->GetLayerIdx() == (int)LAYER::Tile)
	{
		m_Dead = true;
	}
}
