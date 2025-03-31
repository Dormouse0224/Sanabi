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
    int ProjType = m_TargetObj->Camera()->GetProjType();
    if (ImGui::BeginCombo("##ProjectionType", ProjTypes[ProjType]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(ProjTypes); n++)
        {
            const bool is_selected = (ProjType == n);
            if (ImGui::Selectable(ProjTypes[n], is_selected))
            {
                m_TargetObj->Camera()->SetProjType((PROJ_TYPE)n);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Spacing();

    ImGui::Text("Camera View Width & Height");
    float WH[2] = { m_TargetObj->Camera()->GetViewX(), m_TargetObj->Camera()->GetViewY() };
    if (ImGui::DragFloat2("##CameraViewWidthHeight", WH, 1.f, 1.f, FLT_MAX))
    {
        m_TargetObj->Camera()->SetViewX(WH[0]);
        m_TargetObj->Camera()->SetViewY(WH[1]);
    }
    ImGui::Spacing();

    ImGui::BeginDisabled(m_TargetObj->Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC);
    ImGui::Text("FOV");
    ImGui::SameLine(tab);
    float FOV = m_TargetObj->Camera()->GetFOV();
    if (ImGui::DragFloat("##FOV", &FOV, 1.f, 1.f, FLT_MAX))
    {
        m_TargetObj->Camera()->SetFOV(FOV);
    }
    ImGui::EndDisabled();
    ImGui::Spacing();

    ImGui::Text("Far");
    ImGui::SameLine(tab);
    float Far = m_TargetObj->Camera()->GetFar();
    if (ImGui::DragFloat("##Far", &Far, 1.f, 10.f, FLT_MAX))
    {
        m_TargetObj->Camera()->SetFar(Far);
    }
    ImGui::Spacing();

    //int         m_Priority;     // ī�޶� �켱����, -1 : �̵�� ī�޶�, 0 : ���� ī�޶�, 1 ~> : ���� ī�޶�

    bool LayerCheck[(UINT)LAYER::END] = {};
    ImGui::Text("Camera Layer");
    if (ImGui::BeginTable("##CameraLayer", 1, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
    {
        for (int i = 0; i < (UINT)LAYER::END; i++)
        {
            LayerCheck[i] = m_TargetObj->Camera()->GetLayerState((LAYER)i);
            ImGui::TableNextColumn();
            if (ImGui::Selectable(to_str(LAYER_WSTR[i]).c_str(), &LayerCheck[i]))
            {
                m_TargetObj->Camera()->CheckLayer((LAYER)i);
            }
        }
        ImGui::EndTable();
    }
}
