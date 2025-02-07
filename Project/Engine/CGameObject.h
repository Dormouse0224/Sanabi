#pragma once

#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;


class CGameObject :
    public CEntity
{
private:
    CComponent*             m_Com[(UINT)COMPONENT_TYPE::COMPONENT_END];
    vector<CScript*>        m_vecScript;
    CRenderComponent*       m_RenderCom; // 렌더링 컴포넌트 포인터

    CGameObject*            m_Parent;   // 부모 오브젝트
    vector<CGameObject*>    m_vecChild; // 자식 오브젝트

    int                     m_LayerIdx; // 소속 레이어 인덱스, -1 : 어느 레이어에도 소속되어있지 않다.

    bool                    m_Dead;  // 오브젝트가 삭제되기 직전 상태 체크


public:
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_Com[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }
    void AddComponent(CComponent* _Component);

    bool IsDead() { return m_Dead; }

    void AddChild(CGameObject* _Child);

    CGameObject* GetParent() const { return m_Parent; }
    const vector<CGameObject*>& GetChild() const { return m_vecChild; } // 자식 오브젝트
    CGameObject* FindChild(wstring _Name);
    const vector<CScript*>& GetScripts() { return m_vecScript; }
    int GetLayerIdx() { return m_LayerIdx; }

    class CTransform* Transform() { return (CTransform*)GetComponent(COMPONENT_TYPE::TRANSFORM); }
    class CCamera* Camera() { return (CCamera*)GetComponent(COMPONENT_TYPE::CAMERA); }
    class CCollider2D* Collider2D() { return (CCollider2D*)GetComponent(COMPONENT_TYPE::COLLIDER2D); }
    class CPhysxActor* PhysxActor() { return (CPhysxActor*)GetComponent(COMPONENT_TYPE::PHYSXACTOR); }
    class CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(COMPONENT_TYPE::MESHRENDER); }
    class CSpriteRender* SpriteRender() { return (CSpriteRender*)GetComponent(COMPONENT_TYPE::SPRITERENDER); }
    class CFlipbookRender* FlipbookRender() { return (CFlipbookRender*)GetComponent(COMPONENT_TYPE::FLIPBOOKRENDER); }
    class CTileRender* TileRender() { return (CTileRender*)GetComponent(COMPONENT_TYPE::TILERENDER); }
    class CParticleRender* ParticleRender() { return (CParticleRender*)GetComponent(COMPONENT_TYPE::PARTICLERENDER); }

    void Destroy();

public:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();

public:
    CLONE(CGameObject);
public:
    CGameObject();
    ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};

