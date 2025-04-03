#include "pch.h"
#include "COutliner.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CImguiMgr.h"

#include "CTreeUI.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CInspector.h"

COutliner::COutliner(wstring _Name)
	: CImguiObject(_Name)
{
	m_ObjTree = new CTreeUI(_Name);
	m_ObjTree->SetDoubleClickFunc((DELEGATE_1)&COutliner::DoubleClickEvent, this);
}

COutliner::~COutliner()
{
	delete m_ObjTree;
}


void COutliner::Update()
{
	// 레벨이 업데이트되면 레벨 오브젝트 목록 갱신
	if (CLevelMgr::GetInst()->GetLevelModified())
	{
		Renew();
	}
}

void COutliner::Render()
{
	m_ObjTree->Render();
}

void COutliner::DoubleClickEvent(DWORD_PTR _Obj)
{
	CImguiMgr::GetInst()->GetInspector()->SetTarget((CGameObject*)_Obj, TARGET_TYPE::GAMEOBJECT);
}

void COutliner::Renew()
{
	m_ObjTree->GetRoot()->Clear();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (!pLevel)
		return;

	for (int i = 0; i < (UINT)LAYER::END; ++i)
	{
		for (auto parentObj : pLevel->GetLayer(i)->GetParentObjects())
		{
			AddObject(parentObj, m_ObjTree->GetRoot());
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
