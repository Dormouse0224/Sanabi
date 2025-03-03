#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    ~CRenderComponent();

private:
    AssetPtr<CMesh>              m_Mesh;
    AssetPtr<CMaterial>          m_Mtrl;

public:
    void SetMesh(AssetPtr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(AssetPtr<CMaterial> _Mtrl) { m_Mtrl = _Mtrl; }

    AssetPtr<CMesh> GetMesh() { return m_Mesh; }
    AssetPtr<CMaterial> GetMaterial() { return m_Mtrl; }

    virtual void Render() = 0;
};

