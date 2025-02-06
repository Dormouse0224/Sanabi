#include "pch.h"
#include "CParticleRender.h"

#include "CAssetMgr.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CParticleRender::CParticleRender()
	: CRenderComponent(COMPONENT_TYPE::PARTICLERENDER)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticle(500)
	, m_Module{}
	, m_AccTime(0.f)
{
	m_SpawnBuffer = new CStructuredBuffer;
	m_SpawnBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true);

	// 파티클 초기위치 세팅
	tParticle arrParticle[500] = {};
	float Radius = 200.f;

	float fTheta = 0.f;
	for (int i = 0; i < 500; ++i)
	{
		arrParticle[i].WorldPos = Vec3(Radius * cosf(fTheta), Radius * sinf(fTheta), 0.f);
		arrParticle[i].WorldScale = Vec3(10.f, 10.f, 1.f);

		arrParticle[i].Velocity = arrParticle[i].WorldPos;
		arrParticle[i].Velocity.Normalize();
		arrParticle[i].Velocity *= 500;

		arrParticle[i].Active = false;

		fTheta += XM_2PI / 500;
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false, arrParticle);

	CreateMtrl();

	// 파티클 Tick 컴퓨트 쉐이더
	Ptr<CComputeShader> pCS = CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS");

	if (nullptr == pCS)
	{
		pCS = new CParticleTickCS;
		pCS->SetName(L"ParticleTickCS");
		CAssetMgr::GetInst()->AddAsset(pCS->GetName(), pCS.Get());
	}

	m_TickCS = (CParticleTickCS*)pCS.Get();
}

CParticleRender::~CParticleRender()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_SpawnBuffer)
		delete m_SpawnBuffer;
}

void CParticleRender::FinalTick()
{
	// 이번 프레임 파티클 활성화 개수 계산
	CalcSpawnCount();

	m_TickCS->SetSpawnBuffer(m_SpawnBuffer);
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->Excute();
}

void CParticleRender::Render()
{
	Transform()->Binding();

	// 파티클 버퍼 바인딩
	m_ParticleBuffer->Binding(20);

	GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Particle", L"Texture2D\\Ambient_Circle #220141.png"));

	// 재질 및 쉐이더 바인딩
	GetMaterial()->Binding();

	// 파티클 인스턴싱 렌더링
	GetMesh()->Render_Particle(m_MaxParticle);

	// 파티클 버퍼 Clear
	m_ParticleBuffer->Clear(20);
}

void CParticleRender::CreateMtrl()
{
	if (nullptr == GetMesh())
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));

	if (nullptr != GetMaterial())
		return;

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl");
	if (nullptr != pMtrl)
	{
		SetMaterial(pMtrl);
		return;
	}

	// 파티클 렌더링 쉐이더 및 재질
	Ptr<CGraphicShader> pShader = new CGraphicShader;
	pShader->SetName(L"ParticleRenderShader");

	pShader->CreateVertexShader(L"HLSL\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"HLSL\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"HLSL\\particle.fx", "PS_Particle");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	CAssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	pMtrl = new CMaterial;
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());

	// 머티리얼 설정
	SetMaterial(pMtrl);
}

void CParticleRender::CalcSpawnCount()
{
	m_AccTime += DT;
	float Term = 1.f / m_Module.SpawnRate;

	if (Term < m_AccTime)
	{
		m_AccTime -= Term;

		tSpawnCount count = {};
		count.SpawnCount = 1;
		m_SpawnBuffer->SetData(&count);
	}
}
