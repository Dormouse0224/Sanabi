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

    NONE,
};

enum LOCK_FLAG
{
    NONE        = 0,
    LINEAR_X    = (1<<0),
    LINEAR_Y    = (1<<1),
    LINEAR_Z    = (1<<2),
    ANGULAR_X   = (1<<3),
    ANGULAR_Y   = (1<<4),
    ANGULAR_Z   = (1<<5),

    LINEAR_ALL  = LINEAR_X | LINEAR_Y | LINEAR_Z,
    ANGULAR_ALL = ANGULAR_X | ANGULAR_Y | ANGULAR_Z,
};

extern wstring LOCK_FLAG_WSTR[];

class CPhysxActor :
    public CComponent
{
public:
    CPhysxActor();
    ~CPhysxActor();
    CLONE(CPhysxActor);

private:
    PxRigidActor*           m_Body;
    RIGID_TYPE              m_Type;

    float                   m_Density;

    vector<COLLIDER_DESC>   m_vecDesc;
    vector<PxVec3>          m_vecScale;
    vector<PxVec3>          m_vecOffset;

public:
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick();

public:
    PxRigidActor* GetRigidBody() { return m_Body; }
    RIGID_TYPE GetRigidType() { return m_Type; }
    float GetDensity() { return m_Density; }

    void SetRigidBody(RIGID_TYPE _Type, UINT _LockFlag = 0, bool _Gravity = false, float _Density = 1.f, PxVec3 _InitVel = PxVec3(0.f));
    void SetRigidType(RIGID_TYPE _Type);
    void SetDensity(float _Density);

    void AddCollider(COLLIDER_DESC _desc, PxVec3 _Scale = PxVec3(1.f), PxVec3 _Offset = PxVec3(0.f));
    void UpdatePosition(Vec3 _Pos);
    void CkeckLockFlag(LOCK_FLAG _Flag);

private:
    void AttachCollider(COLLIDER_DESC _desc, PxVec3 _Scale, PxVec3 _Offset);
};

