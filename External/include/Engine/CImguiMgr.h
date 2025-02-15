#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class CImguiObject;

class CImguiMgr
	: public Singleton<CImguiMgr>
{
	SINGLE(CImguiMgr);
private:
	map<string, CImguiObject*> m_mapImguiObj;


public:
	void Init();
	void Progress();

	void AddImguiObj(CImguiObject* _ImguiObj);
};

