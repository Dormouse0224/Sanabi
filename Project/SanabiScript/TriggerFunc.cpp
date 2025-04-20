#include "pch.h"
#include "TriggerFunc.h"

#include "Engine/CKeyMgr.h"
#include "Engine/CFSM.h"
#include "Engine/CGameObject.h"
#include "Engine/CScript.h"

#include "Scripts/PlayerScript.h"
#include "Scripts/BattleGateScript.h"
#include "Scripts/MonsterScript.h"
#include "Scripts/BulletScript.h"

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

bool Player_Idle_Damaged(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetDamaged())
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

bool Player_Run_Damaged(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetDamaged())
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

bool Player_Jump_Damaged(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetDamaged())
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

bool Player_Climb_Damaged(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetDamaged())
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

bool Player_Shoot_Damaged(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetDamaged())
		return true;
	return false;
}

bool Player_Swing_Idle(CFSM_State* _Origin, CFSM_State* _Dest)
{
	if (CKeyMgr::GetInst()->GetKeyState(Keyboard::MOUSE_LBTN) == Key_state::NONE)
		return true;
	return false;
}

bool Player_Swing_Damaged(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetDamaged())
		return true;
	return false;
}

bool Player_Damaged_Idle(CFSM_State* _Origin, CFSM_State* _Dest)
{
	int IsDmgFin = 0;
	_Origin->GetConst<int>(2, &IsDmgFin);
	if (IsDmgFin == 1)
		return true;
	return false;
}

bool Player_Damaged_Death(CFSM_State* _Origin, CFSM_State* _Dest)
{
	PlayerScript* pScript = static_cast<PlayerScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class PlayerScript"));
	if (pScript->GetHP() <= 0)
		return true;
	return false;
}

bool BattleGate_Open_Close(CFSM_State* _Origin, CFSM_State* _Dest)
{
	BattleGateScript* pScript = static_cast<BattleGateScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class BattleGateScript"));
	if (pScript->GetClosed())
		return true;
	return false;
}

bool BattleGate_Close_Open(CFSM_State* _Origin, CFSM_State* _Dest)
{
	BattleGateScript* pScript = static_cast<BattleGateScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class BattleGateScript"));
	if (!pScript->GetClosed())
		return true;
	return false;
}

bool Turret_Sleep_Popup(CFSM_State* _Origin, CFSM_State* _Dest)
{
	MonsterScript* pScript = static_cast<MonsterScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class MonsterScript"));
	if (pScript->GetWakeUp())
		return true;
	return false;
}

bool Turret_Popup_Aiming(CFSM_State* _Origin, CFSM_State* _Dest)
{
	int IsFlipbookFin = 0;
	_Origin->GetConst<int>(0, &IsFlipbookFin);
	if (IsFlipbookFin == 1)
		return true;
	return false;
}

bool Turret_Aiming_Shooting(CFSM_State* _Origin, CFSM_State* _Dest)
{
	int IsAimFin = 0;
	_Origin->GetConst<int>(0, &IsAimFin);
	if (IsAimFin == 1)
		return true;
	return false;
}

bool Turret_Aiming_ExcHolded(CFSM_State* _Origin, CFSM_State* _Dest)
{
	MonsterScript* pScript = static_cast<MonsterScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class MonsterScript"));
	if (pScript->GetExcHolded())
		return true;
	return false;
}

bool Turret_Shooting_Aiming(CFSM_State* _Origin, CFSM_State* _Dest)
{
	int IsShootFin = 0;
	_Origin->GetConst<int>(0, &IsShootFin);
	if (IsShootFin == 1)
		return true;
	return false;
}

bool Turret_Shooting_ExcHolded(CFSM_State* _Origin, CFSM_State* _Dest)
{
	MonsterScript* pScript = static_cast<MonsterScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class MonsterScript"));
	if (pScript->GetExcHolded())
		return true;
	return false;
}

bool Turret_ExcHolded_Aiming(CFSM_State* _Origin, CFSM_State* _Dest)
{
	MonsterScript* pScript = static_cast<MonsterScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class MonsterScript"));
	if (!pScript->GetExcHolded() && !pScript->GetDead())
		return true;
	return false;
}

bool Turret_ExcHolded_Dead(CFSM_State* _Origin, CFSM_State* _Dest)
{
	MonsterScript* pScript = static_cast<MonsterScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class MonsterScript"));
	if (!pScript->GetExcHolded() && pScript->GetDead())
		return true;
	return false;
}

bool Bullet_Idle_Burst(CFSM_State* _Origin, CFSM_State* _Dest)
{
	BulletScript* pScript = static_cast<BulletScript*>(_Origin->GetOwner()->GetOwner()->FindScript("class BulletScript"));
	if (pScript->GetDead())
		return true;
	return false;
}
