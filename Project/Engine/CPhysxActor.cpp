#include "pch.h"
#include "CPhysxActor.h"
#include "CTransform.h"

#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

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
    , m_LockFlag(0)
    , m_Gravity(false)
    , m_Density(1.f)
    , m_vecDesc{}
    , m_vecScale{}
    , m_vecOffset{}
    , m_DelayedInit(false)
{

}

CPhysxActor::CPhysxActor(const CPhysxActor& _Other)
    : CComponent(_Other)
    , m_Body(nullptr)
    , m_Type(_Other.m_Type)
    , m_LockFlag(_Other.m_LockFlag)
    , m_Gravity(_Other.m_Gravity)
    , m_Density(_Other.m_Density)
    , m_vecDesc{ _Other.m_vecDesc }
    , m_vecScale{ _Other.m_vecScale }
    , m_vecOffset{ _Other.m_vecOffset }
    , m_DelayedInit(true)
{
   
}

CPhysxActor::~CPhysxActor()
{
    CPhysxMgr::GetInst()->RemoveRigidBody(GetOwner());
}

void CPhysxActor::FinalTick()
{
    if (m_DelayedInit)
    {
        m_DelayedInit = false;
        // m_Body ����
        SetRigidBody(m_Type, m_LockFlag, m_Gravity, m_Density);

        // body �� �浹ü �߰�
        for (int i = 0; i < m_vecDesc.size(); ++i)
            AttachCollider(m_vecDesc[i], m_vecScale[i], m_vecOffset[i]);
    }
}

void CPhysxActor::SetRigidBody(RIGID_TYPE _Type, UINT _LockFlag, bool _Gravity, float _Density, PxVec3 _InitVel)
{
    m_Type = _Type;
    m_LockFlag = _LockFlag;
    m_Gravity = _Gravity;
    m_Density = _Density;

    if(m_Type == RIGID_TYPE::NONE || GetOwner() == nullptr || GetOwner()->Transform() == nullptr)
        return;

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

    if ((m_Type == RIGID_TYPE::KINEMATIC && _Type == RIGID_TYPE::DYNAMIC)
        || (m_Type == RIGID_TYPE::DYNAMIC && _Type == RIGID_TYPE::KINEMATIC))
    {
        // Dynamic <-> Kinematic �� ��� �÷��׸� ��۽�Ų��
        PxRigidDynamic* pBody = (PxRigidDynamic*)m_Body;
        pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, !(pBody->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC));
        m_Type = _Type;
    }
    else if (_Type == RIGID_TYPE::NONE)
    {
        // None Ÿ������ ���� �� ���� ��ü�� �����Ѵ�.
        CPhysxMgr::GetInst()->RemoveRigidBody(GetOwner());
    }
    else if (m_Type == RIGID_TYPE::NONE)
    {
        // None Ÿ�Կ��� ���� �����ϴ� ��� ��ü�� �����.
        SetRigidBody(_Type);

        for (int i = 0; i < m_vecDesc.size(); ++i)
        {
            AttachCollider(m_vecDesc[i], m_vecScale[i], m_vecOffset[i]);
        }
    }
    else
    {
        // �� �� �ϳ��� Static �� ��� ���� ��ü�� �����ϰ� �ű� ��ü�� ���� ����Ѵ�
        CPhysxMgr::GetInst()->RemoveRigidBody(GetOwner());
        SetRigidBody(_Type);

        for (int i = 0; i < m_vecDesc.size(); ++i)
        {
            AttachCollider(m_vecDesc[i], m_vecScale[i], m_vecOffset[i]);
        }
    }
}

void CPhysxActor::SetGravity(bool _Gravity)
{
    // ���̳��� ���Ͱ� �ƴϸ� ��� ����
    if (PxConcreteType::eRIGID_DYNAMIC != m_Body->getConcreteType())
        return;

    m_Gravity = _Gravity;
    PxRigidDynamic* pBody = (PxRigidDynamic*)m_Body;
    pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !_Gravity);
}

void CPhysxActor::SetDensity(float _Density)
{
    m_Density = _Density;
    if (m_Body->getNbShapes() > 0 && PxConcreteType::eRIGID_DYNAMIC == m_Body->getConcreteType())
    {
        PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)m_Body, m_Density);
    }
}

void CPhysxActor::SetColliderDesc(int _Idx, COLLIDER_DESC _Data)
{
    if (_Idx < m_vecShape.size())
    {
        m_vecDesc[_Idx] = _Data;

        m_vecShape[_Idx]->setFlags(_Data.ShapeFlag);

        PxFilterData filterData;
        filterData.word0 = _Data.FilterLayer_Self;
        filterData.word1 = _Data.FilterLayer_Other;
        m_vecShape[_Idx]->setSimulationFilterData(filterData);

        PxMaterial* pMaterial;
        m_vecShape[_Idx]->getMaterials(&pMaterial, 1);
        if (pMaterial)
        {
            pMaterial->setStaticFriction(_Data.StaticFriction);
            pMaterial->setDynamicFriction(_Data.DynamicFriction);
            pMaterial->setRestitution(_Data.Restitution);
        }
    }
}

void CPhysxActor::SetColliderOffset(int _Idx, PxVec3 _Data)
{
    if (_Idx < m_vecShape.size())
    {
        m_vecOffset[_Idx] = _Data;
        m_vecShape[_Idx]->setLocalPose(PxTransform(_Data));
    }
}

void CPhysxActor::AddCollider(COLLIDER_DESC _Desc, PxVec3 _Scale, PxVec3 _Offset)
{
    m_vecDesc.push_back(_Desc);
    m_vecScale.push_back(_Scale);
    m_vecOffset.push_back(_Offset);

    AttachCollider(_Desc, _Scale, _Offset);
}

void CPhysxActor::DeleteCollider(int _Idx)
{
    if (_Idx < m_vecShape.size())
    {
        m_vecDesc.erase(m_vecDesc.begin() + _Idx);
        m_vecScale.erase(m_vecScale.begin() + _Idx);
        m_vecOffset.erase(m_vecOffset.begin() + _Idx);

        m_Body->detachShape(*m_vecShape[_Idx]);
        m_vecShape[_Idx]->release();
        m_vecShape.erase(m_vecShape.begin() + _Idx);
    }

}

void CPhysxActor::UpdatePosition(Vec3 _Pos)
{
    // Scene �� ��ϵ� ��ü�� �����ϴ� ���, �ش� ��ü�� Scene ������ ��ġ�� ������Ʈ
    if (m_Body)
    {
        PxTransform Trans = PxTransform(_Pos.x, _Pos.y, _Pos.z, m_Body->getGlobalPose().q);
        if (m_Type == RIGID_TYPE::KINEMATIC)
        {
            static_cast<PxRigidDynamic*>(m_Body)->setKinematicTarget(Trans);
        }
        else
        {
            m_Body->setGlobalPose(Trans);
        }
        
    }
}

void CPhysxActor::UpdateRotation(Vec4 _RotQuat)
{
    // Scene �� ��ϵ� ��ü�� �����ϴ� ���, �ش� ��ü�� Scene ������ ������ ������Ʈ
    if (m_Body)
    {
        PxQuat Quat(_RotQuat.x, _RotQuat.y, _RotQuat.z, _RotQuat.w);
        PxTransform Trans = PxTransform(m_Body->getGlobalPose().p, Quat);
        if (m_Type == RIGID_TYPE::KINEMATIC)
        {
            static_cast<PxRigidDynamic*>(m_Body)->setKinematicTarget(Trans);
        }
        else
        {
            m_Body->setGlobalPose(Trans);
        }
    }
}

void CPhysxActor::CkeckLockFlag(LOCK_FLAG _Flag)
{
    // ���̳��� ���Ͱ� �ƴϸ� ��� ����
    if (PxConcreteType::eRIGID_DYNAMIC != m_Body->getConcreteType())
        return;

    // ��Ʈ�÷��� ���
    m_LockFlag = m_LockFlag ^ _Flag;
    PxRigidDynamic* pBody = (PxRigidDynamic*)m_Body;
    pBody->setRigidDynamicLockFlags(static_cast<PxRigidDynamicLockFlags>(m_LockFlag));
}

int CPhysxActor::Load(fstream& _Stream)
{
    if (!_Stream.is_open())
        return E_FAIL;

    // RigidBody �ۼ��� ���� �ʿ��� ������ �ҷ�����
    _Stream.read(reinterpret_cast<char*>(&m_Type), sizeof(RIGID_TYPE));
    _Stream.read(reinterpret_cast<char*>(&m_LockFlag), sizeof(UINT));
    _Stream.read(reinterpret_cast<char*>(&m_Gravity), sizeof(bool));
    _Stream.read(reinterpret_cast<char*>(&m_Density), sizeof(float));


    // RigidBody �� Collider ���� �ҷ�����
    int count = 0;
    _Stream.read(reinterpret_cast<char*>(&count), sizeof(int));
    m_vecDesc.resize(count);
    m_vecScale.resize(count);
    m_vecOffset.resize(count);
    for (int i = 0; i < count; ++i)
    {
        _Stream.read(reinterpret_cast<char*>(&m_vecDesc[i]), sizeof(COLLIDER_DESC));
        _Stream.read(reinterpret_cast<char*>(&m_vecScale[i]), sizeof(PxVec3));
        _Stream.read(reinterpret_cast<char*>(&m_vecOffset[i]), sizeof(PxVec3));
    }
    
    m_DelayedInit = true;

    return S_OK;
}

int CPhysxActor::Save(fstream& _Stream)
{
    // m_Body �� �����͸� ���� �ε� �� �ۼ��ȴ�.
    if (!_Stream.is_open())
        return E_FAIL;
    
    // RigidBody �ۼ��� ���� �ʿ��� ������ ����
    _Stream.write(reinterpret_cast<char*>(&m_Type), sizeof(RIGID_TYPE));
    _Stream.write(reinterpret_cast<char*>(&m_LockFlag), sizeof(UINT));
    _Stream.write(reinterpret_cast<char*>(&m_Gravity), sizeof(bool));
    _Stream.write(reinterpret_cast<char*>(&m_Density), sizeof(float));

    // RigidBody �� Collider ���� ����
    int count = m_vecDesc.size();
    _Stream.write(reinterpret_cast<char*>(&count), sizeof(int));
    for (int i = 0; i < count; ++i)
    {
        _Stream.write(reinterpret_cast<char*>(&m_vecDesc[i]), sizeof(COLLIDER_DESC));
        _Stream.write(reinterpret_cast<char*>(&m_vecScale[i]), sizeof(PxVec3));
        _Stream.write(reinterpret_cast<char*>(&m_vecOffset[i]), sizeof(PxVec3));
    }

    return S_OK;
}

void CPhysxActor::AttachCollider(COLLIDER_DESC _Desc, PxVec3 _Scale, PxVec3 _Offset)
{
    // �������, ź�����, ��� ����
    PxMaterial* pMaterial = CPhysxMgr::GetInst()->GetPhysics()->createMaterial(_Desc.StaticFriction, _Desc.DynamicFriction, _Desc.Restitution);
    pMaterial->setRestitutionCombineMode(PxCombineMode::eMIN);
    PxShape* shape = CPhysxMgr::GetInst()->GetPhysics()->createShape(PxBoxGeometry(_Scale), *pMaterial);
    shape->setFlags(_Desc.ShapeFlag);
    shape->setFlag(PxShapeFlag::eVISUALIZATION, true);

    // �浹 ���� ���̾� ����
    PxFilterData filterData;
    filterData.word0 = _Desc.FilterLayer_Self;      // ������ ���� ���̾�
    filterData.word1 = _Desc.FilterLayer_Other;     // �浹 �˻縦 �� ���̾�
    shape->setSimulationFilterData(filterData);

    // ������ ����
    shape->setLocalPose(PxTransform(_Offset));

    // ��ü�� �浹ü �߰�, ���� ���Ϳ� ���
    m_Body->attachShape(*shape);
    m_vecShape.push_back(shape);

    // ���� ������ ��� ��ü ���¿� �е��� ���� ������ ������Ʈ
    if (PxConcreteType::eRIGID_DYNAMIC == m_Body->getConcreteType())
    {
        PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)m_Body, m_Density);
    }
}
