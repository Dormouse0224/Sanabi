#include "pch.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CAssetMgr.h"
#include "CCollisionMgr.h"
#include "CPhysxMgr.h"

#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CSetColorCS.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	delete m_CurLevel;
}

void CLevelMgr::Init()
{
	// Level 생성
	m_CurLevel = new CLevel;

	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Tile");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	m_CurLevel->GetLayer(5)->SetName(L"Enermy");
	m_CurLevel->GetLayer(6)->SetName(L"EnermyProjectile");

	// 카메라 역할 오브젝트 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetPriority(0); // 메인 카메라로 설정
	pCamObj->Camera()->CheckLayerAll();
	pCamObj->Camera()->CheckLayer(31);

	m_CurLevel->AddGameObject(pCamObj, 0, false);

	// Parent Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Parent");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFlipbookRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pObject->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pObject->FlipbookRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"SNB_Idle_Flipbook"));
	pObject->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"SNB_Running_Flipbook"));

	pObject->Collider2D()->SetScale(Vec2(0.3f, 0.5f));
	pObject->Collider2D()->SetOffset(Vec2(0.f, -0.04f));

	CPhysxMgr::GetInst()->AddDynamicActor(pObject, PxVec3(15.f, 25.f, 1.f), PxVec3(0.f, -4.f, 0.f));

	// Child Object
	CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Child");
	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CFlipbookRender);
	pChild->AddComponent(new CCollider2D);

	pChild->Transform()->SetIndependentScale(true);
	pChild->Transform()->SetRelativePos(0.f, 0.f, -1.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pChild->Collider2D()->SetNotifyParent(true);
	pChild->Collider2D()->SetOffset(Vec2(-0.1f, -0.05f));
	pChild->Collider2D()->SetScale(Vec2(0.15f, 0.33f));

	pChild->FlipbookRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pChild->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"SNBArm_Idle_Flipbook"));
	pChild->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"SNBArm_Running_Flipbook"));

	// 부모 자식 연결
	pObject->AddChild(pChild);

	// 레벨에 부모 오브젝트만 추가
	m_CurLevel->AddGameObject(pObject, 3, true);


	// ========
	// Particle
	// ========

	//pObject = new CGameObject;
	//pObject->SetName(L"Particle");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CParticleRender);

	//pObject->Transform()->SetRelativePos(0.f, 300.f, 100.f);
	//pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	//pObject->ParticleRender()->SetSpawnRate(10.f);

	//// 오브젝트를 0번 레이어에 추가
	//m_CurLevel->AddGameObject(pObject, 0, false);


	// ========
	// Platform
	// ========

	pObject = new CGameObject;
	pObject->SetName(L"Platform_1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(0.f, -50.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->Collider2D()->SetScale(Vec2(0.8f, 0.2f));
	pObject->Collider2D()->SetOffset(Vec2(0.f, -0.23f));

	CPhysxMgr::GetInst()->AddStaticActor(pObject, PxVec3(80.f, 10.f, 5.f), PxVec3(0.f, -20.f, 0.f));

	m_CurLevel->AddGameObject(pObject, 4, true);


	pObject = new CGameObject;
	pObject->SetName(L"Platform_2");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(200.f, -30.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->Collider2D()->SetScale(Vec2(0.8f, 0.2f));
	pObject->Collider2D()->SetOffset(Vec2(0.f, -0.23f));

	CPhysxMgr::GetInst()->AddStaticActor(pObject, PxVec3(80.f, 10.f, 5.f), PxVec3(0.f, -20.f, 0.f));

	m_CurLevel->AddGameObject(pObject, 4, true);


	pObject = new CGameObject;
	pObject->SetName(L"Platform_2");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(-200.f, -70.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->Collider2D()->SetScale(Vec2(0.8f, 0.2f));
	pObject->Collider2D()->SetOffset(Vec2(0.f, -0.23f));

	CPhysxMgr::GetInst()->AddStaticActor(pObject, PxVec3(80.f, 10.f, 5.f), PxVec3(0.f, -20.f, 0.f));

	m_CurLevel->AddGameObject(pObject, 4, true);

	// 충돌 레이어 지정
	CCollisionMgr::GetInst()->CollisionLayerCheck(3, 4);
	//CCollisionMgr::GetInst()->CollisionLayerCheck(4, 5);
	//CCollisionMgr::GetInst()->CollisionLayerCheck(3, 6);



	// 레벨 시작
	m_CurLevel->Begin();


}

void CLevelMgr::Progress()
{
	// =====
	// Tick
	// =====
	m_CurLevel->Tick();

	m_CurLevel->Deregister();

	m_CurLevel->FinalTick();
}