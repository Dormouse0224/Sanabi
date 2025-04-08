#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"
#include "assets.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();

	CreateEngineSprite();

	CreateEngineGraphicShader();

	CreateEngineComputeShader();

	CreateEngineMaterial();
}

void CAssetMgr::CreateEngineMesh()
{
	// =========
	// PointMesh
	// =========
	AssetPtr<CMesh> pMesh = new CMesh;
	Vtx vPoint;
	vPoint.vPos = Vec3(0.f, 0.f, 0.f);
	vPoint.vUV = Vec2(0.f, 0.f);
	vPoint.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	UINT Idx = 0;
	pMesh->Create(&vPoint, 1, &Idx, 1);
	AddAsset(L"PointMesh", pMesh.Get());


	// ========
	// RectMesh
	// ========	
	// 0 -- 1
	// | \  |
	// 3 -- 2
	pMesh = new CMesh;
	pMesh->SetName(L"RectMesh");

	Vtx arrVtx[4] = {};
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// Index
	UINT arrIdx[6] = { 0, 1, 2, 0, 2, 3 };

	pMesh->Create(arrVtx, 4, arrIdx, 6);
	AddAsset(L"RectMesh", pMesh.Get());


	// ==============
	// RectMesh_Debug
	// ==============	
	// 0 -- 1
	// | \  |
	// 3 -- 2
	pMesh = new CMesh;
	pMesh->SetName(L"RectMesh_Debug");

	arrIdx[0] = 0; arrIdx[1] = 1; arrIdx[2] = 2; arrIdx[3] = 3; arrIdx[4] = 0;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	AddAsset(L"RectMesh_Debug", pMesh.Get());

	// ==========
	// CircleMesh
	// ==========
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;


	pMesh = new CMesh;
	pMesh->SetName(L"CircleMesh");

	// Circle 중심점
	Vtx v;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	int Slice = 40;
	float Radius = 0.5f;
	float Angle = 0.f;

	// Circle 테두리 정점
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);

		Angle += XM_2PI / (float)Slice;
	}

	// Circle 인덱스
	for (int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh", pMesh.Get());
	vecIdx.clear();

	// ================
	// CircleMesh_Debug
	// ================
	pMesh = new CMesh;
	pMesh->SetName(L"CircleMesh_Debug");

	for (size_t i = 0; i < vecVtx.size() - 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh_Debug", pMesh.Get());
	vecVtx.clear();
	vecIdx.clear();
}

void CAssetMgr::CreateEngineGraphicShader()
{
	// 표준 쉐이더 생성
	AssetPtr<CGraphicShader>	pShader = nullptr;

	// ===========
	// Std2DShader
	// ===========
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\Engine\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"HLSL\\Engine\\std2d.fx", "PS_Std2D");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetTexData(TEX_0, "Main Texture");

	AddAsset(L"Std2DShader", pShader.Get());


	// =====================
	// Std2DAlphaBlendShader
	// =====================
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\Engine\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"HLSL\\Engine\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetTexData(TEX_0, "Alpha Texture");

	AddAsset(L"Std2DAlphaBlendShader", pShader.Get());


	// FlipbookShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"HLSL\\Engine\\flipbook.fx", "VS_Flipbook");
	pShader->CreatePixelShader(L"HLSL\\Engine\\flipbook.fx", "PS_Flipbook");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetTexData(TEX_0, "Atlas Texture");
	pShader->SetConstData(INT_0, "Atlas Tex Flag");
	pShader->SetConstData(INT_1, "Single Tex Flag");
	pShader->SetConstData(VEC2_0, "LeftTop");
	pShader->SetConstData(VEC2_1, "Slice");
	pShader->SetConstData(VEC2_2, "Background");
	pShader->SetConstData(VEC2_3, "Offset");
	CAssetMgr::GetInst()->AddAsset(L"FlipbookShader", pShader.Get());

	// 파티클 렌더링 쉐이더 및 재질
	pShader = new CGraphicShader;
	pShader->SetName(L"ParticleRenderShader");
	pShader->CreateVertexShader(L"HLSL\\Engine\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"HLSL\\Engine\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"HLSL\\Engine\\particle.fx", "PS_Particle");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	CAssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	// SpriteShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"HLSL\\Engine\\sprite.fx", "VS_Sprite");
	pShader->CreatePixelShader(L"HLSL\\Engine\\sprite.fx", "PS_Sprite");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	CAssetMgr::GetInst()->AddAsset(L"SpriteShader", pShader.Get());


	// TileRenderShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"HLSL\\Engine\\tilerender.fx", "VS_TileRender");
	pShader->CreatePixelShader(L"HLSL\\Engine\\tilerender.fx", "PS_TileRender");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	CAssetMgr::GetInst()->AddAsset(L"TileRenderShader", pShader.Get());

}

void CAssetMgr::CreateEngineComputeShader()
{
}

void CAssetMgr::CreateEngineMaterial()
{
	// Std2DMtrl
	AssetPtr<CMaterial> pMtrl = new CMaterial;
	pMtrl->SetShader(Load<CGraphicShader>(L"Std2DShader", true));
	AddAsset(L"Std2DMtrl", pMtrl.Get());

	pMtrl = new CMaterial;
	pMtrl->SetShader(Load<CGraphicShader>(L"Std2DShader", true));
	pMtrl->SetTexParam(TEX_0, Load<CTexture2D>(L"Texture2D\\Spr_SNB_IdleRand1 (1).png"));
	AddAsset(L"SNB_Mtrl", pMtrl.Get());

	pMtrl = new CMaterial;
	pMtrl->SetShader(Load<CGraphicShader>(L"Std2DShader", true));
	pMtrl->SetTexParam(TEX_0, Load<CTexture2D>(L"Texture2D\\Spr_SNBArm_IdleRand1 (1).png"));
	AddAsset(L"SNBArm_Mtrl", pMtrl.Get());

	pMtrl = new CMaterial;
	pMtrl->SetShader(Load<CGraphicShader>(L"Std2DShader", true));
	pMtrl->SetTexParam(TEX_0, Load<CTexture2D>(L"Texture2D\\Spr_CargoPlatform_Idle (lp) (1).png"));
	AddAsset(L"CargoPlatform_Mtrl", pMtrl.Get());

	pMtrl = new CMaterial;
	pMtrl->SetShader(Load<CGraphicShader>(L"Std2DShader", true));
	pMtrl->SetTexParam(TEX_0, Load<CTexture2D>(L"Texture2D\\Chap2_Mid_A01.png"));
	AddAsset(L"Background_Mtrl", pMtrl.Get());

	// FlipbookMtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(CAssetMgr::GetInst()->Load<CGraphicShader>(L"FlipbookShader", true));
	CAssetMgr::GetInst()->AddAsset(L"FlipbookMtrl", pMtrl.Get());

	pMtrl = new CMaterial;
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(CAssetMgr::GetInst()->Load<CGraphicShader>(L"ParticleRenderShader", true));
	CAssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());

	// SpriteMtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(CAssetMgr::GetInst()->Load<CGraphicShader>(L"SpriteShader", true));
	CAssetMgr::GetInst()->AddAsset(L"SpriteMtrl", pMtrl.Get());

	// TileRenderMtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(CAssetMgr::GetInst()->Load<CGraphicShader>(L"TileRenderShader", true));
	CAssetMgr::GetInst()->AddAsset(L"TileRenderMtrl", pMtrl.Get());

}


void CAssetMgr::CreateEngineSprite()
{

	// 플립북 에셋 등록
	AssetPtr<CFlipbook> pFlipbook = new CFlipbook;
	for (int i = 0; i < 8; ++i)
	{
		wchar_t path[255] = {};
		swprintf_s(path, 255, L"Texture2D\\Spr_SNB_Idle (lp) (%d).png", i + 1);
		pFlipbook->AddScene<CTexture2D>(CAssetMgr::GetInst()->Load<CTexture2D>(path));
	}
	AddAsset(L"SNB_Idle_Flipbook", pFlipbook.Get());

	pFlipbook = new CFlipbook;
	for (int i = 0; i < 8; ++i)
	{
		wchar_t path[255] = {};
		swprintf_s(path, 255, L"Texture2D\\Spr_SNBArm_Idle (%d).png", i + 1);
		pFlipbook->AddScene<CTexture2D>(CAssetMgr::GetInst()->Load<CTexture2D>(path));
	}
	AddAsset(L"SNBArm_Idle_Flipbook", pFlipbook.Get());

	pFlipbook = new CFlipbook;
	for (int i = 0; i < 20; ++i)
	{
		wchar_t path[255] = {};
		swprintf_s(path, 255, L"Texture2D\\Spr_SNB_Running (lp) (%d).png", i + 1);
		pFlipbook->AddScene<CTexture2D>(CAssetMgr::GetInst()->Load<CTexture2D>(path));
	}
	AddAsset(L"SNB_Running_Flipbook", pFlipbook.Get());

	pFlipbook = new CFlipbook;
	for (int i = 0; i < 20; ++i)
	{
		wchar_t path[255] = {};
		swprintf_s(path, 255, L"Texture2D\\Spr_SNBArm_Running (lp) (%d).png", i + 1);
		pFlipbook->AddScene<CTexture2D>(CAssetMgr::GetInst()->Load<CTexture2D>(path));
	}
	AddAsset(L"SNBArm_Running_Flipbook", pFlipbook.Get());
}
