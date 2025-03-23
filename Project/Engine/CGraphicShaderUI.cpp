#include "pch.h"
#include "CGraphicShaderUI.h"

#include "CGraphicShader.h"

CGraphicShaderUI::CGraphicShaderUI()
	: CAssetUI(ASSET_TYPE_WSTR[(UINT)ASSET_TYPE::GRAPHIC_SHADER])
{
}

CGraphicShaderUI::~CGraphicShaderUI()
{
}

void CGraphicShaderUI::Update_Ast()
{
}

void CGraphicShaderUI::Render_Ast()
{
    AssetPtr<CGraphicShader> pGS = static_cast<CGraphicShader*>(m_TargetAsset.Get());
    float tab = 130;

    // 그래픽 셰이더의 셰이더 정보
    for (int i = 0; i < SHADER_TYPE_END; ++i)
    {
        if (!pGS->GetShaderName(static_cast<SHADER_TYPE>(i)).first.empty())
        {
            ImGui::Text(SHADER_TYPE_STR[i].c_str());
            string label0 = "##ShaderDir" + to_string(i);
            string label1 = "##ShaderFunc" + to_string(i);
            ImGui::InputText(label0.c_str(), const_cast<char*>(to_str(pGS->GetShaderName(static_cast<SHADER_TYPE>(i)).first).c_str())
                , to_str(pGS->GetShaderName(static_cast<SHADER_TYPE>(i)).first).size() + 1, ImGuiInputTextFlags_ReadOnly);
            ImGui::InputText(label1.c_str(), const_cast<char*>(pGS->GetShaderName(static_cast<SHADER_TYPE>(i)).second.c_str())
                , pGS->GetShaderName(static_cast<SHADER_TYPE>(i)).second.size() + 1, ImGuiInputTextFlags_ReadOnly);
        }
    }
    
    // 토폴로지 정보
    ImGui::Text("Topology");
    const char* topology[] = 
    {
        "D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED",
        "D3D11_PRIMITIVE_TOPOLOGY_POINTLIST",
        "D3D11_PRIMITIVE_TOPOLOGY_LINELIST",
        "D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP",
        "D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST",
        "D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP",
        "D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ",
        "D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ",
        "D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ",
        "D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ",
    };
    int topology_current = pGS->GetTopology();
    if (topology_current < IM_ARRAYSIZE(topology))
        ImGui::Combo("##Topology", &topology_current, topology, IM_ARRAYSIZE(topology));
    else
        ImGui::Text("Topology out of range");
   

    // 셰이더 렌더링 도메인
    ImGui::Text("Render Domain");
    const char* domain[] = { "DOMAIN_QPAQUE", "DOMAIN_MASKED", "DOMAIN_TRANSPARENT", "DOMAIN_DEBUG", };
    int domain_current = static_cast<int>(pGS->GetDomain());
    ImGui::Combo("##RenderDomain", &domain_current, domain, IM_ARRAYSIZE(domain));

    // 래스터라이저 설정
    ImGui::Text("Resterizer State");
    const char* RSType[] = { "CULL_BACK", "CULL_FRONT", "CULL_NONE", "WIRE_FRAME" };
    int RSType_current = static_cast<int>(pGS->GetRSType());
    ImGui::Combo("##ResterizerState", &RSType_current, RSType, IM_ARRAYSIZE(RSType));

    // 깊이 스텐실 설정
    ImGui::Text("Depth-Stencil State");
    const char* DSType[] = { "LESS", "LESS_EQUAL", "GREATER", "NO_WRITE", "NO_TEST_NO_WIRITE" };
    int DSType_current = static_cast<int>(pGS->GetDSType());
    ImGui::Combo("##DepthStencilState", &DSType_current, DSType, IM_ARRAYSIZE(DSType));

    // 혼합 설정
    ImGui::Text("Blend State");
    const char* BSType[] = { "DEFAULT", "ALPHABLEND", "ALPHABLEND_COVERAGE", "ONE_ONE" };
    int BSType_current = static_cast<int>(pGS->GetDomain());
    ImGui::Combo("##BlendState", &BSType_current, BSType, IM_ARRAYSIZE(BSType));
}
