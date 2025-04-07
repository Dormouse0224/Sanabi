#include "pch.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CAssetMgr.h"
#include "assets.h"
#include "CEditorCamScript.h"

void CRenderMgr::Init()
{
	// ================
	// DebugShapeShader
	// ================
	AssetPtr<CGraphicShader> pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\Engine\\debug_shape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"HLSL\\Engine\\debug_shape.fx", "PS_DebugShape");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	CAssetMgr::GetInst()->AddAsset(L"DebugShapeShader", pShader.Get());

	// ==============
	// DebugShapeMtrl
	// ==============
	AssetPtr<CMaterial> pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(L"DebugShapeMtrl", pMtrl.Get());


	// ==================
	// DebugRender Object
	// ==================
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(pMtrl);


	// ==================
	// Editor Cam Object
	// ==================
	m_EditorCam = new CGameObject;
	m_EditorCam->AddComponent(new CTransform);
	m_EditorCam->AddComponent(new CCamera);
	m_EditorCam->AddComponent(new CEditorCamScript);
	m_EditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	m_EditorCam->Camera()->CheckLayerAll();
}