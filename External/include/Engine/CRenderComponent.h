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
    AssetPtr<CMaterial>         m_DefaultMtrl;      // RenderComponent 가 기본적으로 가지고 있는 재질

    // RenderComponent 객체 본인만 사용할 전용 재질(AssetMgr 에 등록시키지 않는다.)
    // 동적재질 생성은 현재 레벨의 상태가 Play 상태일 경우에만 사용 가능
    AssetPtr<CMaterial>         m_DynamicMtrl;

    bool                        m_UsingDynamic;

public:
    void SetMesh(AssetPtr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(AssetPtr<CMaterial> _Mtrl) { m_DefaultMtrl = _Mtrl; }
    void SetDynamic(bool _UsingDynamic);

    AssetPtr<CMesh> GetMesh() { return m_Mesh; }
    // 현재 RenderComponent 의 Material 상태 여부에 따라 기본 재질 또는 동적 재질을 반환합니다.
    AssetPtr<CMaterial> GetMaterial();
    

    virtual void Render() = 0;

protected:
    int RenderCom_Load(fstream& _Stream);
    int RenderCom_Save(fstream& _Stream);

};

