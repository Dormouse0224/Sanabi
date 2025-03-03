#pragma once

class CCamera;
class CGameObject;

class CRenderMgr
	: public Singleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	vector<CCamera*>		m_vecCam;
	CGameObject*			m_DebugObject;

	vector<tDebugShapeInfo> m_vecDebugInfo;


public:
	void RegisterCamera(CCamera* _Cam, int _Priority);
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecDebugInfo.push_back(_info); }

public:
	void Init();
	void Tick();

private:
	void Render();
	void DebugRender();
};

