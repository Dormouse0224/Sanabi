#include "pch.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CAssetMgr.h"
#include "assets.h"

void CRenderMgr::Init()
{
	// ================
	// DebugShapeShader
	// ================
	Ptr<CGraphicShader> pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\debug_shape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"HLSL\\debug_shape.fx", "PS_DebugShape");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	CAssetMgr::GetInst()->AddAsset(L"DebugShapeShader", pShader.Get());

	// ==============
	// DebugShapeMtrl
	// ==============
	Ptr<CMaterial> pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(L"DebugShapeMtrl", pMtrl.Get());


	// ==================
	// DebugRender Object
	// ==================
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(pMtrl);
}