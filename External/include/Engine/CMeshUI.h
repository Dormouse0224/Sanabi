#pragma once
#include "CAssetUI.h"
class CMeshUI :
    public CAssetUI
{
public:
    CMeshUI();
    ~CMeshUI();

private:


public:
    virtual void Update_Ast();
    virtual void Render_Ast();
};

