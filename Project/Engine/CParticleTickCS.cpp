#include "pch.h"
#include "CParticleTickCS.h"



CParticleTickCS::CParticleTickCS()
	: CComputeShader(L"HLSL\\Engine\\particle_tick.fx", "CS_ParticleTick", 256, 1, 1)
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

	// ��ƼŬ ����
	m_ParticleBuffer->Binding_CS_UAV(0);

	// ���� ����
	m_SpawnBuffer->Binding_CS_UAV(1);

	// ��� ������
	m_ModuleBuffer->Binding_CS_SRV(20);

	// ��ƼŬ ���� �ִ� ����
	m_Const.iArr[0] = (int)m_ParticleBuffer->GetElementCount();


	return S_OK;
}

void CParticleTickCS::CalcGroupNum()
{
	m_GroupX = m_ParticleBuffer->GetElementCount() / m_ThreadPerGroupX;
	if (m_ParticleBuffer->GetElementCount() % m_ThreadPerGroupX)
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

	m_ModuleBuffer->Clear_CS_SRV();
	m_ModuleBuffer = nullptr;


}

void CParticleTickCS::SetData()
{
	SetConstData(INT_0, "Max Particle Count");
	SetTexData(TEX_0, "Noise Texture");
}
