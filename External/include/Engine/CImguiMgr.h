#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

class CImguiObject;

class CInspector;
class COutliner;
class CContentViewer;

class CImguiMgr
	: public Singleton<CImguiMgr>
{
	SINGLE(CImguiMgr);
private:
	map<string, CImguiObject*>	m_mapImguiObj;

	bool						m_DebugMenuBar;
	bool						m_DemoActive;
	bool						m_AddGameObjectMenuActive;
	bool						m_SaveLevelActive;
	bool						m_CreateMaterialActive;

	CInspector*					m_Inspector;
	COutliner*					m_Outliner;
	CContentViewer*				m_ContentViewer;

public:
	void Init();
	void Progress();
	void AddImguiObj(CImguiObject* _ImguiObj);
	void DebugMenuBar();

	void AddGameObjectMenuPopup();
	void SaveLevelPopup();
	void CreateMaterialPopup();
	void LoadAsset();
	void LoadLevel();

	CInspector* GetInspector() { return m_Inspector; }
	COutliner* GetOutliner() { return m_Outliner; }
};

