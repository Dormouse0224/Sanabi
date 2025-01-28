#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"

CLevel::CLevel()
	: m_Layer{}
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = new CLayer;
		m_Layer[i]->m_LayerIdx = i;
	}
}

CLevel::~CLevel()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		delete m_Layer[i];
	}
}

void CLevel::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Begin();
	}
}

void CLevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Tick();
	}
}

void CLevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->FinalTick();
	}
}

void CLevel::Deregister()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->m_vecObjects.clear();
	}
}

void CLevel::AddGameObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	m_Layer[_LayerIdx]->AddGameObject(_Object, _bChildMove);
}
