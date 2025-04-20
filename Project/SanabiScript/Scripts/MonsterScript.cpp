#include "pch.h"
#include "MonsterScript.h"

MonsterScript::MonsterScript()
	: CScript()
	, m_bWakeUp(false)
	, m_ExcHolded(false)
	, m_Dead(false)
{
}

MonsterScript::MonsterScript(const MonsterScript& _Origin)
	: CScript(_Origin)
	, m_bWakeUp(false)
	, m_ExcHolded(false)
	, m_Dead(false)
{
}

MonsterScript::~MonsterScript()
{
}

void MonsterScript::Tick()
{
}

int MonsterScript::Save(fstream& _Stream)
{
	return S_OK;
}

int MonsterScript::Load(fstream& _Stream)
{
	return S_OK;
}
