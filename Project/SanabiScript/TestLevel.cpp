#include "pch.h"
#include "Engine\CLevelMgr.h"

#include "Engine\CDevice.h"
#include "Engine\CLevel.h"
#include "Engine\CLayer.h"

#include "Engine\CAssetMgr.h"
#include "Engine\CPhysxMgr.h"
#include "Engine\CTaskMgr.h"
#include "Engine\CLevelMgr.h"

#include "Engine\CGameObject.h"
#include "Engine\components.h"
#include "Engine\CPhysxActor.h"

#include "Engine\CSetColorCS.h"

#include "Scripts\CPlayerScript.h"
#include "Scripts\CCameraMoveScript.h"
#include "TestLevel.h"

TestLevel::TestLevel()
	: CLevel()
{
	// 카메라 역할 오브젝트 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetPriority(0); // 메인 카메라로 설정
	pCamObj->Camera()->CheckLayerAll();
	pCamObj->Camera()->CheckLayer(LAYER::UI);

	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pCamObj, (DWORD_PTR)LAYER::Default);

	// 배경 오브젝트
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Background");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 300.f);
	pObject->Transform()->SetRelativeScale(1000.f, 1000.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"Background_Mtrl"));

	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pObject, (DWORD_PTR)LAYER::Background);


	// Parent Object
	pObject = new CGameObject;
	pObject->SetName(L"Parent");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFlipbookRender);
	pObject->AddComponent(new CPhysxActor);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pObject->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pObject->FlipbookRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	pObject->FlipbookRender()->AddFlipbook(CAssetMgr::GetInst()->Load<CFlipbook>(L"SNB_Idle_Flipbook"));
	pObject->FlipbookRender()->AddFlipbook(CAssetMgr::GetInst()->Load<CFlipbook>(L"SNB_Running_Flipbook"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::DYNAMIC, LINEAR_Z | ANGULAR_ALL, true);
	COLLIDER_DESC desc;
	desc.Restitution - -1.f;
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = COLLISION_LAYER::ePLAYER;
	desc.FilterLayer_Other = COLLISION_LAYER::eLANDSCAPE;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(15.f, 25.f, 1.f), PxVec3(0.f, -4.f, 0.f));

	// Child Object
	CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Child");
	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CFlipbookRender);
	//pChild->AddComponent(new CCollider2D);

	pChild->Transform()->SetIndependentScale(true);
	pChild->Transform()->SetRelativePos(0.f, 0.f, -1.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	//pChild->Collider2D()->SetNotifyParent(true);
	//pChild->Collider2D()->SetOffset(Vec2(-0.1f, -0.05f));
	//pChild->Collider2D()->SetScale(Vec2(0.15f, 0.33f));

	pChild->FlipbookRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	pChild->FlipbookRender()->AddFlipbook(CAssetMgr::GetInst()->Load<CFlipbook>(L"SNBArm_Idle_Flipbook"));
	pChild->FlipbookRender()->AddFlipbook(CAssetMgr::GetInst()->Load<CFlipbook>(L"SNBArm_Running_Flipbook"));

	// 부모 자식 연결
	pObject->AddChild(pChild);

	pChild->Transform()->SetIndependentScale(true);
	pChild->Transform()->SetRelativePos(0.f, 0.f, -1.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);


	pChild = new CGameObject;
	pChild->SetName(L"ParticleChild");
	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CParticleRender);

	pChild->ParticleRender()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture2D>(L"Texture2D\\TX_HitFlash_0.png"));
	pChild->ParticleRender()->SetStartColor(Vec4(1.f, 0.f, 0.f, 1.f));
	pChild->ParticleRender()->SetEndColor(Vec4(1.f, 0.f, 0.f, 0.f));
	pChild->ParticleRender()->SetSpawnRate(0.f);
	pChild->ParticleRender()->SetSpawnCount(10);
	pChild->ParticleRender()->SetSpawnShape(0);
	pChild->ParticleRender()->SetSpawnShapeScale(Vec3(100.f, 100.f, 0.f));
	pChild->ParticleRender()->SetSpawnDir(Vec3(0.f, 1.f, 0.f));
	pChild->ParticleRender()->SetSpawnDirRandomize(0.2f);
	pChild->ParticleRender()->SetMinScale(Vec3(10.f));
	pChild->ParticleRender()->SetMaxScale(Vec3(30.f));
	pChild->ParticleRender()->SetMinSpeed(200.f);
	pChild->ParticleRender()->SetMaxSpeed(500.f);
	pChild->ParticleRender()->SetMinLife(2.f);
	pChild->ParticleRender()->SetMaxLife(4.f);
	pChild->ParticleRender()->SetSpaceType(0);

	// 부모 자식 연결
	pObject->AddChild(pChild);

	// 레벨에 부모 오브젝트만 추가
	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pObject, (DWORD_PTR)LAYER::Player);


	// ========
	// Particle
	// ========

	pObject = new CGameObject;
	pObject->SetName(L"Particle_Bubble");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleRender);

	pObject->Transform()->SetRelativePos(300.f, 50.f, 100.f);
	pObject->Transform()->SetRelativeScale(10.f, 10.f, 1.f);

	pObject->ParticleRender()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture2D>(L"Texture2D\\Ambient_Circle.png"));
	pObject->ParticleRender()->SetStartColor(Vec4(0.f, 1.f, 0.f, 1.f));
	pObject->ParticleRender()->SetEndColor(Vec4(0.f, 1.f, 0.f, 0.f));
	pObject->ParticleRender()->SetSpawnRate(10.f);
	pObject->ParticleRender()->SetSpawnCount(2);
	pObject->ParticleRender()->SetSpawnShape(0);
	pObject->ParticleRender()->SetSpawnShapeScale(Vec3(50.f, 50.f, 0.f));
	pObject->ParticleRender()->SetSpawnDir(Vec3(1.f, 0.f, 0.f));
	pObject->ParticleRender()->SetSpawnDirRandomize(1.f);
	pObject->ParticleRender()->SetMinScale(Vec3(5.f));
	pObject->ParticleRender()->SetMaxScale(Vec3(50.f));
	pObject->ParticleRender()->SetMinSpeed(50.f);
	pObject->ParticleRender()->SetMaxSpeed(100.f);
	pObject->ParticleRender()->SetMinLife(5.f);
	pObject->ParticleRender()->SetMaxLife(10.f);
	pObject->ParticleRender()->SetSpaceType(0);
	pObject->ParticleRender()->SetActiveState(true);
	pObject->ParticleRender()->SetGravityState(false);


	//오브젝트를 0번 레이어에 추가
	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pObject, (DWORD_PTR)LAYER::EnermyProjectile);


	pObject = new CGameObject;
	pObject->SetName(L"Particle_Rain");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleRender);

	pObject->Transform()->SetRelativePos(100.f, 300.f, 100.f);
	pObject->Transform()->SetRelativeScale(10.f, 10.f, 1.f);

	pObject->ParticleRender()->SetParticleTex(CAssetMgr::GetInst()->Load<CTexture2D>(L"Texture2D\\Sprsheet_RainParticle_5x1.png"));
	pObject->ParticleRender()->SetStartColor(Vec4(1.f, 1.f, 1.f, 1.f));
	pObject->ParticleRender()->SetEndColor(Vec4(1.f, 1.f, 1.f, 1.f));
	pObject->ParticleRender()->SetSpawnRate(10.f);
	pObject->ParticleRender()->SetSpawnCount(2);
	pObject->ParticleRender()->SetSpawnShape(0);
	pObject->ParticleRender()->SetSpawnShapeScale(Vec3(1500.f, 0.f, 0.f));
	pObject->ParticleRender()->SetSpawnDir(Vec3(2.f, -4.f, 0.f));
	pObject->ParticleRender()->SetSpawnDirRandomize(0.f);
	pObject->ParticleRender()->SetMinScale(Vec3(50.f));
	pObject->ParticleRender()->SetMaxScale(Vec3(100.f));
	pObject->ParticleRender()->SetMinSpeed(200.f);
	pObject->ParticleRender()->SetMaxSpeed(300.f);
	pObject->ParticleRender()->SetMinLife(3.f);
	pObject->ParticleRender()->SetMaxLife(5.f);
	pObject->ParticleRender()->SetSpaceType(0);
	pObject->ParticleRender()->SetActiveState(true);

	//오브젝트를 0번 레이어에 추가
	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pObject, (DWORD_PTR)LAYER::Background);


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

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::STATIC);
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = COLLISION_LAYER::eLANDSCAPE;
	desc.FilterLayer_Other = COLLISION_LAYER::ePLAYER;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(80.f, 10.f, 10.f), PxVec3(0.f, -20.f, 0.f));

	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pObject, (DWORD_PTR)LAYER::Background);


	pObject = new CGameObject;
	pObject->SetName(L"Platform_2");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPhysxActor);

	pObject->Transform()->SetRelativePos(300.f, -20.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::STATIC);
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = COLLISION_LAYER::eLANDSCAPE;
	desc.FilterLayer_Other = COLLISION_LAYER::ePLAYER;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(80.f, 10.f, 60.f), PxVec3(0.f, -20.f, 0.f));

	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pObject, (DWORD_PTR)LAYER::Background);


	pObject = new CGameObject;
	pObject->SetName(L"Platform_3");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CPhysxActor);

	pObject->Transform()->SetRelativePos(-300.f, -80.f, 101.f);
	pObject->Transform()->SetRelativeScale(200.f, 100.f, 1.f);

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh"));
	pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"CargoPlatform_Mtrl"));

	pObject->PhysxActor()->SetRigidBody(RIGID_TYPE::STATIC);
	desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
	desc.FilterLayer_Self = COLLISION_LAYER::eLANDSCAPE;
	desc.FilterLayer_Other = COLLISION_LAYER::ePLAYER;
	pObject->PhysxActor()->AddCollider(desc, PxVec3(80.f, 10.f, 10.f), PxVec3(0.f, -20.f, 0.f));

	CTaskMgr::GetInst()->AddTask(TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)pObject, (DWORD_PTR)LAYER::Background);

	// 충돌 레이어 지정
	//CCollisionMgr::GetInst()->CollisionLayerCheck(3, 4);
	//CCollisionMgr::GetInst()->CollisionLayerCheck(4, 5);
	//CCollisionMgr::GetInst()->CollisionLayerCheck(3, 6);

	CLevelMgr::GetInst()->SetCurrentLevel(this);
}

TestLevel::~TestLevel()
{
}