#include "pch.h"
#include "CTileRnederUI.h"
#include "CTileRender.h"

CTileRnederUI::CTileRnederUI()
	: CRenderComponentUI(COMPONENT_TYPE::TILERENDER)
{
}

CTileRnederUI::~CTileRnederUI()
{
}

void CTileRnederUI::Update_Com()
{
}

void CTileRnederUI::Render_Com()
{
    Render_RCom();

    UINT& Col = static_cast<CTileRender*>(m_TargetObj->GetRenderComponent())->m_Col;
    UINT& Row = static_cast<CTileRender*>(m_TargetObj->GetRenderComponent())->m_Row;
    Vec2& TileSize = static_cast<CTileRender*>(m_TargetObj->GetRenderComponent())->m_TileSize;
    AssetPtr<CTexture2D> AtlasTex = static_cast<CTileRender*>(m_TargetObj->GetRenderComponent())->m_AtlasTex;

    ImGui::SeparatorText("Tile Render Option");
    ImGui::Text("Column");
    ImGui::SameLine();
    ImGui::InputScalar("##Col", ImGuiDataType_U32, &Col);

    ImGui::Text("Row");
    ImGui::SameLine();
    ImGui::InputScalar("##Row", ImGuiDataType_U32, &Row);

    ImGui::Text("Tile Size");
    ImGui::SameLine();
    float farrTileSize[] = { TileSize.x, TileSize.y };
    if (ImGui::InputFloat2("##TileSize", farrTileSize))
    {
        TileSize.x = farrTileSize[0];
        TileSize.y = farrTileSize[1];
    }

    ImGui::Text("Atlas Texture");
    std::string AtlasTexName = "NO EXIST";
    if (AtlasTex.Get())
    {
        AtlasTexName = to_str(AtlasTex->GetName());
    }
    ImGui::InputText("##AtlasTex", const_cast<char*>(AtlasTexName.c_str())
        , AtlasTexName.size() + 1, ImGuiInputTextFlags_ReadOnly);
}
