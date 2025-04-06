#pragma once
#include "CRenderComponentUI.h"
class CTileRnederUI :
    public CRenderComponentUI
{
public:
    CTileRnederUI();
    ~CTileRnederUI();

private:


public:
    virtual void Update_Com();
    virtual void Render_Com();
};

