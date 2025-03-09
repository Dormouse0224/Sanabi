#include "pch.h"
#include "CCameraUI.h"

#include "CGameObject.h"
#include "CCamera.h"

CCameraUI::CCameraUI()
    : CComponentUI(COMPONENT_TYPE::CAMERA)
{
}

CCameraUI::~CCameraUI()
{
}

void CCameraUI::Update_Com()
{
}

void CCameraUI::Render_Com()
{
    float tab = 100.f;

    ImGui::Text("Projection Type");
    const char* ProjTypes[] = { "ORTHOGRAPHIC", "PERSPECTIVE" };
    int ProjType = m_Target->Camera()->GetProjType();
    if (ImGui::BeginCombo("##ProjectionType", ProjTypes[ProjType]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(ProjTypes); n++)
        {
            const bool is_selected = (ProjType == n);
            if (ImGui::Selectable(ProjTypes[n], is_selected))
            {
                m_Target->Camera()->SetProjType((PROJ_TYPE)n);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Spacing();

    ImGui::Text("Camera View Width & Height");
    float WH[2] = { m_Target->Camera()->GetViewX(), m_Target->Camera()->GetViewY() };
    if (ImGui::DragFloat2("##CameraViewWidthHeight", WH, 1.f, 1.f, FLT_MAX))
    {
        m_Target->Camera()->SetViewX(WH[0]);
        m_Target->Camera()->SetViewY(WH[1]);
    }
    ImGui::Spacing();

    ImGui::BeginDisabled(m_Target->Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC);
    ImGui::Text("FOV");
    ImGui::SameLine(tab);
    float FOV = m_Target->Camera()->GetFOV();
    if (ImGui::DragFloat("##FOV", &FOV, 1.f, 1.f, FLT_MAX))
    {
        m_Target->Camera()->SetFOV(FOV);
    }
    ImGui::EndDisabled();
    ImGui::Spacing();

    ImGui::Text("Far");
    ImGui::SameLine(tab);
    float Far = m_Target->Camera()->GetFar();
    if (ImGui::DragFloat("##Far", &Far, 1.f, 10.f, FLT_MAX))
    {
        m_Target->Camera()->SetFar(Far);
    }
    ImGui::Spacing();

    //int         m_Priority;     // 카메라 우선순위, -1 : 미등록 카메라, 0 : 메인 카메라, 1 ~> : 서브 카메라

    bool LayerCheck[(UINT)CAMERA_LAYER::END] = {};
    ImGui::Text("Camera Layer");
    if (ImGui::BeginTable("##CameraLayer", 1, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
    {
        for (int i = 0; i < (UINT)CAMERA_LAYER::END; i++)
        {
            LayerCheck[i] = m_Target->Camera()->GetLayerState((CAMERA_LAYER)i);
            ImGui::TableNextColumn();
            if (ImGui::Selectable(to_str(CAMERA_LAYER_WSTR[i]).c_str(), &LayerCheck[i]))
            {
                m_Target->Camera()->CheckLayer((CAMERA_LAYER)i);
            }
        }
        ImGui::EndTable();
    }
}
