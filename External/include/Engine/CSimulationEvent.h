#pragma once
#include <physx/PxSimulationEventCallback.h>


// Extracting Contact Information

class CSimulationEvent :
    public PxSimulationEventCallback
{
public:


    virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;


    virtual void onWake(PxActor** actors, PxU32 count) override;


    virtual void onSleep(PxActor** actors, PxU32 count) override;

    // �浹ü�� �浹 �� ȣ��
    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

    // ����ü�� �⵿ �� ȣ��
    virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;


    virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

};

