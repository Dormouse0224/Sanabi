#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"
#include "CGameObject.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"

CLevel::CLevel()
	: m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		m_Layer[i] = new CLayer;
		m_Layer[i]->SetLayerIdx(i);
	}
}

CLevel::CLevel(const CLevel& _Other)
	: CEntity(_Other)
	, m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		m_Layer[i] = _Other.m_Layer[i]->Clone();
	}
}

CLevel::~CLevel()
{
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		delete m_Layer[i];
	}
}

void CLevel::Begin()
{
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		m_Layer[i]->Begin();
	}
}

void CLevel::Tick()
{
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		m_Layer[i]->Tick();
	}
}

void CLevel::FinalTick()
{
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		m_Layer[i]->FinalTick();
	}
}

void CLevel::SetState(LEVEL_STATE _State)
{
	// 현재 상태와, 변경할 상태가 동일한 경우 무시
	if (m_State == _State)
		return;
	
	// Stop -> Pause 는 불가능하므로 무시
	if (m_State == LEVEL_STATE::STOP && _State == LEVEL_STATE::PAUSE)
		return;

	// Stop -> Play
	if (m_State == LEVEL_STATE::STOP && _State == LEVEL_STATE::PLAY)
	{
		m_State = _State;
		CLevelMgr::GetInst()->SetPlayStartLevel(CLevelMgr::GetInst()->GetCurrentLevel()->Clone());
		Begin();
	}
	// Play -> Stop
	// Pause -> Stop
	else if (_State == LEVEL_STATE::STOP)
	{
		m_State = _State;
		CTaskMgr::GetInst()->AddTask(TASK_TYPE::CHANGE_LEVEL, reinterpret_cast<DWORD_PTR>(CLevelMgr::GetInst()->GetPlayStartLevel()), NULL);
	}
	else
	// Play -> Pause
	// Pause -> Play
	{
		m_State = _State;
	}

}

void CLevel::Deregister()
{
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		m_Layer[i]->ClearVecObjects();
	}
}

void CLevel::AddGameObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	m_Layer[_LayerIdx]->AddGameObject(_Object, _bChildMove);
}
