#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class CImguiMgr
	: public Singleton<CImguiMgr>
{
	SINGLE(CImguiMgr);

public:
	void Init();
	void Progress();
};

