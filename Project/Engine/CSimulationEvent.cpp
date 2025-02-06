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
    // 충돌한 객체들의 정보를 가져옴
    for (PxU32 i = 0; i < nbPairs; ++i)
    {
        const PxContactPair& pair = pairs[i];

        // 충돌 지점 데이터 가져오기
        PxContactPairPoint contactPoints[10];  // 최대 10개의 충돌점 저장
        PxU32 contactCount = pair.extractContacts(contactPoints, 10);  // 충돌 데이터 가져오기

        for (PxU32 j = 0; j < contactCount; ++j)
        {
            PxVec3 contactPoint = contactPoints[j].position; // 충돌 지점
            PxVec3 contactNormal = contactPoints[j].normal;  // 충돌 법선 (Normal)
        }
    }
}

void CSimulationEvent::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void CSimulationEvent::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
