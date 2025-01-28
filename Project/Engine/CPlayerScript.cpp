#include "pch.h"
#include "CPlayerScript.h"

#include "CAssetMgr.h"
#include "CMaterial.h"
#include "CRenderMgr.h"

#include "CTransform.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CTaskMgr.h"

#include "CCollider2D.h"
#include "CMeshRender.h"
#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
{

}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::Tick()
{
	CTransform* pTrans = GetOwner()->Transform();

	Vec3 vPos = pTrans->GetRelativePos();

	// 키입력에 따라서 사각형이 움직이도록 한다.
	if (KEY_PRESSED(Keyboard::LEFT))
		vPos.x -= 100.f * DT;
	if (KEY_PRESSED(Keyboard::RIGHT))
		vPos.x += 100.f * DT;
	if (KEY_PRESSED(Keyboard::UP))
		vPos.y += 100.f * DT;
	if (KEY_PRESSED(Keyboard::DOWN))
		vPos.y -= 100.f * DT;

	if (KEY_PRESSED(Keyboard::Z))
	{
		Vec3 vRot = pTrans->GetRelativeRotation();
		vRot.z += DT * 180.f;
		pTrans->SetRelativeRotation(vRot);

		Vec3 vScale = Transform()->GetRelativeScale();
		vScale.x += DT * 10.f;
		vScale.y += DT * 10.f;
		Transform()->SetRelativeScale(vScale);
	}

	if (KEY_TAP(Keyboard::Y))
	{
		RenderComponent()->GetMaterial()->SetScalarParam(INT_0, 1);
	}
	if (KEY_RELEASED(Keyboard::Y))
	{
		RenderComponent()->GetMaterial()->SetScalarParam(INT_0, 0);
	}

	if (KEY_TAP(Keyboard::SPACE))
	{
		// 미사일 발사
		CGameObject* pMissileObj = new CGameObject;
		pMissileObj->AddComponent(new CTransform);
		pMissileObj->AddComponent(new CMeshRender);
		pMissileObj->AddComponent(new CCollider2D);

		pMissileObj->AddComponent(new CMissileScript);

		pMissileObj->Transform()->SetRelativePos(Transform()->GetWorldPos());
		pMissileObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		pMissileObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
		pMissileObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

		tTask task = {};
		task.Type = TASK_TYPE::CREATE_OBJECT;
		task.Param0 = (DWORD_PTR)pMissileObj;
		task.Param1 = 4;

		CTaskMgr::GetInst()->AddTask(task);
	}

	pTrans->SetRelativePos(vPos);
}


void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{

}
