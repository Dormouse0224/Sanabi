#include "pch.h"
#include "CUIComUI.h"

#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CUICom.h"
#include "CFont.h"

CUIComUI::CUIComUI()
	: CComponentUI(COMPONENT_TYPE::UICOM)
{
}

CUIComUI::~CUIComUI()
{
}

void CUIComUI::Update_Com()
{
}

void CUIComUI::Render_Com()
{
	//m_vecUIText;
    ImGui::Text("UI Text List");
    if (ImGui::BeginTable("##UITextList", 2, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Font");
		ImGui::TableSetupColumn("Text");
		ImGui::TableHeadersRow();

		for (auto& tuple : m_TargetObj->UICom()->m_vecUIText)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text(to_str(get<0>(tuple)->GetName()).c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(to_str(get<1>(tuple)).c_str());
		}
		ImGui::EndTable();
    }

	ImGui::NewLine();
	if (ImGui::Button("Add UI Text"))
	{
		ImGui::OpenPopup("AddUIText");
	}
	if (ImGui::BeginPopupModal("AddUIText", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		float tab = 130.f;
		ImGui::Text("Add condition to FSM.");
		ImGui::NewLine();

		// Font Type
		ImGui::Text("Font Type");
		static int FontIdx = 0;
		vector<const char*> vec;
		auto vecstr = CAssetMgr::GetInst()->GetAssetList<CFont>();
		for (int i = 0; i < vecstr.size(); ++i)
			vec.push_back(vecstr[i].c_str());
		ImGui::Combo("##Font", &FontIdx, vec.data(), vec.size());

		// Text
		ImGui::Text("Text");
		static char buff[255] = {};
		ImGui::InputText("##Text", buff, 255);

		// Color
		ImGui::Text("Color");
		static float color[4];
		ImGui::ColorPicker4("##Color", color);

		// Rotation
		ImGui::Text("Rotation");
		static float rot = 0;
		ImGui::InputFloat("##Rot", &rot);

		// Scale
		ImGui::Text("Scale");
		static float scale = 1;
		ImGui::InputFloat("##Scale", &scale);

		// Independent
		ImGui::Text("Independent Scale?");
		static bool ind = 0;
		ImGui::Checkbox("##Ind", &ind);

		if (ImGui::Button("Add"))
		{
			tUITextDesc* pDesc = new tUITextDesc;
			pDesc->Color = Vec4(color);
			pDesc->Rot = rot;
			pDesc->Scale = scale;
			pDesc->Independent = ind;

			m_TargetObj->UICom()->AddUIText(CAssetMgr::GetInst()->Load<CFont>(to_wstr(vec[FontIdx])), to_wstr(buff), pDesc);
			FontIdx = 0; memset(buff, 0, sizeof(buff)); memset(color, 0, sizeof(color)); scale = 1; ind = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			FontIdx = 0; memset(buff, 0, sizeof(buff)); memset(color, 0, sizeof(color)); rot = 0; scale = 1; ind = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
