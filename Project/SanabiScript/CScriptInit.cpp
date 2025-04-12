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

