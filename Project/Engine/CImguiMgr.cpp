#include "pch.h"
#include "CImguiMgr.h"

#include "CImguiObject.h".

#include "CLevelMgr.h"

#include "CInspector.h"
#include "COutliner.h"
#include "CContentViewer.h"

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

            if (ImGui::MenuItem("Content Viewer", nullptr))
            {
                m_ContentViewer->SetActive(true);
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Add GameObject", nullptr))
            {
                m_AddGameObjectMenuActive = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    AddGameObjectMenu();
}

void CImguiMgr::AddGameObjectMenu()
{
    if (m_AddGameObjectMenuActive)
    {
        m_AddGameObjectMenuActive = false;
        ImGui::OpenPopup("AddGameObjectMenu");
    }

    if (ImGui::BeginPopupModal("AddGameObjectMenu", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        float tab = 130.f;
        ImGui::Text("Adding GameObject instance to current level.");
        ImGui::NewLine();

        // 오브젝트 이름 입력
        ImGui::Text("Name: ");
        ImGui::SameLine(tab);
        static char Name[255] = {};
        ImGui::InputText("##Name", Name, 255);

        // 레이어 입력
        ImGui::Text("Layer select");
        static int layeridx = 0;
        string layerstr[static_cast<int>(LAYER::END)] = {};
        const char* layer[static_cast<int>(LAYER::END)] = {};
        for (int i = 0; i < static_cast<int>(LAYER::END); ++i)
        {
            layerstr[i] = to_str(LAYER_WSTR[i]);
            layer[i] = layerstr[i].c_str();
        }
        ImGui::Combo("##Topology", &layeridx, layer, static_cast<int>(LAYER::END));

        if (ImGui::Button("Add"))
        {
            CLevelMgr::GetInst()->AddGameObject(to_wstr(string(Name)), static_cast<LAYER>(layeridx));
            layeridx = 0;
            memset(Name, 0, sizeof(Name));
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }


        ImGui::EndPopup();
    }
    
}
