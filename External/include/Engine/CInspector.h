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
};

