#include "pch.h"
#include "CSimulationEvent.h"

void CSimulationEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    // �浹�� ��ü���� ������ ������
    for (PxU32 i = 0; i < nbPairs; ++i)
    {
        const PxContactPair& pair = pairs[i];

        // �浹�� ��ü�鿡 ���� ó��
        if (pair.flags & PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
        {

        }
    }
}
