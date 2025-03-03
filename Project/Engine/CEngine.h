#pragma once

class CEngine
	: public Singleton<CEngine>
{
	SINGLE(CEngine);
private:
	HINSTANCE	m_hInst;		// 프로세스 주소
	HWND		m_hMainWnd;		// 메인 윈도우 핸들
	Vec2		m_vResolution;	// 메인 윈도우 해상도

public:
	int Init(HINSTANCE _Inst, UINT _Width, UINT _Height);
	void Progress();

	Vec2 GetResolution() { return m_vResolution; }
	HWND GetMainWndHwnd() { return m_hMainWnd; }

private:
	void LiveObjectReport();
	int CreateMainWindow();
};

