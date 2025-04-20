#pragma once
#include "Engine\CScript.h"

class MonsterScript :
    public CScript
{
public:
    MonsterScript();
    MonsterScript(const MonsterScript& _Origin);
    ~MonsterScript();
    CLONE(MonsterScript);

private:
    bool m_bWakeUp;
    bool m_ExcHolded;
    bool m_Dead;


public:
    virtual void Tick();
    virtual int Save(fstream& _Stream);
    virtual int Load(fstream& _Stream);

    bool GetWakeUp() { return m_bWakeUp; }
    bool GetExcHolded() { return m_ExcHolded; }
    bool GetDead() { return m_Dead; }

    void SetWakeUp(bool _b) { m_bWakeUp = _b; }
    void SetExcHolded(bool _b) { m_ExcHolded = _b; }
    void SetDead(bool _b) { m_Dead = _b; }
};
