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
	pShader->SetEngineAsset(true);

	CAssetMgr::GetInst()->AddAsset(L"EA_DebugShapeShader", pShader.Get());

	// ==============
	// DebugShapeMtrl
	// ==============
	AssetPtr<CMaterial> pMtrl = new CMaterial;
	pMtrl->SetShader(pShader);
	pMtrl->SetEngineAsset(true);
	CAssetMgr::GetInst()->AddAsset(L"EA_DebugShapeMtrl", pMtrl.Get());


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
	m_EditorCam->Camera()->CheckLayer(LAYER::UI);


	// ==================
	// UI Cam Object
	// ==================
	m_UICam = new CGameObject;
	m_UICam->AddComponent(new CTransform);
	m_UICam->AddComponent(new CCamera);
	m_UICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	m_UICam->Camera()->CheckLayer(LAYER::UI);

	m_BackBufferMesh = CAssetMgr::GetInst()->Load<CMesh>(L"EA_RectMesh", true);

	// BackBufferShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"HLSL\\Engine\\backbuffer.fx", "VS_BackBuffer");
	pShader->CreatePixelShader(L"HLSL\\Engine\\backbuffer.fx", "PS_BackBuffer");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_SYSTEM);
	pShader->SetEngineAsset(true);
	CAssetMgr::GetInst()->AddAsset(L"EA_BackBufferShader", pShader.Get());

	m_BackBufferMtrl = new CMaterial;
	m_BackBufferMtrl->SetName(L"EA_BackBufferMtrl");
	m_BackBufferMtrl->SetEngineAsset(true);
	m_BackBufferMtrl->SetShader(CAssetMgr::GetInst()->Load<CGraphicShader>(L"EA_BackBufferShader"));
	CAssetMgr::GetInst()->AddAsset(L"EA_BackBufferMtrl", m_BackBufferMtrl.Get());
}