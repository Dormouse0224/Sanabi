#include "pch.h"
#include "CLightMgr.h"

#include "CStructuredBuffer.h"
#include "CLight.h"

CLightMgr::CLightMgr()
	: m_vecLightModule{}
	, m_LightModuleBuffer(nullptr)
{
	m_LightModuleBuffer = new CStructuredBuffer;
}

CLightMgr::~CLightMgr()
{
	if (m_LightModuleBuffer)
		delete m_LightModuleBuffer;
}

void CLightMgr::Tick()
{
	m_vecLightModule.clear();
	m_vecLight.clear();
}

void CLightMgr::RegisterLight(CLight* pLight)
{
	m_vecLightModule.push_back(pLight->GetLightModule());
	m_vecLight.push_back(pLight);
	pLight->SetLightIdx(m_vecLight.size() - 1);
}

void CLightMgr::Binding()
{
	// ���� ������ ���缭 ����ȭ ���� ��������
	if (m_LightModuleBuffer->GetElementCount() < (UINT)m_vecLightModule.size())
	{
		m_LightModuleBuffer->Create(sizeof(tLightModule), (UINT)m_vecLightModule.size(), SB_TYPE::SRV_ONLY, true);
	}


	if (!m_vecLightModule.empty())
	{
		// ���� ������ ����ȭ ���۷� ����
		m_LightModuleBuffer->SetData(m_vecLightModule.data(), sizeof(tLightModule), (UINT)m_vecLightModule.size());

		// t12 �� ���ε�
		m_LightModuleBuffer->Binding(12);
		g_global.g_Light2DCount = m_vecLightModule.size();
	}
}

void CLightMgr::Render()
{
	for (CLight* plight : m_vecLight)
	{
		plight->Render();
	}
}