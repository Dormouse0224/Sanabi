#pragma once

#define REGISTER_SCRIPT(Type) CComponentMgr::GetInst()->RegisterScript(typeid(Type).name(), []() -> CScript* { return new Type; });
#define REGISTER_STATE(Type) CFSMMgr::GetInst()->RegisterState(typeid(Type).name(), []() -> CFSM_State* { return new Type; });
#define REGISTER_TRIGGER(Func) CFSMMgr::GetInst()->RegisterTrigger(#Func, &Func);

class CScriptInit
{
public:
    static void ScriptInit();
    static void FSMInit();
};

