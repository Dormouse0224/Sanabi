#pragma once
#include "CAssetUI.h"
class CComputeShaderUI :
    public CAssetUI
{
    CComputeShaderUI();
    ~CComputeShaderUI();

private:

public:
    virtual void Update_Ast();
    virtual void Render_Ast();
};

