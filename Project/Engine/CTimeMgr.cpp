#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"

CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_FPS(0)
	, m_DT(0.f)
	, m_Time(0.f)
{
}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	// 1초에 1000 카운팅 -> 초당 카운팅 양이 너무 적음
	//GetTickCount();

	// 프레임과 프레임 사이의 시간
	QueryPerformanceFrequency(&m_Frequency); // 초당 카운팅 값 (천만)
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::Tick()
{
	// 현재시점 Count 값
	QueryPerformanceCounter(&m_CurCount);

	m_DT = (float)(m_CurCount.QuadPart - m_PrevCount.QuadPart) / (float)m_Frequency.QuadPart;

	m_PrevCount = m_CurCount;

	++m_FPS;
	m_Time += m_DT;

	if (1.f <= m_Time)
	{
		wchar_t buff[255] = {};
		swprintf_s(buff, 255, L"FPS : %d, DeltaTime : %f", m_FPS, m_DT);
		SetWindowText(CEngine::GetInst()->GetMainWndHwnd(), buff);

		m_FPS = 0;
		m_Time -= 1.f;
	}
}
