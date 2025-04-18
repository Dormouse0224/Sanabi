#include "pch.h"
#include "TriggerFunc.h"

#include "Engine/CKeyMgr.h"
#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CScript.h"
#include "Scripts/PlayerScript.h"

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
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::PRESSED || CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::PRESSED)
		return true;
	return false;
}

bool Player_Idle_Jump(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetAirborne())
		return true;
	return false;
}

bool Player_Idle_Shoot(CFSM_State* _Origin, CFSM_State* _Dest)
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::TAP)
		return true;
	return false;
}

bool Player_Run_Idle(CFSM_State* _Origin, CFSM_State* _Dest)
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::A) == Key_state::NONE && CKeyMgr::GetInst()->GetKeyState(Keyboard::D) == Key_state::NONE)
		return true;
	return false;
}

bool Player_Run_Jump(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetAirborne())
		return true;
	return false;
}

bool Player_Run_Shoot(CFSM_State* _Origin, CFSM_State* _Dest)
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::TAP)
		return true;
	return false;
}

bool Player_Jump_Idle(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (!pScript->GetAirborne())
		return true;
	return false;
}

bool Player_Jump_Climb(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetWallContact())
		return true;
}

bool Player_Jump_Shoot(CFSM_State* _Origin, CFSM_State* _Dest)
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::TAP)
		return true;
	return false;
}

bool Player_Climb_Jump(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (!pScript->GetWallContact())
		return true;
	return false;
}

bool Player_Shoot_Idle(CFSM_State* _Origin, CFSM_State* _Dest)
{
	int grabState = 0, grabFlipbook = 0;
	_Origin->GetConst<int>(1, &grabState);
	_Origin->GetConst<int>(2, &grabFlipbook);
	if (grabState == 0 && grabFlipbook == 1)
		return true;
	return false;
}

bool Player_Shoot_Swing(CFSM_State* _Origin, CFSM_State* _Dest)
{
	int grabState = 0, grabFlipbook = 0;
	_Origin->GetConst<int>(1, &grabState);
	_Origin->GetConst<int>(2, &grabFlipbook);
	if (grabState == 1)
		return true;
	return false;
}

bool Player_Swing_Idle(CFSM_State* _Origin, CFSM_State* _Dest)
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::NONE)
		return true;
	return false;
}
