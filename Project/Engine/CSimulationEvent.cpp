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
    // �浹�� ��ü���� ������ ������
    for (PxU32 i = 0; i < nbPairs; ++i)
    {
        const PxContactPair& pair = pairs[i];

        // �浹 ���� ������ ��������
        PxContactPairPoint contactPoints[10];  // �ִ� 10���� �浹�� ����
        PxU32 contactCount = pair.extractContacts(contactPoints, 10);  // �浹 ������ ��������

        for (PxU32 j = 0; j < contactCount; ++j)
        {
            PxVec3 contactPoint = contactPoints[j].position; // �浹 ����
            PxVec3 contactNormal = contactPoints[j].normal;  // �浹 ���� (Normal)
        }
    }
}

void CSimulationEvent::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void CSimulationEvent::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
