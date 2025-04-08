#pragma once

class CLevel;
class CGameObject;

class CLevelMgr
	: public Singleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
private:
	CLevel*	m_CurLevel;
	CLevel*	m_PlayStartLevel;
	bool			m_LevelModified;


public:
	void Init();
	void Progress();

	CLevel* GetCurrentLevel() { return m_CurLevel; }
	bool GetLevelModified() { return m_LevelModified; }
	const CLevel* GetPlayStartLevel() { return m_PlayStartLevel; }

	void SetCurrentLevel(CLevel* _Level) { m_CurLevel = _Level; }
	void SetLevelModified(bool _b) { m_LevelModified = _b; }
	void SetPlayStartLevel(CLevel* _Level) { m_PlayStartLevel = _Level; }

	// 현재 레벨에 신규 게임 오브젝트 객체를 추가합니다.
	CGameObject* AddGameObject(wstring _Name, LAYER _Layer);
	void ChangeLevel(CLevel* _NextLevel);

};

