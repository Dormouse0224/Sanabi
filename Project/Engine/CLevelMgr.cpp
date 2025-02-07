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
#include "CPhysxActor.h"

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
	pObject->AddComponent(new CPhysxActor);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pObject->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pObject->FlipbookRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"SNB_Idle_Flipbook"));
	pObject->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"SNB_Running_Flipbook"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::DYNAMIC, LINEAR_Z | ANGULAR_ALL);
	COLLIDER_DESC desc;
	desc.Restitution - -1.f;
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = FILTER_LAYER::ePLAYER;
	desc.FilterLayer_Other = FILTER_LAYER::eLANDSCAPE;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(15.f, 25.f, 1.f), PxVec3(0.f, -4.f, 0.f));

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

	pChild->Transform()->SetIndependentScale(true);
	pChild->Transform()->SetRelativePos(0.f, 0.f, -1.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);


	pChild = new CGameObject;
	pChild->SetName(L"ParticleChild");
	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CParticleRender);

	pChild->ParticleRender()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture2D\\Default-Particle.png"));
	pChild->ParticleRender()->SetSpawnRate(0.f);
	pChild->ParticleRender()->SetSpawnCount(10);
	pChild->ParticleRender()->SetSpawnShape(0);
	pChild->ParticleRender()->SetSpawnShapeScale(Vec3(100.f, 100.f, 0.f));
	pChild->ParticleRender()->SetMinScale(Vec3(1.f));
	pChild->ParticleRender()->SetMaxScale(Vec3(5.f));
	pChild->ParticleRender()->SetMinSpeed(2.f);
	pChild->ParticleRender()->SetMaxSpeed(5.f);
	pChild->ParticleRender()->SetMinLife(2.f);
	pChild->ParticleRender()->SetMaxLife(2.f);
	pChild->ParticleRender()->SetSpaceType(1);

	// 부모 자식 연결
	pObject->AddChild(pChild);

	// 레벨에 부모 오브젝트만 추가
	m_CurLevel->AddGameObject(pObject, 3, true);


	// ========
	// Particle
	// ========

	pObject = new CGameObject;
	pObject->SetName(L"Particle_Fireworks");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleRender);

	pObject->Transform()->SetRelativePos(300.f, 50.f, 100.f);
	pObject->Transform()->SetRelativeScale(1.f, 1.f, 1.f);

	pObject->ParticleRender()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture2D\\Ambient_Circle.png"));
	pObject->ParticleRender()->SetSpawnRate(10.f);
	pObject->ParticleRender()->SetSpawnCount(2);
	pObject->ParticleRender()->SetSpawnShape(0);
	pObject->ParticleRender()->SetSpawnShapeScale(Vec3(100.f, 100.f, 0.f));
	pObject->ParticleRender()->SetMinScale(Vec3(1.f));
	pObject->ParticleRender()->SetMaxScale(Vec3(5.f));
	pObject->ParticleRender()->SetMinSpeed(2.f);
	pObject->ParticleRender()->SetMaxSpeed(5.f);
	pObject->ParticleRender()->SetMinLife(2.f);
	pObject->ParticleRender()->SetMaxLife(2.f);
	pObject->ParticleRender()->SetSpaceType(1);
	pObject->ParticleRender()->SetActiveState(true);
	pObject->ParticleRender()->SetGravityState(false);

	//오브젝트를 0번 레이어에 추가
	m_CurLevel->AddGameObject(pObject, 0, false);


	// ========
	// Platform
	// ========

	pObject = new CGameObject;
	pObject->SetName(L"Platform_1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPhysxActor);

	pObject->Transform()->SetRelativePos(0.f, -50.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::STATIC);
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = FILTER_LAYER::eLANDSCAPE;
	desc.FilterLayer_Other = FILTER_LAYER::ePLAYER;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(80.f, 10.f, 10.f), PxVec3(0.f, -20.f, 0.f));

	m_CurLevel->AddGameObject(pObject, 4, true);


	pObject = new CGameObject;
	pObject->SetName(L"Platform_2");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPhysxActor);

	pObject->Transform()->SetRelativePos(300.f, -20.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::STATIC);
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = FILTER_LAYER::eLANDSCAPE;
	desc.FilterLayer_Other = FILTER_LAYER::ePLAYER;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(80.f, 10.f, 60.f), PxVec3(0.f, -20.f, 0.f));

	m_CurLevel->AddGameObject(pObject, 4, true);


	pObject = new CGameObject;
	pObject->SetName(L"Platform_2");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPhysxActor);

	pObject->Transform()->SetRelativePos(-300.f, -80.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::STATIC);
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = FILTER_LAYER::eLANDSCAPE;
	desc.FilterLayer_Other = FILTER_LAYER::ePLAYER;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(80.f, 10.f, 10.f), PxVec3(0.f, -20.f, 0.f));

	m_CurLevel->AddGameObject(pObject, 4, true);

	// 충돌 레이어 지정
	//CCollisionMgr::GetInst()->CollisionLayerCheck(3, 4);
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