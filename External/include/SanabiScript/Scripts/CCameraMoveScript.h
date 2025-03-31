#pragma once
#include "Engine\CScript.h"
class CCameraMoveScript :
    public CScript
{
private:
    float   m_CamSpeed;

public:
    virtual void Tick() override;

private:
    void Move_Perspective();
    void Move_OrthoGraphic();



public:
    CLONE(CCameraMoveScript);
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

