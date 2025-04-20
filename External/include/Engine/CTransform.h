#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
public:
    CTransform();
    ~CTransform();
    CLONE(CTransform);

private:
    Vec3    m_RelativePos;  // ��ġ
    Vec3    m_RelativeScale;// ũ��
    Vec4    m_RelativeRot;  // ȸ��(Quaternion)

    Vec3    m_RelativeDir[(UINT)DIR::END]; // ������Ʈ�� ��������
    Vec3    m_WorldDir[(UINT)DIR::END]; // ������Ʈ�� ��������

    Matrix  m_matWorld;     // ���� ��ȯ���

    // �ڽ� ������Ʈ�϶� �θ��� ũ�⿡ ������ �����ʴ� ������ ũ�⸦ ������.
    bool    m_IndependentScale;
    bool    m_IndependentRot;
    bool    m_IndependentTrans;


public:
    virtual void FinalTick() override;
    void Binding();

public:
    void SetRelativePos(Vec3 _Pos);
    void SetRelativePos(float _x, float _y, float _z) { SetRelativePos(Vec3(_x, _y, _z)); }

    void SetRelativeScale(Vec3 _Scale) { m_RelativeScale = _Scale; }
    void SetRelativeScale(float _x, float _y, float _z) { m_RelativeScale = Vec3(_x, _y, _z); }

    void SetRelativeRot(Vec4 _RotationQuat);
    void SetRelativeRot(Vec3 _RotationDeg);
    void SetRelativeRot(float _xDeg, float _yDeg, float _zDeg);
    //void SetRelativeRotation(PxQuat _Rotation);

    void SetIndependentScale(bool _Scale) { m_IndependentScale = _Scale; }
    void SetIndependentRot(bool _Rot) { m_IndependentRot = _Rot; }
    void SetIndependentTrans(bool _Trans) { m_IndependentTrans = _Trans; }

    Vec3 GetRelativePos() { return m_RelativePos; }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }

    Vec3 GetRelativeScale() { return m_RelativeScale; }
    Vec3 GetWorldScale();
    Vec3 GetWorldRot();
    Vec3 GetWorldTrans();

    Vec3 GetRelativeRot();
    //Vec3 GetWorldRotation();

    Vec3 GetRelativeDir(DIR _DirType) { return m_RelativeDir[(UINT)_DirType]; }
    Vec3 GetWorldDir(DIR _DirType) { return m_WorldDir[(UINT)_DirType]; }

    bool GetIndependentScale() { return m_IndependentScale; }
    bool GetIndependentRot() { return m_IndependentRot; }
    bool GetIndependentTrans() { return m_IndependentTrans; }

    const Matrix& GetWorldMat() { return m_matWorld; }
    void SetWorldMat(const Matrix& _Mat) { m_matWorld = _Mat; }

    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;

};

