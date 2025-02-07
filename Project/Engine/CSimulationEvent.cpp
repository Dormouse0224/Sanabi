#include "pch.h"
#include "CSimulationEvent.h"

void CSimulationEvent::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

void CSimulationEvent::onWake(PxActor** actors, PxU32 count)
{
}

void CSimulationEvent::onSleep(PxActor** actors, PxU32 count)
{
}

void CSimulationEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    pairHeader.actors[0]->userData;
}

void CSimulationEvent::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void CSimulationEvent::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}

