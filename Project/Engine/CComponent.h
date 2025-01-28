#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type) class C##Type* Type() { return m_Owner->Type(); }

class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE    m_Type;
    CGameObject* m_Owner;

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

protected:
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);

    class CRenderComponent* RenderComponent() { return m_Owner->GetRenderComponent(); }

public:
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick() = 0;

public:
    virtual CComponent* Clone() = 0;

public:
    CComponent(COMPONENT_TYPE _Type);
    ~CComponent();
    friend class CGameObject;
};

