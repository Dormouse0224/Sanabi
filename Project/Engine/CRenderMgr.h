#pragma once

class CCamera;
class CGameObject;
class CMesh;
class CMaterial;
class CMRT;

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
	CGameObject*			m_UICam;
	CCamera*				m_CurrentCam;

	vector<tDebugShapeInfo> m_vecDebugInfo;

	CMRT*					m_arrMRT[MRT_END];

	AssetPtr<CMesh>			m_MergeMesh;
	AssetPtr<CMaterial>		m_MergeMtrl;



public:
	void RegisterCamera(CCamera* _Cam, int _Priority);
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecDebugInfo.push_back(_info); }
	void ClearCamera() { m_vecCam.clear(); }
	void ResetEditorCamPos();
	CGameObject* GetUICam() { return m_UICam; }
	CCamera* GetCurrentCam() { return m_CurrentCam; }

public:
	void Init();
	void Tick();

private:
	void Render();
	void UIRender();
	void DebugRender();
	void MergeRender();

	int CreateBackBufferView();
	int CreateMRT();
	void ClearMRT();
};

