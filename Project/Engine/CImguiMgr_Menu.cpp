#include "pch.h"
#include "CImguiMgr.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"

#include "CLevel.h"
#include "CInspector.h"
#include "COutliner.h"
#include "CContentViewer.h"

void CImguiMgr::DebugMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::SeparatorText("Asset");
            if (ImGui::MenuItem("Asset Load", nullptr))
            {
                LoadAsset();
            }

            if (ImGui::BeginMenu("Level"))
            {

                if (ImGui::MenuItem("New", nullptr))
                {
                    CLevel* Level = CLevelMgr::GetInst()->GetCurrentLevel();
                    if (Level) { Level->Save(L"_autosave"); }
                    CLevel* NewLevel = new CLevel;
                    CTaskMgr::GetInst()->AddTask(TASK_TYPE::CHANGE_LEVEL, reinterpret_cast<DWORD_PTR>(NewLevel), NULL);
                }
                if (ImGui::MenuItem("Save", nullptr))
                {
                    CLevel* Level = CLevelMgr::GetInst()->GetCurrentLevel();
                    SYSTEMTIME time = CTimeMgr::GetInst()->GetTime();
                    wchar_t wstr[50];
                    swprintf_s(wstr, L"%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
                    if (Level) { Level->Save(L"Level_" + to_wstring(Level->GetID()) + L"_" + wstring(wstr)); }
                }
                if (ImGui::MenuItem("Save As..", nullptr))
                {
                    m_SaveLevelActive = true;
                }
                if (ImGui::MenuItem("Load", nullptr))
                {
                    LoadLevel();
                }

                ImGui::EndMenu();
            }
            

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::BeginMenu("Level"))
            {
                if (ImGui::MenuItem("Add GameObject"))
                {
                    m_AddGameObjectMenuActive = true;
                }

                if (ImGui::BeginMenu("Change State"))
                {
                    LEVEL_STATE state = LEVEL_STATE::NONE;
                    if (CLevelMgr::GetInst()->GetCurrentLevel())
                        state = CLevelMgr::GetInst()->GetCurrentLevel()->GetState();
                    if (ImGui::MenuItem("Play", nullptr, state == LEVEL_STATE::PLAY, state != LEVEL_STATE::NONE))
                    {
                        CTaskMgr::GetInst()->AddTask(TASK_TYPE::CHANGE_LEVEL_STATE, static_cast<DWORD_PTR>(LEVEL_STATE::PLAY), NULL);
                    }
                    if (ImGui::MenuItem("Stop", nullptr, state == LEVEL_STATE::STOP, state != LEVEL_STATE::NONE))
                    {
                        CTaskMgr::GetInst()->AddTask(TASK_TYPE::CHANGE_LEVEL_STATE, static_cast<DWORD_PTR>(LEVEL_STATE::STOP), NULL);
                    }
                    if (ImGui::MenuItem("Pause", nullptr, state == LEVEL_STATE::PAUSE, state != LEVEL_STATE::NONE))
                    {
                        CTaskMgr::GetInst()->AddTask(TASK_TYPE::CHANGE_LEVEL_STATE, static_cast<DWORD_PTR>(LEVEL_STATE::PAUSE), NULL);
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tool"))
        {
            ImGui::MenuItem("Demo Window", nullptr, &m_DemoActive);

            if (ImGui::MenuItem("Inspector"))
            {
                m_Inspector->SetActive(true);
            }

            if (ImGui::MenuItem("Outliner"))
            {
                m_Outliner->SetActive(true);
            }

            if (ImGui::MenuItem("Content Viewer"))
            {
                m_ContentViewer->SetActive(true);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }


    SaveLevelPopup();
    AddGameObjectMenuPopup();
}