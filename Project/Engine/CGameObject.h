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
    CRenderComponent*       m_RenderCom; // ������ ������Ʈ ������

    CGameObject*            m_Parent;   // �θ� ������Ʈ
    vector<CGameObject*>    m_vecChild; // �ڽ� ������Ʈ

    int                     m_LayerIdx; // �Ҽ� ���̾� �ε���, -1 : ��� ���̾�� �ҼӵǾ����� �ʴ�.

    bool                    m_Dead;  // ������Ʈ�� �����Ǳ� ���� ���� üũ


public:
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_Com[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }
    void AddComponent(CComponent* _Component);

    bool IsDead() { return m_Dead; }

    void AddChild(CGameObject* _Child);

    CGameObject* GetParent() const { return m_Parent; }
    const vector<CGameObject*>& GetChild() const { return m_vecChild; } // �ڽ� ������Ʈ
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

