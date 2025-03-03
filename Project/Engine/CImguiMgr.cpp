#include "pch.h"
#include "CImguiMgr.h"

#include "CImguiObject.h"
#include "CInspector.h"
#include "COutliner.h"

CImguiMgr::CImguiMgr()
    : m_DebugMenuBar(true)
    , m_DemoActive(false)
{

}

CImguiMgr::~CImguiMgr()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    for (auto& ui : m_mapImguiObj)
    {
        delete ui.second;
    }
    m_mapImguiObj.clear();
}



void CImguiMgr::Progress()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 메인 윈도우 메뉴바
    if (m_DebugMenuBar)
    {
        DebugMenuBar();
    }

    // Imgui Demo
    if (m_DemoActive)
    {
        ImGui::ShowDemoWindow(&m_DemoActive);
    }

    // 등록된 Imgui 객체 업데이트 및 렌더 준비
    for (const auto& ui : m_mapImguiObj)
    {
        if (!ui.second->GetParent())
            ui.second->Update_Progress();
    }

    for (const auto& ui : m_mapImguiObj)
    {
        if (!ui.second->GetParent())
            ui.second->Render_Progress();
    }


    // Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImguiMgr::AddImguiObj(CImguiObject* _ImguiObj)
{
    if (_ImguiObj->GetParent() == nullptr)
    {

    }
}

void CImguiMgr::DebugMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tool"))
        {
            ImGui::MenuItem("Demo Window", nullptr, &m_DemoActive);
            if (ImGui::MenuItem("Inspector", nullptr))
            {
                m_Inspector->SetActive(true);
            }
            if (ImGui::MenuItem("Outliner", nullptr))
            {
                m_Outliner->SetActive(true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
