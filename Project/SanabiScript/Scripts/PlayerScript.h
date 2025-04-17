#pragma once
#include "Engine\CScript.h"

class CGameObject;

class PlayerScript :
    public CScript
{
public:
    PlayerScript();
    PlayerScript(const PlayerScript& _Origin);
    ~PlayerScript();
    CLONE(PlayerScript)

private:
    bool m_bInit;
    bool m_bAirborne;
    bool m_bAirborneCheck;
    bool m_bWallContact;
    bool m_bWallLeft;
    bool m_bWallContactCheck;


public:
    virtual void Tick();
    virtual int Save(fstream& _Stream) { return S_OK; }
    virtual int Load(fstream& _Stream) { return S_OK; }

    bool GetAirborne() { return m_bAirborne; }
    bool GetWallContact() { return m_bWallContact; }
    bool GetWallLeft() { return m_bWallLeft; }

    void Init();
    void ContactBegin(CGameObject* _Other, const PxContactPair pair);
    void ContacTick(CGameObject* _Other, const PxContactPair pair);
    void ContactEnd(CGameObject* _Other, const PxContactPair pair);
};

