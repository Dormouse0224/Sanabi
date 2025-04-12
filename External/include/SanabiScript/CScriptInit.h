#pragma once

#define REGISTER_SCRIPT(Type) CComponentMgr::GetInst()->RegisterScript(typeid(Type).name()\
, []() -> CScript* { CScript* pScript = new Type; pScript->SetName(to_wstr(typeid(Type).name())); return pScript; });
#define REGISTER_STATE(Type) CFSMMgr::GetInst()->RegisterState(typeid(Type).name()\
, []() -> CFSM_State* { CFSM_State* pState = new Type; pState->SetName(to_wstr(typeid(Type).name())); return pState; });
#define REGISTER_TRIGGER(Func) CFSMMgr::GetInst()->RegisterTrigger(#Func, &Func);

class CScriptInit
{
public:
    static void ScriptInit();
    static void FSMInit();
};

