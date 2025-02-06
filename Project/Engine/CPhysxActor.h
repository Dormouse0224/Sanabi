#pragma once
#include "CComponent.h"
#include "CPhysxMgr.h"

struct COLLIDER_DESC
{
    PxShapeFlag::Enum   ShapeFlag;
    PxReal              StaticFriction      = 0.f;
    PxReal              DynamicFriction     = 0.f;
    PxReal              Restitution         = 0.f;
    PxU32               FilterLayer_Self;
    PxU32               FilterLayer_Other;
};

enum class RIGID_TYPE
{
    KINEMATIC,
    DYNAMIC,
    STATIC,
};

class CPhysxActor :
    public CComponent
{
public:
    CPhysxActor();
    ~CPhysxActor();

private:

public:
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick() {};
    CLONE(CPhysxActor);

public:
    void SetRigidBody(RIGID_TYPE _Type, PxReal _Mass = 1.f);
    void AddCollider(COLLIDER_DESC _desc, PxVec3 _Scale = PxVec3(1.f), PxVec3 _Offset = PxVec3(0.f));

};

