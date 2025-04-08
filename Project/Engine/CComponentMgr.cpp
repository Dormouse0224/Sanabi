#include "pch.h"
#include "CComponentMgr.h"
#include "components.h"
#include "CAssetMgr.h"

CComponentMgr::CComponentMgr()
    : m_Registry{}
    , m_ScriptInitFunc(nullptr)
{
}

CComponentMgr::~CComponentMgr()
{
}

void CComponentMgr::Init()
{
    if (m_ScriptInitFunc)
    {
        m_ScriptInitFunc();
    }

    // 스크립트 등록까지 완료되면 콘텐츠 파일들을 로드한다. (프리펩 로딩에서 스크립트가 필요한 경우가 있음)
    CAssetMgr::GetInst()->ContentLoad();
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
        // 일반 컴포넌트
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

        // 렌더 컴포넌트
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
