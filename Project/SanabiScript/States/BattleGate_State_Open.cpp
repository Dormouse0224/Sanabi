#include "pch.h"
#include "BattleGate_State_Open.h"

#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CFlipbookRender.h"

#include "Scripts/BattleGateScript.h"

BattleGate_State_Open::BattleGate_State_Open()
{
}

BattleGate_State_Open::BattleGate_State_Open(const BattleGate_State_Open& _Other)
{
}

BattleGate_State_Open::~BattleGate_State_Open()
{
}

void BattleGate_State_Open::Tick()
{
	// 게이트 개방 애니메이션이 끝났는지 검사
	if (m_Opening && m_Owner->GetOwner()->FlipbookRender()->IsFinish())
	{
		m_Opening = false;
		if (m_Gate->GetVert())
		{
			m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateVert_Opened.flip", 10, true);
		}
		else
		{
			m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateHoriz_Opened.flip", 10, true);
		}
	}


}

void BattleGate_State_Open::Begin()
{
	m_Gate = (BattleGateScript*)m_Owner->GetOwner()->FindScript("class BattleGateScript");
	m_Opening = true;
	if (m_Gate->GetVert())
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateVert_Opening.flip", 10, false);
	}
	else
	{
		m_Owner->GetOwner()->FlipbookRender()->Play(L"Flipbook\\BattlegateHoriz_Opening.flip", 10, false);
	}
}

void BattleGate_State_Open::End()
{
}
