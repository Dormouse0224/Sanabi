#pragma once
#include "Engine\CScript.h"

class CListUI;

class BattleGateScript :
    public CScript
{
public:
    BattleGateScript();
    BattleGateScript(const CScript& _Origin);
    ~BattleGateScript();
    CLONE(BattleGateScript);

private:
    bool m_bInit;
    bool m_bGateUsed;
    bool m_bClosed;
    bool m_bVert;

public:
    virtual void Tick();
    virtual int Save(fstream& _Stream);
    virtual int Load(fstream& _Stream);
    virtual void Render_Script();

    bool GetVert() { return m_bVert; }
    bool GetClosed() { return m_bClosed; }

    void SetVert(bool _b) { m_bVert = _b; }
    void SetClosed(bool _b) { m_bClosed = _b; }

    void Init();

    void Trigger(CGameObject* _Other);
};
