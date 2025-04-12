#include "pch.h"
#include "CScript.h"

#include"CImguiMgr.h"

CScript::CScript()
	: CComponent(COMPONENT_TYPE::SCRIPT)
{
}

CScript::CScript(const CScript& _Origin)
	: CComponent(_Origin)
{

}

CScript::~CScript()
{
}

void CScript::Render()
{
	if (ImGui::CollapsingHeader(to_str(GetName()).substr(6).c_str()))
	{
		Render_Script();
		ImGui::Separator();
	}
}

void CScript::Render_Script()
{
	ImGui::Text("Nothing to show...");
}



