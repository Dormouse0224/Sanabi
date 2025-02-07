#include "pch.h"
#include "CPlayerScript.h"

#include "CAssetMgr.h"
#include "CMaterial.h"
#include "CRenderMgr.h"

#include "CTransform.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CTaskMgr.h"
#include "CPhysxMgr.h"

#include "CCollider2D.h"
#include "CMeshRender.h"
#include "CFlipbookRender.h"
#include "CParticleRender.h"
#include "CGameObject.h"

CPlayerScript::CPlayerScript()
	: m_Idle(false)
{
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::Tick()
{
	CTransform* pTrans = GetOwner()->Transform();

	Vec3 vPos = pTrans->GetRelativePos();


	PxRigidDynamic* pBody = (PxRigidDynamic*)(CPhysxMgr::GetInst()->FindRigidBody(GetOwner()));
	// 키입력에 따라서 사각형이 움직이도록 한다.
	if (KEY_PRESSED(Keyboard::LEFT))
	{
		pBody->addForce(PxVec3(-100.f, 0.f, 0.f), PxForceMode::eACCELERATION);
	}
	if (KEY_PRESSED(Keyboard::RIGHT))
	{
		pBody->addForce(PxVec3(100.f, 0.f, 0.f), PxForceMode::eACCELERATION);
	}
	if (KEY_RELEASED(Keyboard::LEFT))
	{
		pBody->addForce(PxVec3(100.f, 0.f, 0.f), PxForceMode::eFORCE);
	}
	if (KEY_RELEASED(Keyboard::RIGHT))
	{
		pBody->addForce(PxVec3(-100.f, 0.f, 0.f), PxForceMode::eFORCE);
	}
	if (KEY_TAP(Keyboard::SPACE))
	{
		pBody->addForce(PxVec3(0.f, 200.f, 0.f), PxForceMode::eVELOCITY_CHANGE);
		GetOwner()->FindChild(L"ParticleChild")->ParticleRender()->SetActiveState(true);
	}
	//if (KEY_PRESSED(Keyboard::UP))
	//	vPos.y += 100.f * DT;
	//if (KEY_PRESSED(Keyboard::DOWN))
	//	vPos.y -= 100.f * DT;



	// 상태 애니메이션
	if (KEY_NONE(Keyboard::LEFT) && KEY_NONE(Keyboard::RIGHT) && m_Idle == false)
	{
		m_Idle = true;
		m_Run = false;
		GetOwner()->FlipbookRender()->Play(0, 5, true);
		vector<CGameObject*> vec = GetOwner()->GetChild();
		for (int i = 0; i < vec.size(); ++i)
		{
			CFlipbookRender* pFlipRender = vec[i]->FlipbookRender();
			if (pFlipRender)
			{
				pFlipRender->Play(0, 5, true);
			}
		}
	}
	if ((KEY_PRESSED(Keyboard::LEFT) || KEY_PRESSED(Keyboard::RIGHT)) && m_Run == false)
	{
		m_Idle = false;
		m_Run = true;
		GetOwner()->FlipbookRender()->Play(1, 10, true);
		vector<CGameObject*> vec = GetOwner()->GetChild();
		for (int i = 0; i < vec.size(); ++i)
		{
			CFlipbookRender* pFlipRender = vec[i]->FlipbookRender();
			if (pFlipRender)
			{
				pFlipRender->Play(1, 10, true);
			}
		}
	}

	


	pTrans->SetRelativePos(vPos);
}


void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{

}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	if (_Other->GetLayerIdx() == 4)
	{
		int a = 0;
	}
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
}
