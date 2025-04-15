#pragma once

#include "CLevel.h"
class CGameObject;

class CLevelMgr
	: public Singleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
private:
	CLevel*					m_CurLevel;
	CLevel*					m_PlayStartLevel;
	map<wstring, CLevel*>	m_mapLevelList;
	bool					m_LevelModified;
	bool					m_LevelModifyChecker;



public:
	void Init();
	void Progress();

	CLevel* GetCurrentLevel() { return m_CurLevel; }
	bool GetLevelModified() { return m_LevelModified; }
	const CLevel* GetPlayStartLevel() { return m_PlayStartLevel; }

	void SetCurrentLevel(CLevel* _Level) { m_CurLevel = _Level; }
	void SetLevelModified() { m_LevelModifyChecker = true; }
	void SetPlayStartLevel(CLevel* _Level) { m_PlayStartLevel = _Level; }

	// �Է¹��� ������ �����Ͽ� ����θ� Ű������ �����մϴ�.
	void AddLevelList(wstring _RelativePath, CLevel* _Level) { m_mapLevelList.insert(make_pair(_RelativePath, _Level->Clone())); }
	// ����θ� Ű������ ����� ������ ã�� ���纻�� ��ȯ�մϴ�.
	CLevel* FindLevel(wstring _RelativePath);
	// ����� ���� �� �ش� Ű������ ��ϵ� ������ �����մϴ�.
	void DeleteLevel(wstring _RelativePath);

	// ���� ������ �ű� ���� ������Ʈ ��ü�� �߰��մϴ�.
	CGameObject* AddGameObject(wstring _Name, LAYER _Layer);
	void ChangeLevel(CLevel* _NextLevel);

	CGameObject* FindObject(LAYER _Layer, wstring _Name);
	CGameObject* DeleteObject(LAYER _Layer, wstring _Name);
	CGameObject* AddObject(LAYER _Layer, wstring _Name);

};

