#include "pch.h"
#include "COutliner.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"

#include "CTreeUI.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

COutliner::COutliner(wstring _Name)
	: CImguiObject(_Name)
{
	m_LevelObjects = new CTreeUI(_Name);
}

COutliner::~COutliner()
{
	delete m_LevelObjects;
}


void COutliner::Update()
{
	// 레벨이 업데이트되면 레벨 오브젝트 목록 갱신
	if (CTaskMgr::GetInst()->GetLevelUpdated())
	{
		Renew();
	}
}

void COutliner::Render()
{
	m_LevelObjects->Render();
}

void COutliner::Renew()
{
	m_LevelObjects->GetRoot()->Clear();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (auto parentObj : pLevel->GetLayer(i)->GetParentObjects())
		{
			AddObject(parentObj, m_LevelObjects->GetRoot());
		}
	}
}

void COutliner::AddObject(CGameObject* _Obj, TreeNode* _ParentNode)
{
	TreeNode* pNode = new TreeNode(string(_Obj->GetName().begin(), _Obj->GetName().end()), (DWORD_PTR)_Obj, _ParentNode);

	for (auto child : _Obj->GetChild())
	{
		AddObject(child, pNode);
	}
}
