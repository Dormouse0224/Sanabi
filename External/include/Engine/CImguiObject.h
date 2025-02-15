#pragma once
#include "CEntity.h"
class CImguiObject :
    public CEntity
{
public:
    CImguiObject();
    ~CImguiObject();
private:
    CImguiObject*           m_Parent;
    vector<CImguiObject*>   m_vecChild;

public:
    void Update_Progress();
    void Render_Progress();

    virtual void Update() = 0;
    virtual void Render() = 0;

    void AddChild(CImguiObject* _ImguiObj) { m_vecChild.push_back(_ImguiObj); }
    CImguiObject* GetParent() { return m_Parent; }

};

