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

#include "Scripts\CCameraMoveScript.h"
#include "Scripts\CPlayerScript.h"
#include "Scripts\MainTitleBgScript.h"

#include "States\MainTitle_Bg_State_Loop.h"
#include "States\MainTitle_Bg_State_Start.h"

void CScriptInit::ScriptInit()
{
	REGISTER_SCRIPT(CCameraMoveScript);
	REGISTER_SCRIPT(CPlayerScript);
	REGISTER_SCRIPT(MainTitleBgScript);
}

void CScriptInit::FSMInit()
{
	REGISTER_STATE(MainTitle_Bg_State_Loop);
	REGISTER_STATE(MainTitle_Bg_State_Start);

	REGISTER_TRIGGER(MainTitle_Bg_Start_Loop);
}

