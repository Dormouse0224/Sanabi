#pragma once
#include "CComponent.h"
#include "CRenderComponentUI.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
    friend class CRenderComponentUI;
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
    // ������ ������ ���� ������� �ٸ� ������Ʈ�� ������ ���� �ʵ��� �ش� ������Ʈ ������ ���� ������ �����մϴ�.
    // �̹� ���� ������ ������ ���� �������� ������, Level �� Play ����� ���� Ȱ��ȭ�˴ϴ�.
    void SetDynamic(bool _UsingDynamic);

    AssetPtr<CMesh> GetMesh() { return m_Mesh; }
    // ���� RenderComponent �� ���� ���� ��� ���ο� ���� �⺻ ���� �Ǵ� ���� ������ ��ȯ�մϴ�.
    AssetPtr<CMaterial> GetMaterial();
    

    virtual void Render() = 0;

protected:
    int RenderCom_Load(fstream& _Stream);
    int RenderCom_Save(fstream& _Stream);

};

