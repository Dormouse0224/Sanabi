#pragma once
#include "Engine\CScript.h"

class CGameObject;

class PlsyerScript :
    public CScript
{
public:
    PlsyerScript();
    PlsyerScript(const PlsyerScript& _Origin);
    ~PlsyerScript();
    CLONE(PlsyerScript)

private:
    bool m_bInit;
    bool m_bAirborne;


public:
    virtual void Tick();
    virtual int Save(fstream& _Stream) { return S_OK; }
    virtual int Load(fstream& _Stream) { return S_OK; }

    void Init();
    void ContactBegin(CGameObject* _Other);
    void ContacTick(CGameObject* _Other);
    void ContactEnd(CGameObject* _Other);
};

