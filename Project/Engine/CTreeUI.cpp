#include "pch.h"
#include "CTreeUI.h"

// Tree Node

UINT TreeNode::g_NextTreeID = 0;

TreeNode::TreeNode(string _Name)
	: m_TreeID(g_NextTreeID++)
	, m_Data((DWORD_PTR)nullptr)
	, m_Parent(nullptr)
	, m_vecChild{}
{
	m_Name = _Name + "##" + to_string(m_TreeID);
}

TreeNode::TreeNode(string _Name, DWORD_PTR _Data, TreeNode* _Parent)
	: m_TreeID(g_NextTreeID++)
	, m_Data(_Data)
	, m_Parent(_Parent)
	, m_vecChild{}
{
	m_Name = _Name + "##" + to_string(m_TreeID);

	if (_Parent)
		_Parent->m_vecChild.push_back(this);
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChild);
}

void TreeNode::Render()
{
	ImGuiTreeNodeFlags flag = 0;

	if (m_vecChild.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(m_Name.c_str(), flag))
	{
		for (auto child : m_vecChild)
		{
			child->Render();
		}

		ImGui::TreePop();
	}
}

void TreeNode::Clear()
{
	Delete_Vec(m_vecChild);
}

// Tree UI

CTreeUI::CTreeUI(wstring _Name)
	: CImguiObject(_Name)
	, m_Root(nullptr)
	, m_ShowRoot(false)
{
	m_Root = new TreeNode(string(_Name.begin(), _Name.end()));
}

CTreeUI::~CTreeUI()
{
	delete m_Root;
}

void CTreeUI::Update()
{
}

void CTreeUI::Render()
{
	if (m_ShowRoot)
	{
		m_Root->Render();
	}
	else
	{
		for (auto child : m_Root->m_vecChild)
		{
			child->Render();
		}
	}
		
}

void CTreeUI::AddChildNode(TreeNode* _Parent, string _Name, DWORD_PTR m_Data)
{
	TreeNode* pChild = new TreeNode(_Name);
	pChild->m_Data = m_Data;
	pChild->m_Parent = _Parent;

	_Parent->m_vecChild.push_back(pChild);
}

