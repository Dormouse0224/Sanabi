#pragma once

#define REGISTER_SCRIPT(Type) CComponentMgr::GetInst()->Register(typeid(Type).name(), []() -> CScript* { return new Type; });

class CScriptInit
{
public:
    static void ScriptInit();
};

