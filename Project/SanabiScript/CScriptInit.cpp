#include "pch.h"
#include "CScriptInit.h"
#include "TriggerFunc.h"
#include "Engine\CComponentMgr.h"
#include "Engine\CFSMMgr.h"

// ========================================================
// 이 코드는 CodeGen 코드 생성기에 의해 작성되었습니다.
// Exeptionlist.txt 는 해당 파일 내 이름들을 예외처리합니다.
// ScriptList.txt 는 코드가 생성된 스크립트 목록입니다.
// 코드는 항상 갱신됩니다.
// ========================================================

#include "Scripts\BattleGateScript.h"
#include "Scripts\BulletScript.h"
#include "Scripts\ContainerScript.h"
#include "Scripts\FarBackgroundScript.h"
#include "Scripts\MainTitleBgScript.h"
#include "Scripts\MainTitle_StartBtn_Script.h"
#include "Scripts\MonsterScript.h"
#include "Scripts\PlayerCamScript.h"
#include "Scripts\PlayerScript.h"

#include "States\BattleGate_State_Close.h"
#include "States\BattleGate_State_Open.h"
#include "States\Bullet_State_Burst.h"
#include "States\Bullet_State_Idle.h"
#include "States\MainTitle_Bg_State_Loop.h"
#include "States\MainTitle_Bg_State_Start.h"
#include "States\Player_State_Climb.h"
#include "States\Player_State_Damaged.h"
#include "States\Player_State_Dash.h"
#include "States\Player_State_Death.h"
#include "States\Player_State_ExcWinding.h"
#include "States\Player_State_Holding.h"
#include "States\Player_State_Idle.h"
#include "States\Player_State_Jump.h"
#include "States\Player_State_Run.h"
#include "States\Player_State_Shoot.h"
#include "States\Player_State_Swing.h"
#include "States\Turret_State_Aiming.h"
#include "States\Turret_State_Dead.h"
#include "States\Turret_State_ExcHolded.h"
#include "States\Turret_State_Popup.h"
#include "States\Turret_State_Shooting.h"
#include "States\Turret_State_Sleep.h"

void CScriptInit::ScriptInit()
{
	REGISTER_SCRIPT(BattleGateScript);
	REGISTER_SCRIPT(BulletScript);
	REGISTER_SCRIPT(ContainerScript);
	REGISTER_SCRIPT(FarBackgroundScript);
	REGISTER_SCRIPT(MainTitleBgScript);
	REGISTER_SCRIPT(MainTitle_StartBtn_Script);
	REGISTER_SCRIPT(MonsterScript);
	REGISTER_SCRIPT(PlayerCamScript);
	REGISTER_SCRIPT(PlayerScript);
}

void CScriptInit::FSMInit()
{
	REGISTER_STATE(BattleGate_State_Close);
	REGISTER_STATE(BattleGate_State_Open);
	REGISTER_STATE(Bullet_State_Burst);
	REGISTER_STATE(Bullet_State_Idle);
	REGISTER_STATE(MainTitle_Bg_State_Loop);
	REGISTER_STATE(MainTitle_Bg_State_Start);
	REGISTER_STATE(Player_State_Climb);
	REGISTER_STATE(Player_State_Damaged);
	REGISTER_STATE(Player_State_Dash);
	REGISTER_STATE(Player_State_Death);
	REGISTER_STATE(Player_State_ExcWinding);
	REGISTER_STATE(Player_State_Holding);
	REGISTER_STATE(Player_State_Idle);
	REGISTER_STATE(Player_State_Jump);
	REGISTER_STATE(Player_State_Run);
	REGISTER_STATE(Player_State_Shoot);
	REGISTER_STATE(Player_State_Swing);
	REGISTER_STATE(Turret_State_Aiming);
	REGISTER_STATE(Turret_State_Dead);
	REGISTER_STATE(Turret_State_ExcHolded);
	REGISTER_STATE(Turret_State_Popup);
	REGISTER_STATE(Turret_State_Shooting);
	REGISTER_STATE(Turret_State_Sleep);

	REGISTER_TRIGGER(MainTitle_Bg_Start_Loop);
	REGISTER_TRIGGER(Player_Idle_Run);
	REGISTER_TRIGGER(Player_Idle_Jump);
	REGISTER_TRIGGER(Player_Idle_Shoot);
	REGISTER_TRIGGER(Player_Idle_Damaged);
	REGISTER_TRIGGER(Player_Run_Idle);
	REGISTER_TRIGGER(Player_Run_Jump);
	REGISTER_TRIGGER(Player_Run_Shoot);
	REGISTER_TRIGGER(Player_Run_Damaged);
	REGISTER_TRIGGER(Player_Jump_Idle);
	REGISTER_TRIGGER(Player_Jump_Climb);
	REGISTER_TRIGGER(Player_Jump_Shoot);
	REGISTER_TRIGGER(Player_Jump_Damaged);
	REGISTER_TRIGGER(Player_Climb_Jump);
	REGISTER_TRIGGER(Player_Climb_Damaged);
	REGISTER_TRIGGER(Player_Shoot_Idle);
	REGISTER_TRIGGER(Player_Shoot_Swing);
	REGISTER_TRIGGER(Player_Shoot_ExcWinding);
	REGISTER_TRIGGER(Player_Shoot_Damaged);
	REGISTER_TRIGGER(Player_Swing_Idle);
	REGISTER_TRIGGER(Player_Swing_Damaged);
	REGISTER_TRIGGER(Player_Damaged_Idle);
	REGISTER_TRIGGER(Player_Damaged_Death);
	REGISTER_TRIGGER(Player_ExcWinding_Holding);
	REGISTER_TRIGGER(Player_Holding_Damaged);
	REGISTER_TRIGGER(Player_Holding_Dash);
	REGISTER_TRIGGER(Player_Dash_Idle);
	REGISTER_TRIGGER(Player_Dash_Shoot);
	REGISTER_TRIGGER(BattleGate_Open_Close);
	REGISTER_TRIGGER(BattleGate_Close_Open);
	REGISTER_TRIGGER(Turret_Sleep_Popup);
	REGISTER_TRIGGER(Turret_Popup_Aiming);
	REGISTER_TRIGGER(Turret_Aiming_Shooting);
	REGISTER_TRIGGER(Turret_Aiming_ExcHolded);
	REGISTER_TRIGGER(Turret_Shooting_Aiming);
	REGISTER_TRIGGER(Turret_Shooting_ExcHolded);
	REGISTER_TRIGGER(Turret_ExcHolded_Aiming);
	REGISTER_TRIGGER(Turret_ExcHolded_Dead);
	REGISTER_TRIGGER(Bullet_Idle_Burst);
}

