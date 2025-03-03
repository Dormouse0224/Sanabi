#pragma once
#include "CImguiObject.h"
class CListUI :
    public CImguiObject
{
public:
    CListUI(wstring _Name);
    ~CListUI();
    CLONE_DISABLE(CListUI);

private:

public:
    virtual void Update();
    virtual void Render();
};

