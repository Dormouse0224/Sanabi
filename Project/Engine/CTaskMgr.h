#pragma once

class CGameObject;

class CTaskMgr
	: public Singleton<CTaskMgr>
{
	SINGLE(CTaskMgr);
private:
	vector<tTask>			m_vecTask;
	vector<CGameObject*>	m_vecGC;

public:
	void Tick();
	void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }
};

