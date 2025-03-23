#include "pch.h"
#include "CFlipbookUI.h"

#include "CFlipbook.h"
#include "CTexture2D.h"
#include "CSprite.h"

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

    if (pFlipbook->GetSceneType() == SceneType::SPRITE)
    {
        ImGui::Text("Flipbook Type - Sprite");
        int sprite_current = 0;
        ImGui::SliderInt("##FlipbookSlider", &sprite_current, 0, pFlipbook->GetSceneCount() - 1);
        AssetPtr<CSprite> pScene = pFlipbook->GetScene<CSprite>(sprite_current);
        ImGui::Image(reinterpret_cast<ImTextureID>(pScene->GetAtlasTex()->GetSRV().Get()), ImVec2(300, 300));
    }
    else if (pFlipbook->GetSceneType() == SceneType::TEXTURE)
    {
        ImGui::Text("Flipbook Type - Texture");
        int texture_current = 0;
        ImGui::SliderInt("##FlipbookSlider", &texture_current, 0, pFlipbook->GetSceneCount() - 1);
        AssetPtr<CTexture2D> pScene = pFlipbook->GetScene<CTexture2D>(texture_current);
        ImGui::Image(reinterpret_cast<ImTextureID>(pScene->GetSRV().Get()), ImVec2(300, 300));
    }
}
