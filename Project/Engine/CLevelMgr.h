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

	// 현재 레벨에 신규 게임 오브젝트 객체를 추가합니다.
	void AddGameObject(wstring _Name, LAYER _Layer);
};

