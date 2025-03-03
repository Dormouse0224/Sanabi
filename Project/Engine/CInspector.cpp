#include "pch.h"
#include "CInspector.h"

#include "CGameObject.h"

CInspector::CInspector(wstring _Name)
	: CImguiObject(_Name)
	, m_TargetObject(nullptr)
{
}

CInspector::~CInspector()
{
}

void CInspector::Update()
{
}

void CInspector::Render()
{
	if (!m_TargetObject)
		return;
	
	ImGui::Text(string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end()).c_str());

	ImGui::Separator();
}
