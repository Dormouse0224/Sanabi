#pragma once


class CLevelMgr
	: public Singleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
private:
	class CLevel*	m_CurLevel;
	bool			m_LevelUpdated;

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }

	void SetCurrentLevel(CLevel* _Level) { m_CurLevel = _Level; }

public:
	void Init();
	void Progress();

	bool GetLevelUpdated() { return m_LevelUpdated; }

	void SetLevelUpdated(bool _b) { m_LevelUpdated = _b; }

	// ���� ������ �ű� ���� ������Ʈ ��ü�� �߰��մϴ�.
	void AddGameObject(wstring _Name, LAYER _Layer);
};

