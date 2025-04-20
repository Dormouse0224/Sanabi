#include "pch.h"
#include "BattleGateScript.h"

#include "Engine/CPhysxActor.h"
#include "Engine/CImguiMgr.h"
#include "Engine/CLevelMgr.h"
#include "Engine/CLayer.h"
#include "Engine/CListUI.h"

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

	// 최상위 게이트 객체가 하위 게이트 객체를 제어한다
	if (CGameObject* pPatent = GetOwner()->GetParent(); pPatent)
	{
		// 부모가 있는 하위 게이트 객체
		BattleGateScript* pParentGate = (BattleGateScript*)pPatent->FindScript("class BattleGateScript");
		if (!pParentGate)
			return;

		// 상위 게이트 객체의 상태를 복사해옴
		m_bGateUsed = pParentGate->m_bGateUsed;
		m_bClosed = pParentGate->m_bClosed;
	}
	else
	{
		// 부모가 없는 최상위 게이트 객체
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
	// 게이트는 1회만 사용됨
	if (m_bGateUsed)
		return;

	// 이 스크립트를 소유한 게이트 객체의 자식 중, 몬스터 스크립트를 가진 객체에게 신호
	vector<CGameObject*> vecChild = GetOwner()->GetChild();
	for (int i = 0; i < vecChild.size(); ++i)
	{
		if (MonsterScript* pMonSc = (MonsterScript*)vecChild[i]->FindScript("class MonsterScript"); pMonSc)
		{
			pMonSc->SetWakeUp(true);
		}
	}

	//  게이트 폐쇄
	m_bClosed = true;
}