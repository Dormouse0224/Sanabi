#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Other);
    ~CRenderComponent();

private:
    AssetPtr<CMesh>             m_Mesh;
    AssetPtr<CMaterial>         m_DefaultMtrl;      // RenderComponent �� �⺻������ ������ �ִ� ����

    // RenderComponent ��ü ���θ� ����� ���� ����(AssetMgr �� ��Ͻ�Ű�� �ʴ´�.)
    // �������� ������ ���� ������ ���°� Play ������ ��쿡�� ��� ����
    AssetPtr<CMaterial>         m_DynamicMtrl;

    bool                        m_UsingDynamic;

public:
    void SetMesh(AssetPtr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(AssetPtr<CMaterial> _Mtrl) { m_DefaultMtrl = _Mtrl; }
    void SetDynamic(bool _UsingDynamic);

    AssetPtr<CMesh> GetMesh() { return m_Mesh; }
    // ���� RenderComponent �� Material ���� ���ο� ���� �⺻ ���� �Ǵ� ���� ������ ��ȯ�մϴ�.
    AssetPtr<CMaterial> GetMaterial();
    

    virtual void Render() = 0;

protected:
    int RenderCom_Load(fstream& _Stream);
    int RenderCom_Save(fstream& _Stream);

};

