#include "pch.h"
#include "TriggerFunc.h"

#include "Engine/CKeyMgr.h"

bool MainTitle_Bg_Start_Loop(CFSM_State* _Origin, CFSM_State* _Dest)
{
	float time = 1.f;
	if (!_Origin->GetConst<float>(0, &time))
		return false;

	if (time <= 0)
		return true;
	else
		return false;
}

bool Player_Idle_Run(CFSM_State* _Origin, CFSM_State* _Dest)
{	
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::TAP || CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::TAP)
		return true;
	return false;
}

bool Player_Run_Idle(CFSM_State* _Origin, CFSM_State* _Dest)
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::RELEASE || CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::RELEASE)
		return true;
	return false;
}