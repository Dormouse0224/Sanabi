#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
public:
    CCollider2D();
    ~CCollider2D();
    CLONE(CCollider2D);

private:
    Vec3    m_Scale;
    Vec3    m_Offset;

    int     m_OverlapCount;

    Matrix  m_matColWorld;
    bool    m_IndependentScale;

    bool    m_NotifyParent;

public:
    void SetScale(Vec2 _Scale) { m_Scale.x = _Scale.x; m_Scale.y = _Scale.y; }
    void SetOffset(Vec2 _Offset) { m_Offset.x = _Offset.x; m_Offset.y = _Offset.y; }

    Vec2 GetScale() { return Vec2(m_Scale.x, m_Scale.y); }
    Vec2 GetOffset() { return Vec2(m_Offset.x, m_Offset.y); }
    const Matrix& GetWorldMat() { return m_matColWorld; }

    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set; }
    bool GetIndependentScale() { return m_IndependentScale; }

    void SetNotifyParent(bool _Notify) { m_NotifyParent = _Notify; }

public:
    virtual void FinalTick() override;


public:
    void BeginOverlap(CCollider2D* _Other);
    void Overlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);
    virtual int Save(fstream& _Stream) { return S_OK; }
    virtual int Load(fstream& _Stream) { return S_OK; }


private:
    void NotifyBeginOverlap(CCollider2D* _Other);
    void NotifyOverlap(CCollider2D* _Other);
    void NotifyEndOverlap(CCollider2D* _Other);

};

