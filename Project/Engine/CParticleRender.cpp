#include "pch.h"
#include "CParticleRender.h"

#include "CAssetMgr.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CPhysxActor.h"
#include "CPhysxMgr.h"

CParticleRender::CParticleRender()
	: CRenderComponent(COMPONENT_TYPE::PARTICLERENDER)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticle(1000)
	, m_Module{}
	, m_AccTime(0.f)
	, m_ModuleChanged(true)
	, m_ParticleTex(nullptr)
	, m_Active(false)
	, m_Gravity(true)
{
	m_SpawnBuffer = new CStructuredBuffer;
	m_SpawnBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true);

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_ONLY, true);

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, true);

	CreateMtrl();

	// ��ƼŬ Tick ��ǻƮ ���̴�
	AssetPtr<CParticleTickCS> pCS = CAssetMgr::GetInst()->Load<CParticleTickCS>(L"EA_ParticleTickCS");

	m_TickCS = pCS.Get();
}

CParticleRender::~CParticleRender()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_SpawnBuffer)
		delete m_SpawnBuffer;

	if (nullptr != m_ModuleBuffer)
		delete m_ModuleBuffer;

	if (!m_mapParticleObj.empty())
	{
		for (map<UINT, CGameObject*>::iterator iter = m_mapParticleObj.begin(); iter != m_mapParticleObj.end();)
		{
			delete iter->second;
			iter = m_mapParticleObj.erase(iter);
		}
	}
}

void CParticleRender::FinalTick()
{

	m_ModuleChanged = true;
	m_Module.ObjectWorldPos = Transform()->GetWorldPos();

	// �̹� ������ ��ƼŬ Ȱ��ȭ ���� ���
	CalcSpawnCount();

	// ��ƼŬ ��� ������ �ݿ�
	if (m_ModuleChanged)
	{
		m_ModuleChanged = false;
		m_ModuleBuffer->SetData(&m_Module);
	}

	m_TickCS->SetSpawnBuffer(m_SpawnBuffer);
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->SetNoiseTex(CAssetMgr::GetInst()->Load<CTexture2D>(L"Noise\\noise_03.jpg"));

	// ��ƼŬ ��ǻƮ ���̴� Tick ����
	m_TickCS->Execute();

	vector<tParticle> vecParticle = {};
	vecParticle.resize(m_ParticleBuffer->GetElementCount());
	m_ParticleBuffer->GetData(vecParticle.data());
	for (int i = 0; i < vecParticle.size(); ++i)
	{
		if (m_Module.SpaceType == 1)
		{
			if (vecParticle[i].Active == true && vecParticle[i].PrevActive == false)
			{
				CGameObject* pParticle = new CGameObject;
				pParticle->AddComponent(new CTransform);
				pParticle->AddComponent(new CPhysxActor);
				
				pParticle->Transform()->SetRelativePos(vecParticle[i].LocalPos.x, vecParticle[i].LocalPos.y, vecParticle[i].LocalPos.z - 3);
				pParticle->Transform()->SetRelativeScale(10.f, 10.f, 1.f);

				PxVec3 InitVel(vecParticle[i].Velocity.x, vecParticle[i].Velocity.y, vecParticle[i].Velocity.z);
				pParticle->PhysxActor()->SetRigidBody(RIGID_TYPE::DYNAMIC, NONE, m_Gravity, 0.01f, InitVel);
				COLLIDER_DESC desc;
				desc.Restitution - 0.f;
				desc.ShapeFlag = PxShapeFlag::eSIMULATION_SHAPE;
				desc.FilterLayer_Self = COLLISION_LAYER::ePARTICLE;
				desc.FilterLayer_Other = COLLISION_LAYER::eLANDSCAPE;
				pParticle->PhysxActor()->AddCollider(desc, PxVec3(1.f, 1.f, 1.f), PxVec3(0.f, 0.f, 0.f));

				vecParticle[i].EntityID = pParticle->GetID();
				m_mapParticleObj.insert(make_pair(pParticle->GetID(), pParticle));
			}
			else if (vecParticle[i].Active == false && vecParticle[i].PrevActive == true)
			{
				map<UINT, CGameObject*>::iterator iter = m_mapParticleObj.find(vecParticle[i].EntityID);
				CGameObject* pParticle = iter->second;
				if (pParticle)
				{
					CPhysxMgr::GetInst()->RemoveRigidBody(pParticle);
					delete pParticle;
					pParticle = nullptr;
				}
				m_mapParticleObj.erase(iter);
			}
		}
	}
	m_ParticleBuffer->SetData(vecParticle.data());
}

void CParticleRender::Render()
{
	Transform()->Binding();

	vector<tParticle> vecParticle = {};
	vecParticle.resize(m_ParticleBuffer->GetElementCount());
	m_ParticleBuffer->GetData(vecParticle.data());
	for (int i = 0; i < vecParticle.size(); ++i)
	{
		map<UINT, CGameObject*>::iterator iter = m_mapParticleObj.find(vecParticle[i].EntityID);
		if (iter != m_mapParticleObj.end())
		{
			vecParticle[i].LocalPos = iter->second->Transform()->GetRelativePos();
		}
	}
	m_ParticleBuffer->SetData(vecParticle.data());



	// ��ƼŬ ���� ���ε�
	m_ParticleBuffer->Binding(20);
	// ��� ���� ���ε�
	m_ModuleBuffer->Binding(21);

	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->SetScalarParam(VEC4_0, Transform()->GetWorldPos());

	// ���� �� ���̴� ���ε�
	GetMaterial()->Binding();

	// ��ƼŬ �ν��Ͻ� ������
	GetMesh()->Render_Particle(m_MaxParticle);

	// ��ƼŬ ���� Clear
	m_ParticleBuffer->Clear(20);
	// ������ Clear
	m_ModuleBuffer->Clear(21);
}

void CParticleRender::CreateMtrl()
{
	if (nullptr == GetMesh())
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));

	if (nullptr != GetMaterial())
		return;

	AssetPtr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl");
	if (nullptr != pMtrl)
	{
		SetMaterial(pMtrl);
		return;
	}

	// ��ƼŬ ������ ���̴� �� ����
	AssetPtr<CGraphicShader> pShader = new CGraphicShader;
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

	pMtrl = new CMaterial;
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());

	// ��Ƽ���� ����
	SetMaterial(pMtrl);
}

void CParticleRender::CalcSpawnCount()
{
	if (m_Active)
	{
		if (m_Module.SpawnRate == 0)
		{
			m_Active = false;
			tSpawnCount count = {};
			count.SpawnCount = m_SpawnCount;
			m_SpawnBuffer->SetData(&count);
		}
		else
		{
			m_AccTime += DT;
			float Term = 1.f / m_Module.SpawnRate;
			if (Term < m_AccTime)
			{
				m_AccTime -= Term;
				tSpawnCount count = {};
				count.SpawnCount = m_SpawnCount;
				m_SpawnBuffer->SetData(&count);
			}
		}
	}
}
