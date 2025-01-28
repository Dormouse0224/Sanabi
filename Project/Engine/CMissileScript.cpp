#include "pch.h"
#include "CMissileScript.h"

#include "CTransform.h"
#include "CTimeMgr.h"

CMissileScript::CMissileScript()
	: m_vDir(0.f, 1.f, 0.f)
	, m_Speed(100.f)
{
}

CMissileScript::~CMissileScript()
{
}


void CMissileScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	vPos += m_vDir * m_Speed * DT;

	Transform()->SetRelativePos(vPos);
}

void CMissileScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	if (L"Monster" == _Other->GetName())
	{
		_Other->Destroy();
	}
}


