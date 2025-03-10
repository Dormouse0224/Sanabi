#include "pch.h"
#include "CInspector.h"

#include "CGameObject.h"
#include "CComponentUI.h"
#include "CTransformUI.h"
#include "CCameraUI.h"
#include "CPhysxActorUI.h"

CInspector::CInspector(wstring _Name)
	: CImguiObject(_Name)
	, m_TargetObject(nullptr)
{
	m_ComponentUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new CTransformUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::CAMERA] = new CCameraUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::PHYSXACTOR] = new CPhysxActorUI;
}

CInspector::~CInspector()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
	{
		delete m_ComponentUI[i];
	}
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

	for (int i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
	{
		if (m_TargetObject->GetComponent((COMPONENT_TYPE)i))
		{
			if (m_ComponentUI[i] == nullptr)
				continue;
			m_ComponentUI[i]->SetTarget(m_TargetObject);
			m_ComponentUI[i]->Render();
		}
	}
}
