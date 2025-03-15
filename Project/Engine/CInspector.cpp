#include "pch.h"
#include "CInspector.h"

#include "CGameObject.h"
#include "CComponentUI.h"
#include "CTransformUI.h"
#include "CCameraUI.h"
#include "CPhysxActorUI.h"

#include "CAsset.h"
#include "CAssetUI.h"
#include "CMeshUI.h"

CInspector::CInspector(wstring _Name)
	: CImguiObject(_Name)
	, m_TargetObj(nullptr)
	, m_ComponentUI{}
	, m_TargetAsset(nullptr)
	, m_AssetUI{}
{
	m_ComponentUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new CTransformUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::CAMERA] = new CCameraUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::PHYSXACTOR] = new CPhysxActorUI;

	m_AssetUI[(UINT)ASSET_TYPE::MESH] = new CMeshUI;
}

CInspector::~CInspector()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
	{
		delete m_ComponentUI[i];
	}

	for (int i = 0; i < (UINT)ASSET_TYPE::ASSET_END; ++i)
	{
		delete m_AssetUI[i];
	}
}

void CInspector::Update()
{
	switch (m_Type)
	{
	case TARGET_TYPE::GAMEOBJECT:
	{
		if (!m_TargetObj)
			return;

		for (int i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
		{
			if (m_TargetObj->GetComponent((COMPONENT_TYPE)i))
			{
				if (m_ComponentUI[i] == nullptr)
					continue;
				m_ComponentUI[i]->SetTargetObj(m_TargetObj);
				m_ComponentUI[i]->Update();
			}
		}
	}
	break;
	case TARGET_TYPE::ASSET:
	{
		if (!m_TargetAsset.Get())
			return;
		if (CAssetUI* pAssetUI = m_AssetUI[(UINT)m_TargetAsset->GetType()])
		{
			pAssetUI->SetTarget(m_TargetAsset);
			pAssetUI->Update();
		}
	}
	break;
	}
}

void CInspector::Render()
{
	switch (m_Type)
	{
	case TARGET_TYPE::GAMEOBJECT:
	{
		if (!m_TargetObj)
			return;
		ImGui::Text(to_str(m_TargetObj->GetName()).c_str());
		ImGui::Separator();

		for (int i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
		{
			if (m_TargetObj->GetComponent((COMPONENT_TYPE)i))
			{
				if (m_ComponentUI[i] == nullptr)
					continue;
				m_TargetAsset;
				m_ComponentUI[i]->Render();
			}
		}
	}
		break;
	case TARGET_TYPE::ASSET:
	{
		if (!m_TargetAsset.Get())
			return;
		ImGui::Text(to_str(m_TargetAsset->GetName()).c_str());
		ImGui::Separator();
		if (CAssetUI* pAssetUI = m_AssetUI[(UINT)m_TargetAsset->GetType()])
		{
			pAssetUI->SetTarget(m_TargetAsset);
			pAssetUI->Render();
		}
	}
		break;
	}
	
	
}

void CInspector::SetTarget(CEntity* _Obj, TARGET_TYPE _Type)
{
	m_Type = _Type;
	switch (_Type)
	{
	case TARGET_TYPE::GAMEOBJECT:
	{
		m_TargetObj = dynamic_cast<CGameObject*>(_Obj);
	}
		break;
	case TARGET_TYPE::ASSET:
	{
		m_TargetAsset = dynamic_cast<CAsset*>(_Obj);
	}
		break;
	}
}
