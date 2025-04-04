#include "pch.h"
#include "CTimeMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CEngine.h"

CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_LocalTime()
	, m_FPS(0)
	, m_DT(0.f)
	, m_Time(0.f)
	, m_EngineDT(0.f)
	, m_EngineTime(0.f)
{
}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	// 1�ʿ� 1000 ī���� -> �ʴ� ī���� ���� �ʹ� ����
	//GetTickCount();

	// �����Ӱ� ������ ������ �ð�
	QueryPerformanceFrequency(&m_Frequency); // �ʴ� ī���� �� (õ��)
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::Tick()
{
	// ���� �ð� ���
	GetLocalTime(&m_LocalTime);

	// ������� Count ��
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDT = m_DT = (float)(m_CurCount.QuadPart - m_PrevCount.QuadPart) / (float)m_Frequency.QuadPart;

	m_PrevCount = m_CurCount;

	++m_FPS;

	// Level State �� ���� DT Ȱ��ȭ/��Ȱ��ȭ, m_Time �� DT �����ð��̱� ������ STOP �� ��� �������� ����
	LEVEL_STATE State = LEVEL_STATE::STOP;
	if (CLevelMgr::GetInst()->GetCurrentLevel())
		State = CLevelMgr::GetInst()->GetCurrentLevel()->GetState();

	if (LEVEL_STATE::PLAY != State)
	{
		m_DT = 0.f;
	}
	else
	{
		m_Time += m_DT;
	}

	m_EngineTime += m_EngineDT;

	static float OneSecond = 0.f;
	OneSecond += m_EngineDT;
	if (1.f <= OneSecond)
	{
		wchar_t buff[255] = {};
		swprintf_s(buff, 255, L"FPS : %d, DeltaTime : %f", m_FPS, m_DT);
		SetWindowText(CEngine::GetInst()->GetMainWndHwnd(), buff);

		m_FPS = 0;
		OneSecond -= 1.f;
	}

	// ���� ��� ������ ����
	g_global.g_DT = m_DT;
	g_global.g_EngineDT = m_EngineDT;
	g_global.g_Time = m_Time;
	g_global.g_EngineTime = m_EngineTime;
}
