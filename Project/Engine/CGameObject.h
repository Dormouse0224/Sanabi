#pragma once

#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;


class CGameObject :
    public CEntity
{
private:
    CComponent*             m_Com[COMPONENT_END];
    vector<CScript*>        m_vecScript;
    CRenderComponent*       m_RenderCom; // 렌더링 컴포넌트 포인터

    CGameObject*            m_Parent;   // 부모 오브젝트
    vector<CGameObject*>    m_vecChild; // 자식 오브젝트

    int                     m_LayerIdx; // 소속 레이어 인덱스, -1 : 어느 레이어에도 소속되어있지 않다.

    bool                    m_Dead;  // 오브젝트가 삭제되기 직전 상태 체크


public:
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_Com[_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }
    void AddComponent(CComponent* _Component);

    bool IsDead() { return m_Dead; }

    void AddChild(CGameObject* _Child);

    CGameObject* GetParent() const { return m_Parent; }
    const vector<CGameObject*>& GetChild() const { return m_vecChild; } // 자식 오브젝트
    const vector<CScript*>& GetScripts() { return m_vecScript; }

    class CTransform* Transform() { return (CTransform*)GetComponent(TRANSFORM); }
    class CCamera* Camera() { return (CCamera*)GetComponent(CAMERA); }
    class CCollider2D* Collider2D() { return (CCollider2D*)GetComponent(COLLIDER2D); }

    class CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(MESHRENDER); }
    class CSpriteRender* SpriteRender() { return (CSpriteRender*)GetComponent(SPRITERENDER); }
    class CFlipbookRender* FlipbookRender() { return (CFlipbookRender*)GetComponent(FLIPBOOKRENDER); }
    class CTileRender* TileRender() { return (CTileRender*)GetComponent(TILERENDER); }
    class CParticleRender* ParticleRender() { return (CParticleRender*)GetComponent(PARTICLERENDER); }

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

