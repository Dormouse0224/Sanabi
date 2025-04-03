#include "pch.h"
#include "CTaskMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

CTaskMgr::CTaskMgr()
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::Tick()
{
	for (size_t i = 0; i < m_vecGC.size(); ++i)
	{
		delete m_vecGC[i];
	}
	m_vecGC.clear();

	// =========
	// Task ó��
	// =========

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		const tTask& task = m_vecTask[i];

		switch (task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			CLevelMgr::GetInst()->SetLevelModified(true);
			CGameObject* pNewObj = (CGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;
			pLevel->AddGameObject(pNewObj, LayerIdx, true);

			// Object �� ������ ������ Level �� ���°� Play or Pause ��
			// Begin �Լ��� ȣ�����ش�.
			if (pLevel->GetState() != LEVEL_STATE::STOP)
			{
				pNewObj->Begin();
			}
		}
			break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CLevelMgr::GetInst()->SetLevelModified(true);
			CGameObject* pDelObj = (CGameObject*)task.Param0;

			if (!pDelObj->m_Dead)
			{
				pDelObj->m_Dead = true;
				m_vecGC.push_back(pDelObj);
			}
		}
			break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			CLevelMgr::GetInst()->SetLevelModified(true);
			CLevelMgr::GetInst()->ChangeLevel(reinterpret_cast<CLevel*>(task.Param0));
		}
			break;

		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			CLevelMgr::GetInst()->SetLevelModified(true);
			CLevelMgr::GetInst()->GetCurrentLevel()->SetState(static_cast<LEVEL_STATE>(task.Param0));
		}
		break;
		}
	}

	m_vecTask.clear();
}

void CTaskMgr::AddTask(TASK_TYPE _Type, DWORD_PTR _Param0, DWORD_PTR _Param1)
{
	tTask task = {};
	task.Type = _Type;
	task.Param0 = _Param0;
	task.Param1 = _Param1;
	m_vecTask.push_back(task);
}
