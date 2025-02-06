#include "pch.h"
#include "CGameObject.h"
#include "components.h"
#include "CScript.h"

#include "CRenderComponent.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

CGameObject::CGameObject()
	: m_Com{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
{
}

CGameObject::~CGameObject()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			delete m_Com[i];
	}
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE Type = _Component->GetType();

	if (Type == SCRIPT)
	{
		m_vecScript.push_back((CScript*)_Component);
	}
	else
	{
		// 이미 가지고 있는 컴포넌트인 경우
		assert(!m_Com[Type]);
		m_Com[Type] = _Component;

		if (dynamic_cast<CRenderComponent*>(_Component))
		{
			// 하나의 GameObject 는 한종류의 RenderComonent 만 가질 수 있다.
			if (nullptr != m_RenderCom)
				assert(nullptr);

			m_RenderCom = (CRenderComponent*)_Component;
		}
	}

	_Component->m_Owner = this;
}

void CGameObject::Begin()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->Begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void CGameObject::Tick()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->Tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->FinalTick();
	}

	// Layer 에 GameObject 등록하기
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pMyLayer = pCurLevel->GetLayer(m_LayerIdx);
	pMyLayer->RegisterGameObject(this);

	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

void CGameObject::Render()
{
	if (!m_RenderCom)
		return;

	m_RenderCom->Render();
}


void CGameObject::AddChild(CGameObject* _Child)
{
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}


#include "CTaskMgr.h"
void CGameObject::Destroy()
{
	tTask task = {};

	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param0 = (DWORD_PTR)this;

	CTaskMgr::GetInst()->AddTask(task);
}