#include "pch.h"
#include "CInspector.h"

#include "CPathMgr.h"

#include "CGameObject.h"
#include "CComponentUI.h"
#include "CComponentMgr.h"

#include "CEngine.h"
#include "CAsset.h"
#include "assets.h"
#include "uilist.h"
#include "CAssetUI.h"

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
	m_ComponentUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new CMeshRenderUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::SPRITERENDER] = new CSpriteRenderUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::FLIPBOOKRENDER] = new CFlipbookRenderUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::TILERENDER] = new CParticleRenderUI;
	m_ComponentUI[(UINT)COMPONENT_TYPE::PARTICLERENDER] = new CParticleRenderUI;

	m_AssetUI[(UINT)ASSET_TYPE::MESH] = new CMeshUI;
	m_AssetUI[(UINT)ASSET_TYPE::MATERIAL] = new CMaterialUI;
	m_AssetUI[(UINT)ASSET_TYPE::TEXTURE2D] = new CTexture2DUI;
	m_AssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new CComputeShaderUI;
	m_AssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = new CGraphicShaderUI;
	m_AssetUI[(UINT)ASSET_TYPE::SPRITE] = new CSpriteUI;
	m_AssetUI[(UINT)ASSET_TYPE::FLIPBOOK] = new CFlipbookUI;
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

		float tab = 130.f;
		// 기본 정보 표기
		ImGui::Text(to_str(m_TargetObj->GetName()).c_str());
		ImGui::Separator();

		ImGui::Text("Layer");
		ImGui::SameLine(tab);
		if (m_TargetObj->GetLayerIdx() < 0)
			ImGui::InputText("##Layer", const_cast<char*>(string("NO LAYER").c_str())
				, 8, ImGuiInputTextFlags_ReadOnly);
		else
			ImGui::InputText("##Layer", const_cast<char*>(to_str(LAYER_WSTR[m_TargetObj->GetLayerIdx()]).c_str())
			, to_str(LAYER_WSTR[m_TargetObj->GetLayerIdx()]).size() + 1, ImGuiInputTextFlags_ReadOnly);

		// 컴포넌트 정보 표기
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

		// 스크립트 정보 표기


		// 오브젝트 프리펩화
		ImGui::Separator();
		if (ImGui::Button("Convert Object To Prefab"))
		{
			AssetPtr<CPrefab> prefab = new CPrefab;
			prefab->ConvertToPrefab(m_TargetObj);
		}

		ImGui::SameLine();
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}
		if (ImGui::BeginPopupModal("AddComponent", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			float tab = 130.f;
			ImGui::Text("Add component to GameObject.");
			ImGui::Text("Do not enter extention.");
			ImGui::Text("Directory folders and extention will automatically decided.");
			ImGui::NewLine();

			// 레이어 입력
			ImGui::Text("Component select");
			static int ComIdx = 0;
			string ComStr[static_cast<int>(COMPONENT_TYPE::COMPONENT_END)] = {};
			const char* Com[static_cast<int>(COMPONENT_TYPE::COMPONENT_END)] = {};
			for (int i = 0; i < static_cast<int>(COMPONENT_TYPE::COMPONENT_END); ++i)
			{
				ComStr[i] = to_str(COMPONENT_TYPE_WSTR[i]);
				Com[i] = ComStr[i].c_str();
			}
			ImGui::Combo("##Component", &ComIdx, Com, static_cast<int>(COMPONENT_TYPE::COMPONENT_END));

			// 저장/취소 버튼
			if (ImGui::Button("Add"))
			{
				m_TargetObj->AddComponent(CComponentMgr::GetInst()->CreateComp(static_cast<COMPONENT_TYPE>(ComIdx)));
				ComIdx = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ComIdx = 0;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
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

		ImGui::Separator();
		if (ImGui::Button("Save Asset as File"))
		{
			ImGui::OpenPopup("AssetSave");
		}
		if (ImGui::BeginPopupModal("AssetSave", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			float tab = 130.f;
			ImGui::Text("Save asset as file.");
			ImGui::Text("Do not enter extention.");
			ImGui::Text("Directory folders and extention will automatically decided.");
			ImGui::NewLine();

			// 에셋 이름 입력
			ImGui::Text("Name: ");
			ImGui::SameLine(tab);
			static char Name[255] = {};
			ImGui::InputText("##Name", Name, 255);

			// 저장/취소 버튼
			if (ImGui::Button("Save"))
			{
				if (m_TargetAsset->GetEngineAsset())
					MessageBoxW(nullptr, L"엔진 에셋은 파일 저장을 지원하지 않습니다.", L"Asset Save Error", MB_OK);
				else
					m_TargetAsset->Save(to_wstr(Name));
				memset(Name, 0, sizeof(Name));
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				memset(Name, 0, sizeof(Name));
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
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
