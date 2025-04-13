#pragma once

class CCamera;
class CGameObject;

class CRenderMgr
	: public Singleton<CRenderMgr>
{
	SINGLE(CRenderMgr);
private:
	// ���� �������� �������� �������� ī�޶� ����. ������ ������ �� ī�޶� ������Ʈ�� ������ ������Ʈ ��ġ ��,
	// SetPriority �Լ����� ī�޶��� Priority ���� �Է¹ް� RegisterCamera �Լ��� ���� �� ���Ϳ� ��ϵȴ�.
	vector<CCamera*>		m_vecCam;
	CGameObject*			m_DebugObject;
	CGameObject*			m_EditorCam;

	vector<tDebugShapeInfo> m_vecDebugInfo;


public:
	void RegisterCamera(CCamera* _Cam, int _Priority);
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecDebugInfo.push_back(_info); }
	void ClearCamera() { m_vecCam.clear(); }
	void ResetEditorCamPos();

public:
	void Init();
	void Tick();

private:
	void Render();
	void DebugRender();
};

