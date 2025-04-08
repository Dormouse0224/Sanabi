#pragma once

#include "CScript.h"

typedef CScript* (*CreateFunc)();
typedef void (*ScriptInit)();

class CComponentMgr
	: public Singleton<CComponentMgr>
{
	SINGLE(CComponentMgr);
private:
	std::map<std::string, CreateFunc>	m_Registry;
	ScriptInit							m_ScriptInitFunc;

public:
	void Init();

	void SetScriptInitFunc(ScriptInit _ScriptInitFunc) { m_ScriptInitFunc = _ScriptInitFunc; Init(); }

	void Register(const std::string& className, CreateFunc func);

	CScript* CreateScript(const std::string& className);
	CComponent* CreateComp(COMPONENT_TYPE _Type);
   

};

