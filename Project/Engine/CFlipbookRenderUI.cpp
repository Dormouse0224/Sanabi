#include "pch.h"
#include "CFlipbookRenderUI.h"
#include "CFlipbookRender.h"

CFlipbookRenderUI::CFlipbookRenderUI()
	: CRenderComponentUI(COMPONENT_TYPE::FLIPBOOKRENDER)
{
}

CFlipbookRenderUI::~CFlipbookRenderUI()
{
}

void CFlipbookRenderUI::Update_Com()
{
}

void CFlipbookRenderUI::Render_Com()
{
    Render_RCom();

    vector<AssetPtr<CFlipbook>> vecFlipbook = static_cast<CFlipbookRender*>(m_TargetObj->GetRenderComponent())->m_vecFlipbook;


    ImGui::SeparatorText("Flipbook Render Option");
    if (ImGui::BeginTable("FlipbookList", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Flipbook List");
        ImGui::TableHeadersRow();
        
        for (int row = 0; row < vecFlipbook.size(); ++row)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(to_str(vecFlipbook[row]->GetName()).c_str());
        }
        ImGui::EndTable();
    }

    // 재생 정보
    AssetPtr<CFlipbook> CurFlipbook = static_cast<CFlipbookRender*>(m_TargetObj->GetRenderComponent())->m_CurFlipbook;
    int& SceneIdx = static_cast<CFlipbookRender*>(m_TargetObj->GetRenderComponent())->m_SceneIdx;
    float& FPS = static_cast<CFlipbookRender*>(m_TargetObj->GetRenderComponent())->m_FPS;
    bool& Repeat = static_cast<CFlipbookRender*>(m_TargetObj->GetRenderComponent())->m_Repeat;

    ImGui::SliderInt("##FlipbookSlider", &SceneIdx, 0, CurFlipbook->GetSceneCount() - 1);

    ImGui::Text("FPS");
    ImGui::SameLine();
    ImGui::InputFloat("##FPS", &FPS);

    ImGui::Text("Repeat?");
    ImGui::SameLine();
    ImGui::Checkbox("##Repeat", &Repeat);
}
