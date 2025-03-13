#pragma once
#include "CAssetUI.h"
class CSpriteUI :
    public CAssetUI
{
    CSpriteUI();
    ~CSpriteUI();

private:

public:
    virtual void Update_Ast();
    virtual void Render_Ast();
};

