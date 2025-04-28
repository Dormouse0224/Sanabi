#pragma once
#include "Engine\CScript.h"

class FarBackgroundScript :
    public CScript
{
public:
    FarBackgroundScript();
    FarBackgroundScript(const FarBackgroundScript& _Origin);
    ~FarBackgroundScript();
    CLONE(FarBackgroundScript);

private:
    float m_Scale;
    Vec2 m_Offset;

public:
    virtual void Tick();
    virtual int Save(fstream& _Stream);
    virtual int Load(fstream& _Stream);
    virtual void Render_Script();
};

