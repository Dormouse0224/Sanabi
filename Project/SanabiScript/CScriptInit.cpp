#include "pch.h"
#include "CScriptInit.h"
#include "Engine\CComponentMgr.h"

// ========================================================
// �� �ڵ�� CodeGen �ڵ� �����⿡ ���� �ۼ��Ǿ����ϴ�.
// Exeptionlist.txt �� �ش� ���� �� �̸����� ����ó���մϴ�.
// ScriptList.txt �� �ڵ尡 ������ ��ũ��Ʈ ����Դϴ�.
// �ڵ�� �׻� ���ŵ˴ϴ�.
// ========================================================

#include "Scripts\CCameraMoveScript.h"
#include "Scripts\CPlayerScript.h"

void CScriptInit::ScriptInit()
{
	REGISTER_SCRIPT(CCameraMoveScript);
	REGISTER_SCRIPT(CPlayerScript);
}
