#include "pch.h"
#include "FarBackgroundScript.h"

#include "Engine/CRenderMgr.h"
#include "Engine/CCamera.h"
#include "Engine/CTransform.h"
#include "Engine/CEngine.h"
#include "Engine/CImguiMgr.h"

FarBackgroundScript::FarBackgroundScript()
	: CScript()
	, m_Scale(1.f)
	, m_Offset(Vec2(0, 0))
{
}

FarBackgroundScript::FarBackgroundScript(const FarBackgroundScript& _Origin)
	: CScript(_Origin)
	, m_Scale(_Origin.m_Scale)
	, m_Offset(_Origin.m_Offset)
{
}

FarBackgroundScript::~FarBackgroundScript()
{
}

void FarBackgroundScript::Tick()
{
	CCamera* pCam = CRenderMgr::GetInst()->GetCurrentCam();
	if (pCam)
	{
		Vec3 pCamPos = pCam->GetOwner()->Transform()->GetRelativePos();
		GetOwner()->Transform()->SetRelativePos(pCamPos.x + m_Offset.x, pCamPos.y + m_Offset.y, GetOwner()->Transform()->GetRelativePos().z);
		GetOwner()->Transform()->SetRelativeScale(CEngine::GetInst()->GetResolution().x * m_Scale, CEngine::GetInst()->GetResolution().y * m_Scale, 0);
	}
}

int FarBackgroundScript::Save(fstream& _Stream)
{
	_Stream.write(reinterpret_cast<char*>(&m_Scale), sizeof(float));
	_Stream.write(reinterpret_cast<char*>(&m_Offset), sizeof(Vec2));
	return S_OK;
}

int FarBackgroundScript::Load(fstream& _Stream)
{
	_Stream.read(reinterpret_cast<char*>(&m_Scale), sizeof(float));
	_Stream.read(reinterpret_cast<char*>(&m_Offset), sizeof(Vec2));
	return S_OK;
}

void FarBackgroundScript::Render_Script()
{
	ImGui::Text("Scale");
	ImGui::DragFloat("##Scale", &m_Scale, 0.01f);

	ImGui::Text("Offset");
	float fOff[2] = { m_Offset.x, m_Offset.y };
	if (ImGui::DragFloat2("##Offset", fOff, 0.01f))
	{
		m_Offset = Vec2(fOff[0], fOff[1]);
	}
}
