#include "pch.h"
#include "CMeshUI.h"
#include "CMesh.h"

CMeshUI::CMeshUI()
	: CAssetUI(ASSET_TYPE_WSTR[(UINT)ASSET_TYPE::MESH])
{
}

CMeshUI::~CMeshUI()
{
}

void CMeshUI::Update_Ast()
{
}

void CMeshUI::Render_Ast()
{
	float tab = 130.f;

	AssetPtr<CMesh> pMesh = static_cast<CMesh*>(m_TargetAsset.Get());
	int VtxCount = pMesh->GetVtxCount();
	int IdxCount = pMesh->GetIdxCount();

	ImGui::Text("Vertex Count");
	ImGui::SameLine(tab);
	ImGui::BeginDisabled(true);
	ImGui::InputInt("##VertexCount", &VtxCount, 0, 0, ImGuiInputTextFlags_ReadOnly);
	ImGui::EndDisabled();

	ImGui::Text("Index Count");
	ImGui::SameLine(tab);
	ImGui::BeginDisabled(true);
	ImGui::InputInt("##IndexCount", &IdxCount, 0, 0, ImGuiInputTextFlags_ReadOnly);
	ImGui::EndDisabled();
}
