#include "pch.h"
#include "CImguiMgr.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"

#include "CLevel.h"
#include "CInspector.h"
#include "COutliner.h"
#include "CContentViewer.h"
#include "CMaterial.h"

void CImguiMgr::DebugMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("Asset"))
            {
                if (ImGui::MenuItem("Asset Load", nullptr))
                {
                    LoadAsset();
                }

				if (ImGui::BeginMenu("Asset Create"))
				{
                    if (ImGui::MenuItem("Material", nullptr))
                    {
                        m_PopupFlag |= PopupFlags_CreateMaterial;
                    }
                    if (ImGui::MenuItem("Graphic Shader", nullptr))
                    {

                    }
                    if (ImGui::MenuItem("Sprite", nullptr))
                    {

                    }
                    if (ImGui::MenuItem("Flipbook", nullptr))
                    {
                        m_PopupFlag |= PopupFlags_CreateFlipbook;
                    }

                    ImGui::EndMenu();
				}

				ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Level"))
            {

                if (ImGui::MenuItem("New", nullptr))
                {
                    m_PopupFlag |= PopupFlags_NewLevel;
                }
                if (ImGui::MenuItem("Save", nullptr))
                {
                    CLevel* Level = CLevelMgr::GetInst()->GetCurrentLevel();
                    if (Level) { Level->Save(Level->GetName()); }
                }
                if (ImGui::MenuItem("Save As..", nullptr))
                {
                    m_PopupFlag |= PopupFlags_SaveLevel;
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
                    m_PopupFlag |= PopupFlags_AddGameObjectMenu;
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


    // 팝업함수 호출
    AddGameObjectMenuPopup();
    NewLevelPopup();
    SaveLevelPopup();
    CreateMaterialPopup();
    CreateFlipbookPopup();
}