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

    // 오브젝트 좌표 가져오기
    PxTransform Trans(PxVec3(GetOwner()->Transform()->GetRelativePos().x, GetOwner()->Transform()->GetRelativePos().y, GetOwner()->Transform()->GetRelativePos().z));

    if (_Type == RIGID_TYPE::DYNAMIC || _Type == RIGID_TYPE::KINEMATIC)
    {
        // 강체 생성
        PxRigidDynamic* pBody = CPhysxMgr::GetInst()->GetPhysics()->createRigidDynamic(Trans);
        m_Body = pBody;
        pBody->userData = GetOwner();   // 사용자 정의 데이터 영역에 컴포넌트를 소유한 오브젝트 주소를 저장
        pBody->setRigidDynamicLockFlags((PxRigidDynamicLockFlags)_LockFlag);  // 축 이동 및 회전 잠금 플래그
        pBody->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
        pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !_Gravity);  // 중력 적용 옵션
        pBody->setLinearVelocity(_InitVel); // 강체 적용 시 초기 속도

        if (_Type == RIGID_TYPE::KINEMATIC)
            pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // 씬에 액터 등록
        CPhysxMgr::GetInst()->AddRigidBody(GetOwner(), pBody);
    }
    else if (_Type == RIGID_TYPE::STATIC)
    {
        // 강체 생성
        PxRigidStatic* pBody = CPhysxMgr::GetInst()->GetPhysics()->createRigidStatic(Trans);
        m_Body = pBody;
        pBody->userData = GetOwner();

        // 씬에 액터 등록
        CPhysxMgr::GetInst()->AddRigidBody(GetOwner(), pBody);
    }

}

void CPhysxActor::SetRigidType(RIGID_TYPE _Type)
{
    if (m_Type == _Type)
        return;

    if (m_Type != RIGID_TYPE::STATIC && _Type != RIGID_TYPE::STATIC)
    {
        // Dynamic <-> Kinematic 인 경우 플래그만 토글시킨다
        PxRigidDynamic* pBody = (PxRigidDynamic*)m_Body;
        pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, !(pBody->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC));
        m_Type = _Type;
    }
    else
    {
        // 둘 중 하나가 Static 인 경우 기존 강체를 제거하고 동적 액터로 새로 등록한다
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
    // Scene 에 등록된 강체가 존재하는 경우, 해당 강체의 Scene 에서의 위치를 업데이트
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
    // 마찰계수, 탄성계수, 모양 설정
    PxMaterial* pMaterial = CPhysxMgr::GetInst()->GetPhysics()->createMaterial(_Desc.StaticFriction, _Desc.DynamicFriction, _Desc.Restitution);
    pMaterial->setRestitutionCombineMode(PxCombineMode::eMIN);
    PxShape* shape = CPhysxMgr::GetInst()->GetPhysics()->createShape(PxBoxGeometry(_Scale), *pMaterial);
    shape->setFlag(_Desc.ShapeFlag, true);

    // 충돌 필터 레이어 설정
    PxFilterData filterData;
    filterData.word0 = _Desc.FilterLayer_Self;      // 본인이 속한 레이어
    filterData.word1 = _Desc.FilterLayer_Other;     // 충돌 검사를 할 레이어
    shape->setSimulationFilterData(filterData);

    // 오프셋 설정
    shape->setLocalPose(PxTransform(_Offset));

    // 강체에 충돌체 추가
    m_Body->attachShape(*shape);

    // 동적 액터인 경우 강체 형태와 밀도에 따라 질량을 업데이트
    if (PxConcreteType::eRIGID_DYNAMIC == m_Body->getConcreteType())
    {
        PxRigidBodyExt::updateMassAndInertia(*(PxRigidBody*)m_Body, m_Density);
    }
}
