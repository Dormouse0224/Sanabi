#pragma once
#include "CAssetUI.h"
class CTextureUI :
    public CAssetUI
{
    CTextureUI();
    ~CTextureUI();

private:

public:
    virtual void Update_Ast();
    virtual void Render_Ast();
};

