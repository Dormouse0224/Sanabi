#pragma once
#include "CImguiObject.h"
class CContentViewer :
    public CImguiObject
{
public:
    CContentViewer(wstring _Name);
    ~CContentViewer();
    CLONE_DISABLE(CContentViewer);

private:

public:
    virtual void Update();
    virtual void Render();
};

