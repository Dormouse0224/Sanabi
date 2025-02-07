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

    // ������Ʈ ��ǥ ��������
    PxTransform Trans(PxVec3(GetOwner()->Transform()->GetRelativePos().x, GetOwner()->Transform()->GetRelativePos().y, GetOwner()->Transform()->GetRelativePos().z));

    if (_Type == RIGID_TYPE::DYNAMIC || _Type == RIGID_TYPE::KINEMATIC)
    {
        // ��ü ����, ���� ���� ���
        PxRigidDynamic* pBody = CPhysxMgr::GetInst()->m_Physics->createRigidDynamic(Trans);
        pBody->userData = GetOwner();
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, _LockFlag & LINEAR_X);       // Z�� �̵� ���
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, _LockFlag & LINEAR_Y);       // Z�� �̵� ���
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, _LockFlag & LINEAR_Z);       // Z�� �̵� ���
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, _LockFlag & ANGULAR_X);      // X�� ȸ�� ���
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, _LockFlag & ANGULAR_Y);      // Y�� ȸ�� ���
        pBody->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, _LockFlag & ANGULAR_Z);      // Z�� ȸ�� ���
        pBody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, _DisableGravity);
        CPhysxMgr::GetInst()->m_Scene->addActor(*pBody);

        //if (_Type == RIGID_TYPE::KINEMATIC)
            //pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // ������Ʈ�� �ùķ��̼� ��ü ��Ͽ� �߰�
        CPhysxMgr::GetInst()->m_mapRigidBody.insert(make_pair(GetOwner(), pBody));
    }
    else if (_Type == RIGID_TYPE::STATIC)
    {
        // ��ü ����, ���� ���� ���
        PxRigidStatic* pBody = CPhysxMgr::GetInst()->m_Physics->createRigidStatic(Trans);
        pBody->userData = GetOwner();
        CPhysxMgr::GetInst()->m_Scene->addActor(*pBody);

        // ������Ʈ�� �ùķ��̼� ��ü ��Ͽ� �߰�
        CPhysxMgr::GetInst()->m_mapRigidBody.insert(make_pair(GetOwner(), pBody));
    }

}

void CPhysxActor::AddCollider(COLLIDER_DESC _desc, PxVec3 _Scale, PxVec3 _Offset)
{
    PxRigidActor* Body = CPhysxMgr::GetInst()->FindRigidBody(GetOwner());
    assert(Body);
    // �������, ź�����, ��� ����
    PxMaterial* pMaterial = CPhysxMgr::GetInst()->m_Physics->createMaterial(_desc.StaticFriction, _desc.DynamicFriction, _desc.Restitution);
    pMaterial->setRestitutionCombineMode(PxCombineMode::eMIN);
    PxShape* shape = CPhysxMgr::GetInst()->m_Physics->createShape(PxBoxGeometry(_Scale), *pMaterial);
    shape->setFlag(_desc.ShapeFlag, true);

    // �浹 ���� ���̾� ����
    PxFilterData filterData;
    filterData.word0 = _desc.FilterLayer_Self;      // ������ ���� ���̾�
    filterData.word1 = _desc.FilterLayer_Other;     // �浹 �˻縦 �� ���̾�
    shape->setSimulationFilterData(filterData);

    // ������ ����
    shape->setLocalPose(PxTransform(_Offset));

    // ��ü�� �浹ü �߰�
    Body->attachShape(*shape);

    if (PxConcreteType::eRIGID_DYNAMIC == Body->getConcreteType())
    {
        PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)Body, m_Density);
    }
}
