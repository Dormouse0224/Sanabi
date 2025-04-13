#pragma once
#include "CComponentUI.h"
class CUIComUI :
    public CComponentUI
{
public:
    CUIComUI();
    ~CUIComUI();

private:


public:
    virtual void Update_Com();
    virtual void Render_Com();
};
