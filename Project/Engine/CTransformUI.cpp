#include "pch.h"
#include "CTransformUI.h"

#include "CGameObject.h"
#include "CTransform.h"

CTransformUI::CTransformUI()
    : CComponentUI(COMPONENT_TYPE::TRANSFORM)
{
}

CTransformUI::~CTransformUI()
{
}

void CTransformUI::Update()
{
}

void CTransformUI::Render()
{
    CTransform* pTransform = dynamic_cast<CTransform*>(m_Target->GetComponent(m_Type));

    if (pTransform == nullptr)
        return;

    float tab = 120.f;

    ImGui::Text("Position");
    ImGui::SameLine(tab);
    float Position[3] = { pTransform->GetRelativePos().x,  pTransform->GetRelativePos().y,  pTransform->GetRelativePos().z };
    if (ImGui::DragFloat3("##Position", Position))
    {
        pTransform->SetRelativePos(Vec3(Position));
    }

    ImGui::Text("Scale");
    ImGui::SameLine(tab);
    float Scale[3] = { pTransform->GetRelativeScale().x,  pTransform->GetRelativeScale().y,  pTransform->GetRelativeScale().z };
    if (ImGui::DragFloat3("##Scale", Scale))
    {
        pTransform->SetRelativeScale(Vec3(Scale));
    }

    ImGui::Text("Rotation");
    ImGui::SameLine(tab);
    float Rotation[3] = { pTransform->GetRelativeRotation().x,  pTransform->GetRelativeRotation().y,  pTransform->GetRelativeRotation().z };
    if (ImGui::DragFloat3("##Rotation", Rotation))
    {
        pTransform->SetRelativeRotation(Vec4(Vec3(Rotation), pTransform->GetRelativeRotation().w));
    }

    ImGui::Text("Independent Scale");
    ImGui::SameLine(tab);
    bool bIS = pTransform->GetIndependentScale();
    if (ImGui::Checkbox("##IndependentScale", &bIS))
    {
        pTransform->SetIndependentScale(bIS);
    }
}
