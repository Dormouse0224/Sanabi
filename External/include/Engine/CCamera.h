#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
    friend class CRenderMgr;
public:
    CCamera();
    CCamera(const CCamera& _Origin);
    ~CCamera();
    CLONE(CCamera);

private:
    PROJ_TYPE   m_ProjType;     // ���� ���
    float       m_ViewX;        // ī�޶� ���� ���α���
    float       m_ViewY;        // ī�޶� ���� ���α���
    float       m_FOV;          // �þ߰�(FieldOfView)
    float       m_Far;          // �ִ� �þ߰Ÿ�

    Matrix      m_matView;      // View ���
    Matrix      m_matProj;      // �������

    int         m_Priority;     // ī�޶� �켱����, -1 : �̵�� ī�޶�, 0 : ���� ī�޶�, 1 ~> : ���� ī�޶�
    bool        m_Registered;
    float       m_Zoom;

    UINT        m_LayerCheck;   // ī�޶� �������� ���̾� ��Ʈ����


    // ��ü �з� �뵵
    vector<CGameObject*>                m_vecOpaque;
    vector<CGameObject*>                m_vecMasked;
    vector<pair<float, CGameObject*>>   m_vecTransparent;
    vector<CGameObject*>                m_vecPostprocess;
    vector<CGameObject*>                m_vecUI;



public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    void SetViewX(float _Width) { m_ViewX = _Width; }
    void SetViewY(float _Height) { m_ViewY = _Height; }
    void SetFOV(float _FOV) { m_FOV = (_FOV / 180.f) * XM_PI; }
    void SetFar(float _Far) { m_Far = _Far; }
    void SetPriority(int _Priority);
    void SetZoom(float _Zoom) { m_Zoom = _Zoom; }

    PROJ_TYPE GetProjType() { return m_ProjType; }
    float GetViewX() { return m_ViewX; }
    float GetViewY() { return m_ViewY; }
    float GetFOV() { return (m_FOV / XM_PI) * 180.f; }
    float GetFar() { return m_Far; }
    int GetPriority() { return m_Priority; }
    bool GetLayerState(LAYER _LayerIdx) { return m_LayerCheck & (1 << (UINT)_LayerIdx); }
    float GetZoom() { return m_Zoom; }

    void CheckLayer(LAYER _LayerIdx) { m_LayerCheck ^= (1 << (UINT)_LayerIdx); }
    void CheckLayerAll() { m_LayerCheck = 0xffffffff; }
    void CheckLayerClear() { m_LayerCheck = 0; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }



private:
    void SortObject();

public:
    virtual void FinalTick() override;
    void SetMatrix();
    void Render();

    /// <summary>
    /// Render Manager ���� �������� �ʴ� ������ ī�޶� ��ü�� ������ �Լ��Դϴ�.
    /// </summary>
    /// <param name="_vecObj">������ ��� ������Ʈ ����</param>
    void Direct_Render(const vector<CGameObject*>& _vecObj);

    virtual int Load(fstream& _Stream) override;
    virtual int Save(fstream& _Stream) override;
};