#include "pch.h"
#include "CSpriteUI.h"

#include "CSprite.h"
#include "CTexture2D.h"

CSpriteUI::CSpriteUI()
	: CAssetUI(ASSET_TYPE_WSTR[(UINT)ASSET_TYPE::SPRITE])
{
}

CSpriteUI::~CSpriteUI()
{
}

void CSpriteUI::Update_Ast()
{
}

void CSpriteUI::Render_Ast()
{
    AssetPtr<CSprite> pSprite = static_cast<CSprite*>(m_TargetAsset.Get());
    float tab = 130;

    // 스프라이트의 아틀라스 텍스쳐 정보
    ImGui::Text("Atlas Texture");
    ImGui::InputText("##AtlasTex", const_cast<char*>(to_str(pSprite->GetAtlasTex()->GetName()).c_str())
        , to_str(pSprite->GetAtlasTex()->GetName()).size() + 1, ImGuiInputTextFlags_ReadOnly);
    ImGui::Image(reinterpret_cast<ImTextureID>(pSprite->GetAtlasTex()->GetSRV().Get()), ImVec2(300, 300));
    
    // LeftTop
    ImGui::Text("LeftTop");
    float LeftTop[2] = { pSprite->GetLeftTop().x, pSprite->GetLeftTop().y };
    ImGui::InputFloat("##LeftTop", LeftTop, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
    
    // Slice
    ImGui::Text("Slice");
    float Slice[2] = { pSprite->GetSlice().x, pSprite->GetSlice().y };
    ImGui::InputFloat("##Slice", Slice, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);

    // Offset
    ImGui::Text("Offset");
    float Offset[2] = { pSprite->GetOffset().x, pSprite->GetOffset().y };
    ImGui::InputFloat("##Offset", Offset, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);

    // Background
    ImGui::Text("Background");
    float Background[2] = { pSprite->GetBackground().x, pSprite->GetBackground().y };
    ImGui::InputFloat("##Background", Background, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
}
