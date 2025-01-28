#pragma once
#include "CComponent.h"

class CScript :
    public CComponent
{
private:

public:
    virtual void Tick() = 0;
    virtual void FinalTick() final {}

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) {}

public:
    virtual CScript* Clone() = 0;

public:
    CScript();
    ~CScript();
};

