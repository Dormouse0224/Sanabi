#include "pch.h"
#include "CLevelMgr.h"

#include "CRenderMgr.h"

#include "CLevel.h"
#include "CGameObject.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
	, m_PlayStartLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	delete m_CurLevel;
	m_CurLevel = nullptr;

	delete m_PlayStartLevel;
	m_PlayStartLevel = nullptr;
}

void CLevelMgr::Init()
{

}

void CLevelMgr::Progress()
{
	m_LevelModified = false;

	if (m_CurLevel == nullptr)
		return;

	// =====
	// Tick
	// =====
	m_CurLevel->Tick();

	m_CurLevel->Deregister();

	m_CurLevel->FinalTick();

}

CGameObject* CLevelMgr::AddGameObject(wstring _Name, LAYER _Layer)
{
	CGameObject* pObject = new CGameObject;
	pObject->SetName(_Name);
	m_CurLevel->AddGameObject(pObject, static_cast<int>(_Layer), true);
	m_LevelModified = true;

	return pObject;
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_CurLevel == _NextLevel)
		return;

	delete m_CurLevel;

	m_CurLevel = _NextLevel;

	if (m_CurLevel == m_PlayStartLevel)
		m_PlayStartLevel = nullptr;

	// 레벨이 변경되면서, Render Manager 가 관리하던 이전 레벨의 카메라 목록을 클리어한다.
	CRenderMgr::GetInst()->ClearCamera();
}