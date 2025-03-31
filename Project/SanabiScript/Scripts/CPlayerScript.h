#pragma once
#include "Engine\CScript.h"

class CPlayerScript :
    public CScript
{
private:
    float   m_Speed;

    bool    m_Idle;
    bool    m_Run;

public:
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

public:
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

