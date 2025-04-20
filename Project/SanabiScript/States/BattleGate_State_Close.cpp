#include "pch.h"
#include "BattleGate_State_Close.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"

#include "Scripts/BattleGateScript.h"

BattleGate_State_Close::BattleGate_State_Close()
{
}

BattleGate_State_Close::BattleGate_State_Close(const BattleGate_State_Close& _Other)
{
}

BattleGate_State_Close::~BattleGate_State_Close()
{
}

void BattleGate_State_Close::Tick()
{
	// ����Ʈ ���� �ִϸ��̼��� �������� �˻�
	if (m_Closing && m_Owner->GetOwner()->FlipbookRender()->IsFinish())
	{
		m_Closing = false;
		if (m_Gate->GetVert())
		{
			m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateVert_Closed.flip", 10, true);
		}
		else
		{
			m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateHoriz_Closed.flip", 10, true);
		}
	}
}

void BattleGate_State_Close::Begin()
{
	m_Gate = (BattleGateScript*)m_Owner->GetOwner()->FindScript("class BattleGateScript");
	m_Closing = true;
	if (m_Gate->GetVert())
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateVert_Closing.flip", 10, false);
	}
	else
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateHoriz_Closing.flip", 10, false);
	}
}

void BattleGate_State_Close::End()
{
}
