#include "pch.h"
#include "CScriptInit.h"
#include "TriggerFunc.h"
#include "Engine\CComponentMgr.h"
#include "Engine\CFSMMgr.h"

// ========================================================
// �� �ڵ�� CodeGen �ڵ� �����⿡ ���� �ۼ��Ǿ����ϴ�.
// Exeptionlist.txt �� �ش� ���� �� �̸����� ����ó���մϴ�.
// ScriptList.txt �� �ڵ尡 ������ ��ũ��Ʈ ����Դϴ�.
// �ڵ�� �׻� ���ŵ˴ϴ�.
// ========================================================

#include "Scripts\MainTitleBgScript.h"
#include "Scripts\MainTitle_StartBtn_Script.h"
#include "Scripts\PlayerCamScript.h"
#include "Scripts\PlayerScript.h"

#include "States\MainTitle_Bg_State_Loop.h"
#include "States\MainTitle_Bg_State_Start.h"
#include "States\Player_State_Climb.h"
#include "States\Player_State_Idle.h"
#include "States\Player_State_Jump.h"
#include "States\Player_State_Run.h"

void CScriptInit::ScriptInit()
{
	REGISTER_SCRIPT(MainTitleBgScript);
	REGISTER_SCRIPT(MainTitle_StartBtn_Script);
	REGISTER_SCRIPT(PlayerCamScript);
	REGISTER_SCRIPT(PlayerScript);
}

void CScriptInit::FSMInit()
{
	REGISTER_STATE(MainTitle_Bg_State_Loop);
	REGISTER_STATE(MainTitle_Bg_State_Start);
	REGISTER_STATE(Player_State_Climb);
	REGISTER_STATE(Player_State_Idle);
	REGISTER_STATE(Player_State_Jump);
	REGISTER_STATE(Player_State_Run);

	REGISTER_TRIGGER(MainTitle_Bg_Start_Loop);
	REGISTER_TRIGGER(Player_Idle_Run);
	REGISTER_TRIGGER(Player_Idle_Jump);
	REGISTER_TRIGGER(Player_Run_Idle);
	REGISTER_TRIGGER(Player_Run_Jump);
	REGISTER_TRIGGER(Player_Jump_Idle);
	REGISTER_TRIGGER(Player_Jump_Climb);
	REGISTER_TRIGGER(Player_Climb_Jump);
}

