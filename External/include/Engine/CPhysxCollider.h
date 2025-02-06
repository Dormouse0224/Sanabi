#pragma once
#include "CComponent.h"
#include "CPhysxMgr.h"

class CPhysxCollider :
    public CComponent
{
public:
    CPhysxCollider();
    ~CPhysxCollider();

private:

public:
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick() {};
    CLONE(CPhysxCollider);

public:
    void AddDynamicActor(CGameObject* _Object, PxVec3 _Scale = PxVec3(1.f), PxVec3 _Offset = PxVec3(0.f));
    void AddStaticActor(CGameObject* _Object, PxVec3 _Scale = PxVec3(1.f), PxVec3 _Offset = PxVec3(0.f));


};

