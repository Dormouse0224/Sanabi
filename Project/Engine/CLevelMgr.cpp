#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CGameObject.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	delete m_CurLevel;
}

void CLevelMgr::Init()
{

}

void CLevelMgr::Progress()
{
	m_LevelUpdated = false;

	if (m_CurLevel == nullptr)
		return;

	// =====
	// Tick
	// =====
	m_CurLevel->Tick();

	m_CurLevel->Deregister();

	m_CurLevel->FinalTick();

}

void CLevelMgr::AddGameObject(wstring _Name, LAYER _Layer)
{
	CGameObject* pObject = new CGameObject;
	pObject->SetName(_Name);
	m_CurLevel->AddGameObject(pObject, static_cast<int>(_Layer), true);
	m_LevelUpdated = true;
}
