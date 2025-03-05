#pragma once
#include "CImguiObject.h"

class CGameObject;

class CComponentUI :
    public CImguiObject
{
public:
    CComponentUI(COMPONENT_TYPE _Type);
    ~CComponentUI();
    CLONE_DISABLE(CComponentUI);

protected:
    const COMPONENT_TYPE    m_Type;
    CGameObject*            m_Target;

public:
    CGameObject* GetTarget() { return m_Target; }

    void SetTarget(CGameObject* _Obj) { m_Target = _Obj; }
};

