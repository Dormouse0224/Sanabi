#pragma once
#include "Engine\CScript.h"

class CGameObject;

class MainTitle_StartBtn_Script :
    public CScript
{
public:
    MainTitle_StartBtn_Script();
    MainTitle_StartBtn_Script(const MainTitle_StartBtn_Script& _Origin);
    ~MainTitle_StartBtn_Script();
    CLONE(MainTitle_StartBtn_Script);
private:
    bool m_Init;

public:
    virtual void Tick();
    virtual int Save(fstream& _Stream);
    virtual int Load(fstream& _Stream);

public:
    static void ClickEvent(CGameObject* _Owner);
};