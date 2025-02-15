#include "pch.h"
#include "CImguiObject.h"

CImguiObject::CImguiObject()
	: m_Parent(nullptr)
	, m_vecChild{}
{
}

CImguiObject::~CImguiObject()
{
	for (auto& child : m_vecChild)
	{
		delete child;
	}
	m_vecChild.clear();
}

void CImguiObject::Update_Progress()
{
	Update();

	for (const auto& child : m_vecChild)
	{
		child->Update();
	}
}

void CImguiObject::Render_Progress()
{
	Render();

	for (const auto& child : m_vecChild)
	{
		child->Render();
	}
}


