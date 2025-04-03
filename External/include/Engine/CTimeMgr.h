#pragma once

class CTimeMgr
	: public Singleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;

	UINT			m_FPS;
	float			m_DT;		// Delta Time : ������ ���� �ð�, 1 ������ �����ϴµ� �ɸ��� �ð�
	float			m_EngineDT;
	float			m_Time;		
	float			m_EngineTime;

public:
	void Init();
	void Tick();

public:
	float GetDeltaTime() { return m_DT; }
	float GetEngineDeltaTime() { return m_EngineDT; }
};

