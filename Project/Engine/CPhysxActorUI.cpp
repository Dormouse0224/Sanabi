#include "pch.h"
#include "CPhysxActorUI.h"

#include "CGameObject.h"
#include "CPhysxActor.h"

CPhysxActorUI::CPhysxActorUI()
	: CComponentUI(COMPONENT_TYPE::PHYSXACTOR)
{
}

CPhysxActorUI::~CPhysxActorUI()
{
}

void CPhysxActorUI::Update_Com()
{
}

void CPhysxActorUI::Render_Com()
{
	float tab = 130;

	if (m_Target->PhysxActor()->GetRigidType() == RIGID_TYPE::NONE)
	{
		ImGui::Text("Rigid type is NONE");
		return;
	}
	
	PxRigidActor* pBody = m_Target->PhysxActor()->GetRigidBody();

	// Rigid Type
	ImGui::Text("Rigid Type");
	ImGui::SameLine(tab);
	int TypeIdx = (int)m_Target->PhysxActor()->GetRigidType();
	const char* const Type[] = { "KINEMATIC", "DYNAMIC", "STATIC" };
	if (ImGui::Combo("##RigidType", &TypeIdx, Type, 3))
	{
		m_Target->PhysxActor()->SetRigidType((RIGID_TYPE)TypeIdx);
		pBody = m_Target->PhysxActor()->GetRigidBody();
	}

	// Density
	ImGui::Text("Density");
	ImGui::SameLine(tab);
	float Dens = m_Target->PhysxActor()->GetDensity();
	if (ImGui::DragFloat("##Density", &Dens, 1.f, 0.1f))
	{
		m_Target->PhysxActor()->SetDensity(Dens);
	}

	if ((RIGID_TYPE)TypeIdx == RIGID_TYPE::DYNAMIC)
	{
		PxRigidDynamic* pDynamicBody = (PxRigidDynamic*)pBody;
		// LockFlag
		UINT Flags = pDynamicBody->getRigidDynamicLockFlags();
		bool LockState[6] = { Flags & LINEAR_X, Flags & LINEAR_Y, Flags & LINEAR_Z, Flags & ANGULAR_X, Flags & ANGULAR_Y, Flags & ANGULAR_Z };
		ImGui::Text("Lock Flags");
		if (ImGui::BeginTable("##LockFlags", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			for (int i = 0; i < 6; i++)
			{
				ImGui::TableNextColumn();
				if (ImGui::Selectable(to_str(LOCK_FLAG_WSTR[i]).c_str(), &LockState[i]))
				{
					m_Target->PhysxActor()->CkeckLockFlag((LOCK_FLAG)(1 << i));
				}
			}
			ImGui::EndTable();
		}
		
		// Gravity
		ImGui::Text("Gravity");
		bool bGravity = !(pDynamicBody->getActorFlags() & PxActorFlag::eDISABLE_GRAVITY);
		if (ImGui::Checkbox("##Gravity", &bGravity))
		{
			pDynamicBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !bGravity);
		}


		// Linear Velocity
		ImGui::Text("Linear Velocity");
		float LinVel[3] = { pDynamicBody->getLinearVelocity().x, pDynamicBody->getLinearVelocity().y, pDynamicBody->getLinearVelocity().z };
		if (ImGui::DragFloat3("##LinearVelocity", LinVel))
		{
			pDynamicBody->setLinearVelocity(PxVec3(LinVel[0], LinVel[1], LinVel[2]));
		}

		// Angular Velocity
		ImGui::Text("Angular Velocity");
		float AngVel[3] = { pDynamicBody->getAngularVelocity().x, pDynamicBody->getAngularVelocity().y, pDynamicBody->getAngularVelocity().z };
		if (ImGui::DragFloat3("##AngularVelocity", AngVel))
		{
			pDynamicBody->setAngularVelocity(PxVec3(AngVel[0], AngVel[1], AngVel[2]));
		}
	}
}
