#pragma once
#include "Engine\CScript.h"

class BulletScript :
    public CScript
{
public:
    BulletScript();
    BulletScript(const BulletScript& _Origin);
    ~BulletScript();
    CLONE(BulletScript);

private:
    bool m_Init;

    bool m_Dead;
    float m_Damage;
    Vec3 m_Velocity;


public:
    virtual void Tick();
    virtual int Save(fstream& _Stream);
    virtual int Load(fstream& _Stream);
    virtual void Render_Script();

    bool GetDead() { return m_Dead; }
    float GetDamage() { return m_Damage; }
    Vec3 GetVelocity() { return m_Velocity; }

    void SetDead(bool _b) { m_Dead = _b; }
    void SetDamage(float _f) { m_Damage = _f; }
    void SetVelocity(Vec3 _v3);

    void Init();

    void TriggerBegin(CGameObject* _Other);
};

