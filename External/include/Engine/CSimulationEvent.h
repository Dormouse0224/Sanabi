#pragma once
#include "D:\repos\Sanabi\External\include\physx\PxSimulationEventCallback.h"

class CSimulationEvent :
    public PxSimulationEventCallback
{
public:
    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
    virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override {}
    virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
    virtual void onWake(PxActor** actors, PxU32 count) override {}
    virtual void onSleep(PxActor** actors, PxU32 count) override {}
};

