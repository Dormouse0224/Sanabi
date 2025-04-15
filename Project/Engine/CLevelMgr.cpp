#include "pch.h"
#include "CLevelMgr.h"

#include "CRenderMgr.h"
#include "CPhysxMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "CLayer.h"

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

	for (const auto& pair : m_mapLevelList)
	{
		delete pair.second;
	}
	m_mapLevelList.clear();
}

void CLevelMgr::Init()
{

}

void CLevelMgr::Progress()
{
	if (m_LevelModified)
		m_LevelModified = false;
	if (m_LevelModifyChecker)
	{
		m_LevelModifyChecker = false;
		m_LevelModified = true;
	}

	if (m_CurLevel == nullptr)
		return;

	// =====
	// Tick
	// =====
	m_CurLevel->Tick();

	m_CurLevel->Deregister();

	m_CurLevel->FinalTick();

}

CLevel* CLevelMgr::FindLevel(wstring _RelativePath)
{
	const auto iter = m_mapLevelList.find(_RelativePath);
	if (iter == m_mapLevelList.end())
		return nullptr;
	else
		return iter->second->Clone();
}

void CLevelMgr::DeleteLevel(wstring _RelativePath)
{
	const auto iter = m_mapLevelList.find(_RelativePath);
	if (iter == m_mapLevelList.end())
		return;
	else
	{
		delete iter->second;
		m_mapLevelList.erase(iter);
	}
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
	// 레벨이 변경되면서, Render Manager 가 관리하던 이전 레벨의 카메라 목록을 클리어한다.
	CRenderMgr::GetInst()->ClearCamera();

	// 물리 시뮬레이션 객체를 모두 제거한다.
	CPhysxMgr::GetInst()->ClearScene();


	if (m_CurLevel == _NextLevel)
		return;

	delete m_CurLevel;

	m_CurLevel = _NextLevel;

	if (m_CurLevel == m_PlayStartLevel)
		m_PlayStartLevel = nullptr;
}

CGameObject* CLevelMgr::FindObject(LAYER _Layer, wstring _Name)
{
	const auto& vec = m_CurLevel->GetLayer(static_cast<int>(_Layer))->GetObjects();
	for (const auto pObj : vec)
	{
		if (pObj->GetName() == _Name)
			return pObj;
	}
	return nullptr;
}
