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
	m_ObjTree->SetDragDropFunc((DELEGATE_2)&COutliner::DragDropEvent, this);
}

COutliner::~COutliner()
{
	delete m_ObjTree;
}


void COutliner::Update()
{
	// ������ ������Ʈ�Ǹ� ���� ������Ʈ ��� ����
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

void COutliner::DragDropEvent(DWORD_PTR _Drag, DWORD_PTR _Drop)
{
	CGameObject* pDragObj = reinterpret_cast<CGameObject*>(_Drag);
	CGameObject* pDropObj = reinterpret_cast<CGameObject*>(_Drop);

	// �ڽ��� �θ𿡰� ����ϴ� ��� ����
	if (pDragObj->GetParent() == pDropObj)
		return;

	// �ڽ��� �ڽ� �迭 ������Ʈ�� ����ϴ� ��� ����
	if (pDragObj->IsAncestorOf(pDropObj))
		return;

	if (pDragObj->GetParent())
		pDragObj->GetParent()->DeleteChild(pDragObj);
	else
	{
		CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(pDragObj->GetLayerIdx())->UnRegisterParentObject(pDragObj);
		
	}
	pDropObj->AddChild(pDragObj);

	CLevelMgr::GetInst()->SetLevelModified();
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
