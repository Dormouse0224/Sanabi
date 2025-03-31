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
	/// <summary>
	/// Task 를 추가합니다.
	/// </summary>
	/// <param name="Type">Task Type.</param>
	/// <param name="Param0">CREATE_OBJECT : (CGameObject*) Create Object / DELETE_OBJECT : (CGameObject*) Delete Object / CHANGE_LEVEL : </param>
	/// <param name="Param1">CREATE_OBJECT : (int) Layer Index / DELETE_OBJECT : No used / CHANGE_LEVEL : </param>
	void AddTask(TASK_TYPE Type, DWORD_PTR Param0, DWORD_PTR Param1);
	
};

