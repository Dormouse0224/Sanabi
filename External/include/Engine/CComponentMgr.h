#pragma once

#include "CScript.h"

// ���� ���̺귯������ ������ ��ũ��Ʈ�� �ڽ� Ŭ������ �������ִ� �Լ��Դϴ�.
typedef CScript* (*ScriptCreateFunc)();

// ScriptCreateFunc ���� CComponentMgr �� ������Ʈ���� ������ִ� �Լ��Դϴ�.
typedef void (*ScriptInit)();

class CComponentMgr
	: public Singleton<CComponentMgr>
{
	SINGLE(CComponentMgr);
private:
	std::map<std::string, ScriptCreateFunc>		m_SrciptRegistry;
	ScriptInit									m_ScriptInitFunc;

public:
	void Init();

	// Engine ���� ���̺귯������ ���ǵ� ��ũ��Ʈ�� ������ִ� �Լ��� �����ϰ�, �ʱ�ȭ�� �����մϴ�.
	void SetScriptInitFunc(ScriptInit _ScriptInitFunc) { m_ScriptInitFunc = _ScriptInitFunc; Init(); }

	void RegisterScript(const std::string& className, ScriptCreateFunc func);

	CScript* CreateScript(const std::string& className);
	CComponent* CreateComp(COMPONENT_TYPE _Type);
   

};

