#include "pch.h"
#include "CSimulationEvent.h"

void CSimulationEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    // 충돌한 객체들의 정보를 가져옴
    for (PxU32 i = 0; i < nbPairs; ++i)
    {
        const PxContactPair& pair = pairs[i];

        // 충돌된 객체들에 대한 처리
        if (pair.flags & PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
        {

        }
    }
}
