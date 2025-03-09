#pragma once
#include "CComponentUI.h"
class CTransformUI :
    public CComponentUI
{
public:
    CTransformUI();
    ~CTransformUI();

private:


public:
    virtual void Update();
    virtual void Render_Com();

};

