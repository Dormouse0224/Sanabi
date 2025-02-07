#include "pch.h"
#include "CPhysxActor.h"
#include "CTransform.h"


CPhysxActor::CPhysxActor()
    : CComponent(COMPONENT_TYPE::PHYSXACTOR)
    , m_Density(1.f)
{

}

CPhysxActor::~CPhysxActor()
{

}

void CPhysxActor::SetRigidBody(RIGID_TYPE _Type, UINT _LockFlag, float _Density, bool _DisableGravity)
{
    m_Density = _Density;

    // 오브젝트 좌표 가져오기
    PxTransform Trans(PxVec3(GetOwner()->Transform()->GetRelativePos().x, GetOwner()->Transform()->GetRelativePos().y, GetOwner()->Transform()->GetRelativePos().z));

    if (_Type == RIGID_TYPE::DYNAMIC || _Type == RIGID_TYPE::KINEMATIC)
    {
        // 강체 생성, 씬에 액터 등록
        PxRigidDynamic* pBody = CPhysxMgr::GetInst()->m_Physics->createRigidDynamic(Trans);
        pBody->userData = GetOwner();
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, _LockFlag & LINEAR_X);       // Z축 이동 잠금
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, _LockFlag & LINEAR_Y);       // Z축 이동 잠금
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, _LockFlag & LINEAR_Z);       // Z축 이동 잠금
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, _LockFlag & ANGULAR_X);      // X축 회전 잠금
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, _LockFlag & ANGULAR_Y);      // Y축 회전 잠금
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, _LockFlag & ANGULAR_Z);      // Z축 회전 잠금
        pBody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, _DisableGravity);
        CPhysxMgr::GetInst()->m_Scene->addActor(*pBody);

        //if (_Type == RIGID_TYPE::KINEMATIC)
            //pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // 오브젝트를 시뮬레이션 강체 목록에 추가
        CPhysxMgr::GetInst()->m_mapRigidBody.insert(make_pair(GetOwner(), pBody));
    }
    else if (_Type == RIGID_TYPE::STATIC)
    {
        // 강체 생성, 씬에 액터 등록
        PxRigidStatic* pBody = CPhysxMgr::GetInst()->m_Physics->createRigidStatic(Trans);
        pBody->userData = GetOwner();
        CPhysxMgr::GetInst()->m_Scene->addActor(*pBody);

        // 오브젝트를 시뮬레이션 강체 목록에 추가
        CPhysxMgr::GetInst()->m_mapRigidBody.insert(make_pair(GetOwner(), pBody));
    }

}

void CPhysxActor::AddCollider(COLLIDER_DESC _desc, PxVec3 _Scale, PxVec3 _Offset)
{
    PxRigidActor* Body = CPhysxMgr::GetInst()->FindRigidBody(GetOwner());
    assert(Body);
    // 마찰계수, 탄성계수, 모양 설정
    PxMaterial* pMaterial = CPhysxMgr::GetInst()->m_Physics->createMaterial(_desc.StaticFriction, _desc.DynamicFriction, _desc.Restitution);
    pMaterial->setRestitutionCombineMode(PxCombineMode::eMIN);
    PxShape* shape = CPhysxMgr::GetInst()->m_Physics->createShape(PxBoxGeometry(_Scale), *pMaterial);
    shape->setFlag(_desc.ShapeFlag, true);

    // 충돌 필터 레이어 설정
    PxFilterData filterData;
    filterData.word0 = _desc.FilterLayer_Self;      // 본인이 속한 레이어
    filterData.word1 = _desc.FilterLayer_Other;     // 충돌 검사를 할 레이어
    shape->setSimulationFilterData(filterData);

    // 오프셋 설정
    shape->setLocalPose(PxTransform(_Offset));

    // 강체에 충돌체 추가
    Body->attachShape(*shape);

    if (PxConcreteType::eRIGID_DYNAMIC == Body->getConcreteType())
    {
        PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)Body, m_Density);
    }
}
