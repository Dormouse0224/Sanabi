#include "pch.h"
#include "CFlipbookUI.h"

#include "CFlipbook.h"
#include "CTexture2D.h"
#include "CSprite.h"
#include "CAssetMgr.h"

CFlipbookUI::CFlipbookUI()
	: CAssetUI(ASSET_TYPE_WSTR[(UINT)ASSET_TYPE::FLIPBOOK])
{
}

CFlipbookUI::~CFlipbookUI()
{
}

void CFlipbookUI::Update_Ast()
{
}

void CFlipbookUI::Render_Ast()
{
    AssetPtr<CFlipbook> pFlipbook = static_cast<CFlipbook*>(m_TargetAsset.Get());
    float tab = 130;
    static int sprite_current = 0;
    static int texture_current = 0;

    if (pFlipbook->GetSceneType() == SceneType::SPRITE)
    {
        ImGui::Text("Flipbook Type - Sprite");
        sprite_current = min(sprite_current, pFlipbook->GetSceneCount() - 1);
        ImGui::SliderInt("##FlipbookSlider", &sprite_current, 0, pFlipbook->GetSceneCount() - 1);
        AssetPtr<CSprite> pScene = pFlipbook->GetScene<CSprite>(sprite_current);
        if (pScene.Get())
            ImGui::Image(reinterpret_cast<ImTextureID>(pScene->GetAtlasTex()->GetSRV().Get()), ImVec2(300, 300));
        else
            ImGui::Text("Filpbook Empty");
    }
    else if (pFlipbook->GetSceneType() == SceneType::TEXTURE)
    {
        ImGui::Text("Flipbook Type - Texture");
        texture_current = min(texture_current, pFlipbook->GetSceneCount() - 1);
        ImGui::SliderInt("##FlipbookSlider", &texture_current, 0, pFlipbook->GetSceneCount() - 1);
        AssetPtr<CTexture2D> pScene = pFlipbook->GetScene<CTexture2D>(texture_current);
        if (pScene.Get())
            ImGui::Image(reinterpret_cast<ImTextureID>(pScene->GetSRV().Get()), ImVec2(300, 300));
        else
            ImGui::Text("Filpbook Empty");
    }
    
    if (ImGui::Button("Add Scene"))
    {
        if (pFlipbook->GetSceneType() == SceneType::SPRITE)
        {
            AssetPtr<CSprite> pSprite = CAssetMgr::GetInst()->LoadFromFile<CSprite>(L".sprite");
            if (pSprite.Get())
                pFlipbook->AddScene<CSprite>(pSprite);
        }
        else if (pFlipbook->GetSceneType() == SceneType::TEXTURE)
        {
            AssetPtr<CTexture2D> pTex = CAssetMgr::GetInst()->LoadFromFile<CTexture2D>(L"TEX");
            if (pTex.Get())
                pFlipbook->AddScene<CTexture2D>(pTex);
        }
    }

    if (ImGui::Button("Delete Scene"))
    {
        if (pFlipbook->GetSceneType() == SceneType::SPRITE)
        {
            pFlipbook->DeleteScene(sprite_current);
        }
        else if (pFlipbook->GetSceneType() == SceneType::TEXTURE)
        {
            pFlipbook->DeleteScene(texture_current);
        }
    }
}
