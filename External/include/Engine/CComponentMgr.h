#pragma once

#include "CScript.h"

typedef CScript* (*CreateFunc)();

class CComponentMgr
	: public Singleton<CComponentMgr>
{
	SINGLE(CComponentMgr);
private:
	std::map<std::string, CreateFunc> m_Registry;

public:
	void Register(const std::string& className, CreateFunc func);

	CScript* CreateScript(const std::string& className);
	CComponent* CreateComp(COMPONENT_TYPE _Type);
   
};

