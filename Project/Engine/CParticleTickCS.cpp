#include "pch.h"
#include "CParticleTickCS.h"

CParticleTickCS::CParticleTickCS()
	: CComputeShader(L"HLSL\\particle_tick.fx", "CS_ParticleTick", 256, 1, 1)
	, m_ParticleBuffer(nullptr)
{
}

CParticleTickCS::~CParticleTickCS()
{
}

int CParticleTickCS::Binding()
{
	if (nullptr == m_ParticleBuffer)
		return E_FAIL;

	m_ParticleBuffer->Binding_CS_UAV(0);
	m_SpawnBuffer->Binding_CS_UAV(1);

	m_Const.iArr[0] = (int)m_ParticleBuffer->GetElementCount();

	return S_OK;
}

void CParticleTickCS::CalcGroupNum()
{
	m_GroupX = m_ParticleBuffer->GetElementCount() / m_GroupPerThreadX;
	if (m_ParticleBuffer->GetElementCount() % m_GroupPerThreadX)
		m_GroupX += 1;

	m_GroupY = 1;
	m_GroupZ = 1;
}

void CParticleTickCS::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV();
	m_ParticleBuffer = nullptr;

	m_SpawnBuffer->Clear_CS_UAV();
	m_SpawnBuffer = nullptr;
}