#include "pch.h"
#include "CPhysxActor.h"
#include "CTransform.h"

wstring LOCK_FLAG_WSTR[] =
{
    L"LINEAR_X",
    L"LINEAR_Y",
    L"LINEAR_Z",
    L"ANGULAR_X",
    L"ANGULAR_Y",
    L"ANGULAR_Z",
};

CPhysxActor::CPhysxActor()
    : CComponent(COMPONENT_TYPE::PHYSXACTOR)
    , m_Body(nullptr)
    , m_Type(RIGID_TYPE::NONE)
    , m_Density(1.f)
{

}

CPhysxActor::~CPhysxActor()
{

}

void CPhysxActor::FinalTick()
{

}

void CPhysxActor::SetRigidBody(RIGID_TYPE _Type, UINT _LockFlag, bool _Gravity, float _Density, PxVec3 _InitVel)
{
    m_Type = _Type;
    m_Density = _Density;

    // ������Ʈ ��ǥ ��������
    PxTransform Trans(PxVec3(GetOwner()->Transform()->GetRelativePos().x, GetOwner()->Transform()->GetRelativePos().y, GetOwner()->Transform()->GetRelativePos().z));

    if (_Type == RIGID_TYPE::DYNAMIC || _Type == RIGID_TYPE::KINEMATIC)
    {
        // ��ü ����
        PxRigidDynamic* pBody = CPhysxMgr::GetInst()->GetPhysics()->createRigidDynamic(Trans);
        m_Body = pBody;
        pBody->userData = GetOwner();   // ����� ���� ������ ������ ������Ʈ�� ������ ������Ʈ �ּҸ� ����
        pBody->setRigidDynamicLockFlags((PxRigidDynamicLockFlags)_LockFlag);  // �� �̵� �� ȸ�� ��� �÷���
        pBody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !_Gravity);  // �߷� ���� �ɼ�
        pBody->setLinearVelocity(_InitVel); // ��ü ���� �� �ʱ� �ӵ�

        if (_Type == RIGID_TYPE::KINEMATIC)
            pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // ���� ���� ���
        CPhysxMgr::GetInst()->AddRigidBody(GetOwner(), pBody);
    }
    else if (_Type == RIGID_TYPE::STATIC)
    {
        // ��ü ����
        PxRigidStatic* pBody = CPhysxMgr::GetInst()->GetPhysics()->createRigidStatic(Trans);
        m_Body = pBody;
        pBody->userData = GetOwner();

        // ���� ���� ���
        CPhysxMgr::GetInst()->AddRigidBody(GetOwner(), pBody);
    }

}

void CPhysxActor::SetRigidType(RIGID_TYPE _Type)
{
    if (m_Type == _Type)
        return;

    if (m_Type != RIGID_TYPE::STATIC && _Type != RIGID_TYPE::STATIC)
    {
        // Dynamic <-> Kinematic �� ��� �÷��׸� ��۽�Ų��
        PxRigidDynamic* pBody = (PxRigidDynamic*)m_Body;
        pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, !(pBody->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC));
        m_Type = _Type;
    }
    else
    {
        // �� �� �ϳ��� Static �� ��� ���� ��ü�� �����ϰ� ���� ���ͷ� ���� ����Ѵ�
        CPhysxMgr::GetInst()->RemoveRigidBody(GetOwner());
        SetRigidBody(_Type);

        for (int i = 0; i < m_vecDesc.size(); ++i)
        {
            AttachCollider(m_vecDesc[i], m_vecScale[i], m_vecOffset[i]);
        }
    }
}

void CPhysxActor::SetDensity(float _Density)
{
    m_Density = _Density;
    if (m_Body->getNbShapes() > 0 && PxConcreteType::eRIGID_DYNAMIC == m_Body->getConcreteType())
    {
        PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)m_Body, m_Density);
    }
}

void CPhysxActor::AddCollider(COLLIDER_DESC _Desc, PxVec3 _Scale, PxVec3 _Offset)
{
    m_vecDesc.push_back(_Desc);
    m_vecScale.push_back(_Scale);
    m_vecOffset.push_back(_Offset);

    AttachCollider(_Desc, _Scale, _Offset);
}

void CPhysxActor::UpdatePosition(Vec3 _Pos)
{
    // Scene �� ��ϵ� ��ü�� �����ϴ� ���, �ش� ��ü�� Scene ������ ��ġ�� ������Ʈ
    if (m_Body)
    {
        PxTransform Trans = PxTransform(_Pos.x, _Pos.y, _Pos.z, m_Body->getGlobalPose().q);
        m_Body->setGlobalPose(Trans);
    }
}

void CPhysxActor::CkeckLockFlag(LOCK_FLAG _Flag)
{
    if (PxConcreteType::eRIGID_DYNAMIC != m_Body->getConcreteType())
        return;

    PxRigidDynamic* pBody = (PxRigidDynamic*)m_Body;
    pBody->setRigidDynamicLockFlag((PxRigidDynamicLockFlag::Enum)_Flag, !(pBody->getRigidDynamicLockFlags() & (PxRigidDynamicLockFlag::Enum)_Flag));
}

void CPhysxActor::AttachCollider(COLLIDER_DESC _Desc, PxVec3 _Scale, PxVec3 _Offset)
{
    // �������, ź�����, ��� ����
    PxMaterial* pMaterial = CPhysxMgr::GetInst()->GetPhysics()->createMaterial(_Desc.StaticFriction, _Desc.DynamicFriction, _Desc.Restitution);
    pMaterial->setRestitutionCombineMode(PxCombineMode::eMIN);
    PxShape* shape = CPhysxMgr::GetInst()->GetPhysics()->createShape(PxBoxGeometry(_Scale), *pMaterial);
    shape->setFlag(_Desc.ShapeFlag, true);

    // �浹 ���� ���̾� ����
    PxFilterData filterData;
    filterData.word0 = _Desc.FilterLayer_Self;      // ������ ���� ���̾�
    filterData.word1 = _Desc.FilterLayer_Other;     // �浹 �˻縦 �� ���̾�
    shape->setSimulationFilterData(filterData);

    // ������ ����
    shape->setLocalPose(PxTransform(_Offset));

    // ��ü�� �浹ü �߰�
    m_Body->attachShape(*shape);

    // ���� ������ ��� ��ü ���¿� �е��� ���� ������ ������Ʈ
    if (PxConcreteType::eRIGID_DYNAMIC == m_Body->getConcreteType())
    {
        PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)m_Body, m_Density);
    }
}
