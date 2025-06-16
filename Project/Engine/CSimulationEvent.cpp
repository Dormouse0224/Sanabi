#include "pch.h"
#include "CSimulationEvent.h"

#include "CGameObject.h"
#include "CPhysxActor.h"
#include "CLevelMgr.h"

// 	�������� ����(����)�� �������� �� ȣ��� (��: hinge�� �η��� ��).
void CSimulationEvent::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
}

// ��ü�� ���� ���¿��� ��� �� ȣ���.
void CSimulationEvent::onWake(PxActor** actors, PxU32 count)
{
}

// ��ü�� �ٽ� ���� ���·� �� �� ȣ���.
void CSimulationEvent::onSleep(PxActor** actors, PxU32 count)
{
}

/// <summary>
/// �� ���� ���� ��ü�� �浹���� �� ȣ��Ǹ�, �浹�� ��ü ������ �浹 �� ������ ���޵˴ϴ�.
/// </summary>
/// <param name="pairHeader">�浹�� �� ���� Actor(��ü)�� ���� �⺻ ������ �����ϴ� ����ü</param>
/// <param name="pairs">�浹�� ���� Shape(�浹��)���� ���� �迭</param>
/// <param name="nbPairs">�̹� onContact ȣ�⿡�� ���޵� pairs �迭�� ����(�浹�� Shape ���� ����)</param>
void CSimulationEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];

        CGameObject* go_0 = reinterpret_cast<CGameObject*>(pairHeader.actors[0]->userData);
        CGameObject* go_1 = reinterpret_cast<CGameObject*>(pairHeader.actors[1]->userData);
        if (go_0->IsDead() || go_1->IsDead())
            return;

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            // �浹�� ���۵Ǿ��� ��
            go_0->PhysxActor()->ContactBegin(go_1, cp);
            go_1->PhysxActor()->ContactBegin(go_0, cp);
        }

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            // �浹�� ��ӵǰ� ���� �� (�� �����Ӹ��� ȣ���)
            go_0->PhysxActor()->ContactTick(go_1, cp);
            go_1->PhysxActor()->ContactTick(go_0, cp);
        }

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            // �浹�� ������ ��
            go_0->PhysxActor()->ContactEnd(go_1, cp);
            go_1->PhysxActor()->ContactEnd(go_0, cp);
        }
    }
}

// Trigger ������ Collider�� �ٸ� ��ü�� �����ų� ���� �� ȣ���.
void CSimulationEvent::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    for (PxU32 i = 0; i < count; ++i)
    {
        const PxTriggerPair& pair = pairs[i];

        // �� actor ��� ��ȿ�ؾ� �ϰ�, userData�� �����Ǿ� �־�� ��
        if (!pair.triggerActor || !pair.otherActor)
            continue;

        CGameObject* triggerGO = reinterpret_cast<CGameObject*>(pair.triggerActor->userData);
        CGameObject* otherGO = reinterpret_cast<CGameObject*>(pair.otherActor->userData);

        if (!triggerGO || !otherGO)
            continue;

        if (triggerGO->IsDead() || otherGO->IsDead())
            continue;

        if (pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            // Ʈ���ſ� ���� ��
            triggerGO->PhysxActor()->TriggerBegin(otherGO);
            otherGO->PhysxActor()->TriggerBegin(triggerGO);
        }

        if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            // Ʈ���ſ��� ������ ��
            triggerGO->PhysxActor()->TriggerEnd(otherGO);
            otherGO->PhysxActor()->TriggerEnd(triggerGO);
        }
    }
}

// continuous collision detection ���� ������ ������Ʈ�� �� ȣ���.
void CSimulationEvent::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}

