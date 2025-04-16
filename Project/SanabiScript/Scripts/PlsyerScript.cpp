#include "pch.h"
#include "PlsyerScript.h"

#include "Engine/CPhysxActor.h"

PlsyerScript::PlsyerScript()
	: CScript()
	, m_bInit(false)
	, m_bAirborne(false)
{
}

PlsyerScript::PlsyerScript(const PlsyerScript& _Origin)
	: CScript(_Origin)
	, m_bInit(false)
	, m_bAirborne(false)
{
}

PlsyerScript::~PlsyerScript()
{
}

void PlsyerScript::Tick()
{
	if (!m_bInit)
		Init();


}

void PlsyerScript::Init()
{
	GetOwner()->PhysxActor()->SetContactBegin(this, (ContactFunc)(&PlsyerScript::ContactBegin));
	GetOwner()->PhysxActor()->SetContactTick(this, (ContactFunc)(&PlsyerScript::ContacTick));
	GetOwner()->PhysxActor()->SetContactEnd(this, (ContactFunc)(&PlsyerScript::ContactEnd));
	m_bInit = true;
}

void PlsyerScript::ContactBegin(CGameObject* _Other)
{
	int a = 0;
}

void PlsyerScript::ContacTick(CGameObject* _Other)
{
	int a = 0;
}

void PlsyerScript::ContactEnd(CGameObject* _Other)
{
	int a = 0;
}

