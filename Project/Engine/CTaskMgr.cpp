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
	// Task Ã³¸®
	// =========
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		const tTask& task = m_vecTask[i];

		switch (task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			CGameObject* pNew = (CGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;
			pLevel->AddGameObject(pNew, LayerIdx, false);
		}
			break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObj = (CGameObject*)task.Param0;

			if (!pObj->m_Dead)
			{
				pObj->m_Dead = true;
				m_vecGC.push_back(pObj);
			}
		}
			break;
		case TASK_TYPE::CHANGE_LEVEL:

			break;

		}
	}

	m_vecTask.clear();
}
