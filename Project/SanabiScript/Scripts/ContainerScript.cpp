#include "pch.h"
#include "ContainerScript.h"

#include "Engine\CPhysxActor.h"
#include "Engine\CTransform.h"
#include "Engine\CLevelMgr.h"


ContainerScript::ContainerScript()
	: CScript()
	, m_Gap(0.f)
	, m_Speed(0.2f)
{
}

ContainerScript::ContainerScript(const ContainerScript& _Origin)
	: CScript(_Origin)
	, m_Gap(0.f)
	, m_Speed(_Origin.m_Speed)
{
}

ContainerScript::~ContainerScript()
{
}

void ContainerScript::Tick()
{
	if (CheckConnection(GetOwner()))
	{
		CGameObject* pPlayer = CLevelMgr::GetInst()->FindObject(LAYER::Player, L"Player");
		CGameObject* pAnchor = CLevelMgr::GetInst()->FindObject(LAYER::PlayerProjectile, L"Anchor");

		GetOwner()->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos() - Vec3(0.f, m_Speed, 0.f));
		if (pPlayer)
			pPlayer->Transform()->SetRelativePos(pPlayer->Transform()->GetRelativePos() - Vec3(0.f, m_Speed, 0.f));
		if (pAnchor)
			pAnchor->Transform()->SetRelativePos(pAnchor->Transform()->GetRelativePos() - Vec3(0.f, m_Speed, 0.f));
		m_Gap += m_Speed;
	}
	else
	{
		if (m_Gap > 0.f)
		{
			GetOwner()->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos() + Vec3(0.f, m_Speed, 0.f));
			m_Gap -= m_Speed;
		}
	}

}

bool ContainerScript::CheckConnection(CGameObject* _Obj)
{
	// ƽ���� �÷��̾�� ����Ǿ� �ִ��� �˻� - �÷��̾� �Ǵ� ������ �浹���� ��� õõ�� ����
	// �ֻ��� �θ� �� ��ũ��Ʈ�� ������, �ڽ� ��ü�� �˻��Ѵ�

	// �浹ü ������Ʈ�� ������ false
	CPhysxActor* PhysxActor = _Obj->PhysxActor();
	if (PhysxActor == nullptr)
		return false;

	// �÷��̾�� �����̳ʴ� �ùķ��̼� ������Ʈ, �浹 �̺�Ʈ�� �߻���. �浹 ������Ʈ���� �÷��̾ �ִ� ��� true ��ȯ.
	set<CGameObject*> setContactObj = PhysxActor->GetContactObj();
	if (CGameObject* pPlayer = CLevelMgr::GetInst()->FindObject(LAYER::Player, L"Player"))
	{
		auto iterPlayer = setContactObj.find(pPlayer);
		if (iterPlayer != setContactObj.end())
			return true;
	}

	// ��Ŀ�� Ʈ���� ������Ʈ, Ʈ���� �̺�Ʈ�� �߻���.
	if (CGameObject* pAnchor = CLevelMgr::GetInst()->FindObject(LAYER::PlayerProjectile, L"Anchor"))
	{
		auto iterPlayerGrab = setContactObj.find(pAnchor);
		if (iterPlayerGrab != setContactObj.end())
			return true;
	}

	// �ڽ� ��ü�� ���ؼ��� �˻�
	vector<CGameObject*> vecChild = _Obj->GetChild();
	for (CGameObject* child : vecChild)
	{
		if (CheckConnection(child))
			return true;
	}

	// ��� ���н� false
	return false;
}
