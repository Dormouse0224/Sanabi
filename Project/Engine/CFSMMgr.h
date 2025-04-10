#pragma once

#include "CFSM.h"
#include "CFSM_State.h"

// ���� ���̺귯������ ������ FSM State �� �ڽ� Ŭ������ �������ִ� �Լ��Դϴ�.
typedef CFSM_State* (*StateCreateFunc)();

// StateCreateFunc ���� CComponentMgr �� ������Ʈ���� ������ִ� �Լ��Դϴ�.
typedef void (*FSMInit)();

class CFSMMgr
	: public Singleton<CFSMMgr>
{
	SINGLE(CFSMMgr);
private:
	std::map<std::string, StateCreateFunc>		m_StateRegistry;
	std::map<std::string, TriggerFunc>			m_TriggerRegistry;
	FSMInit										m_FSMInitFunc;

public:
	void Init();

	TriggerFunc GetTriggerFunc(string _FuncName);
	vector<const char*> GetStateRegistryList();
	vector<const char*> GetTriggerRegistryList();

	// Engine ���� ���̺귯������ ���ǵ� State �� ������ִ� �Լ��� �����ϰ�, �ʱ�ȭ�� �����մϴ�.
	void SetFSMInitFunc(FSMInit _ScriptInitFunc) { m_FSMInitFunc = _ScriptInitFunc; Init(); }

	void RegisterState(const std::string& className, StateCreateFunc func);
	void RegisterTrigger(const std::string& funcName, TriggerFunc func);

	CFSM_State* CreateState(const std::string& className);


};

