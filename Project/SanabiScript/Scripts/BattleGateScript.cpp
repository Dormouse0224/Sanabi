#include "pch.h"
#include "BattleGateScript.h"

#include "Engine/CPhysxActor.h"
#include "Engine/CImguiMgr.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLayer.h"
#include "Engine/CListUI.h"
#include "Engine/CTransform.h"

#include "Scripts/MonsterScript.h"

BattleGateScript::BattleGateScript()
	: CScript()
	, m_bInit(false)
	, m_bGateUsed(false)
	, m_bClosed(false)
{
}

BattleGateScript::BattleGateScript(const CScript& _Origin)
	: CScript()
	, m_bInit(false)
	, m_bGateUsed(false)
	, m_bClosed(false)
{
}

BattleGateScript::~BattleGateScript()
{
}

void BattleGateScript::Tick()
{
	if (!m_bInit && CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
		Init();

	// �ֻ��� ����Ʈ ��ü�� ���� ����Ʈ ��ü�� �����Ѵ�
	if (CGameObject* pPatent = GetOwner()->GetParent(); pPatent)
	{
		// �θ� �ִ� ���� ����Ʈ ��ü
		BattleGateScript* pParentGate = (BattleGateScript*)pPatent->FindScript("class BattleGateScript");
		if (!pParentGate)
			return;

		// ���� ����Ʈ ��ü�� ���¸� �����ؿ�
		m_bGateUsed = pParentGate->m_bGateUsed;
		m_bClosed = pParentGate->m_bClosed;
	}
	else
	{
		// �θ� ���� �ֻ��� ����Ʈ ��ü
		if (m_bClosed)
		{
			// ����Ʈ�� ��� ������ ���, �ڽ� ��ü �� ���� ��ũ��Ʈ�� ������ ��ü�� Dead �������� Ȯ��
			vector<CGameObject*> vecChild = GetOwner()->GetChild();
			bool DeadCheck = true;
			for (int i = 0; i < vecChild.size(); ++i)
			{
				if (MonsterScript* pMonSc = (MonsterScript*)vecChild[i]->FindScript("class MonsterScript"); pMonSc)
				{
					if (!pMonSc->GetDead())
					{
						DeadCheck = false;
						break;
					}
				}
			}
			if (DeadCheck)
				m_bClosed = false;
		}

	}
}

int BattleGateScript::Save(fstream& _Stream)
{
	_Stream.write(reinterpret_cast<char*>(&m_bVert), sizeof(bool));
	return S_OK;
}

int BattleGateScript::Load(fstream& _Stream)
{
	_Stream.read(reinterpret_cast<char*>(&m_bVert), sizeof(bool));
	return S_OK;
}

void BattleGateScript::Render_Script()
{
	ImGui::Checkbox("Is Vertical?", &m_bVert);
}


void BattleGateScript::Init()
{
	m_bInit = true;

	if (GetOwner()->GetParent() == nullptr)
		GetOwner()->PhysxActor()->SetTriggerEnd(this, (PxTriggerFunc)&BattleGateScript::Trigger);
}

void BattleGateScript::Trigger(CGameObject* _Other)
{
	// ����Ʈ�� 1ȸ�� ����
	if (m_bGateUsed)
		return;

	Vec3 dir = _Other->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();

	if (!((dir.x > 0 && m_bVert) || (dir.y > 0 && !m_bVert)))
		return;

	// �� ��ũ��Ʈ�� ������ ����Ʈ ��ü�� �ڽ� ��, ���� ��ũ��Ʈ�� ���� ��ü���� ��ȣ
	vector<CGameObject*> vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (MonsterScript* pMonSc = (MonsterScript*)vecChild[i]->FindScript("class MonsterScript"); pMonSc)
		{
			pMonSc->SetWakeUp(true);
		}
	}

	//  ����Ʈ ���
	m_bClosed = true;
}