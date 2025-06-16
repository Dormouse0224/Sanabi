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
	// 틱마다 플레이어와 연결되어 있는지 검사 - 플레이어 또는 갈고리와 충돌중인 경우 천천히 낙하
	// 최상위 부모가 이 스크립트를 가지며, 자식 객체도 검사한다

	// 충돌체 컴포넌트가 없으면 false
	CPhysxActor* PhysxActor = _Obj->PhysxActor();
	if (PhysxActor == nullptr)
		return false;

	// 플레이어와 컨테이너는 시뮬레이션 오브젝트, 충돌 이벤트가 발생함. 충돌 오브젝트에서 플레이어가 있는 경우 true 반환.
	set<CGameObject*> setContactObj = PhysxActor->GetContactObj();
	if (CGameObject* pPlayer = CLevelMgr::GetInst()->FindObject(LAYER::Player, L"Player"))
	{
		auto iterPlayer = setContactObj.find(pPlayer);
		if (iterPlayer != setContactObj.end())
			return true;
	}

	// 앵커는 트리거 오브젝트, 트리거 이벤트가 발생함.
	if (CGameObject* pAnchor = CLevelMgr::GetInst()->FindObject(LAYER::PlayerProjectile, L"Anchor"))
	{
		auto iterPlayerGrab = setContactObj.find(pAnchor);
		if (iterPlayerGrab != setContactObj.end())
			return true;
	}

	// 자식 객체에 대해서도 검사
	vector<CGameObject*> vecChild = _Obj->GetChild();
	for (CGameObject* child : vecChild)
	{
		if (CheckConnection(child))
			return true;
	}

	// 모두 실패시 false
	return false;
}
