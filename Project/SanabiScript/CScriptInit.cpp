#include "pch.h"
#include "CScriptInit.h"
#include "Engine\CComponentMgr.h"

// ========================================================
// 이 코드는 CodeGen 코드 생성기에 의해 작성되었습니다.
// Exeptionlist.txt 는 해당 파일 내 이름들을 예외처리합니다.
// ScriptList.txt 는 코드가 생성된 스크립트 목록입니다.
// 코드는 항상 갱신됩니다.
// ========================================================

#include "Scripts\CCameraMoveScript.h"
#include "Scripts\CPlayerScript.h"

void CScriptInit::ScriptInit()
{
	REGISTER_SCRIPT(CCameraMoveScript);
	REGISTER_SCRIPT(CPlayerScript);
}
