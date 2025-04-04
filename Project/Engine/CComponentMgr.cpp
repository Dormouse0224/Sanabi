#include "pch.h"
#include "CComponentMgr.h"
#include "components.h"

CComponentMgr::CComponentMgr()
    : m_Registry{}
    , m_InitFunc(nullptr)
{
}

CComponentMgr::~CComponentMgr()
{
}

void CComponentMgr::Register(const std::string& className, CreateFunc func)
{
    auto iter = m_Registry.find(className);
    if (iter != m_Registry.end())
        return;
    m_Registry.insert(make_pair(className, func));
}

CScript* CComponentMgr::CreateScript(const std::string& className)
{
    {
        auto iter = m_Registry.find(className);
        if (iter != m_Registry.end())
        {
            return iter->second();
        }
        return nullptr;
    }
}

CComponent* CComponentMgr::CreateComp(COMPONENT_TYPE _Type)
{
    switch (_Type)
    {
        // ÀÏ¹Ý ÄÄÆ÷³ÍÆ®
    case COMPONENT_TYPE::TRANSFORM:
        return new CTransform;
        break;
    case COMPONENT_TYPE::CAMERA:
        return new CCamera;
        break;
    case COMPONENT_TYPE::PHYSXACTOR:
        return new CPhysxActor;
        break;
    case COMPONENT_TYPE::COLLIDER3D:
        break;
    case COMPONENT_TYPE::BOUNDINGBOX:
        break;

        // ·»´õ ÄÄÆ÷³ÍÆ®
    case COMPONENT_TYPE::MESHRENDER:
        return new CMeshRender;
        break;
    case COMPONENT_TYPE::SPRITERENDER:
        return new CSpriteRender;
        break;
    case COMPONENT_TYPE::FLIPBOOKRENDER:
        return new CFlipbookRender;
        break;
    case COMPONENT_TYPE::TILERENDER:
        return new CTileRender;
        break;
    case COMPONENT_TYPE::PARTICLERENDER:
        return new CParticleRender;
        break;
    case COMPONENT_TYPE::SKYBOX:
        break;
    case COMPONENT_TYPE::DECAL:
        break;
    case COMPONENT_TYPE::LANDSCAPE:
        break;
    case COMPONENT_TYPE::COMPONENT_END:
        break;
    case COMPONENT_TYPE::SCRIPT:
        break;
    }

    return nullptr;
}
