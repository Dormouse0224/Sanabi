#pragma once
#include "CImguiObject.h"

class CGameObject;

class CInspector :
    public CImguiObject
{
public:
    CInspector(wstring _Name);
    ~CInspector();
    CLONE_DISABLE(CInspector);

private:
    CGameObject* m_TargetObject;



public:
    virtual void Update();
    virtual void Render();

    CGameObject* GetTargetObject() { return m_TargetObject; }

    void SetTargetObject(CGameObject* _Obj) { m_TargetObject = _Obj; }
};

