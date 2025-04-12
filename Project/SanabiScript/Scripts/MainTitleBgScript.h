#pragma once
#include "Engine/CScript.h"

class MainTitleBgScript :
    public CScript
{
public:
    MainTitleBgScript();
    MainTitleBgScript(const MainTitleBgScript& _Origin);
    ~MainTitleBgScript();
    CLONE(MainTitleBgScript)

private:
    float   m_Timer;

public:
    virtual void Tick();
    virtual int Save(fstream& _Stream) { return S_OK; }
    virtual int Load(fstream& _Stream) { return S_OK; }

};

