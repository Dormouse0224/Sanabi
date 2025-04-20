#pragma once
#include "Engine\CScript.h"

class CGameObject;
class CLevel;

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
    bool m_Damaged;

    bool m_Immune;
    int m_HP;
    //CLevel* m_SavePoint;

public:
    virtual void Tick();
    virtual int Save(fstream& _Stream) { return S_OK; }
    virtual int Load(fstream& _Stream) { return S_OK; }

    bool GetAirborne() { return m_bAirborne; }
    bool GetWallContact() { return m_bWallContact; }
    bool GetWallLeft() { return m_bWallLeft; }
    bool GetDamaged() { return m_Damaged; }
    bool GetImmune() { return m_Immune; }
    int GetHP() { return m_HP; }
    //CLevel* GetSavePoint() { return m_SavePoint; }

    void SetDamaged(bool _b) { m_Damaged = _b; }
    void SetImmune(bool _b) { m_Immune = _b; }
    //void SetSavePoint(CLevel* _SavePoint) { m_SavePoint = _SavePoint; }

    void Init();
    void ContactBegin(CGameObject* _Other, const PxContactPair pair);
    void ContacTick(CGameObject* _Other, const PxContactPair pair);
    void ContactEnd(CGameObject* _Other, const PxContactPair pair);

    void GiveDamage(int _Dmg);
};

