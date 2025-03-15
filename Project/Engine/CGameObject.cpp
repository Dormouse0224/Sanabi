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
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
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

	if (Type == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((CScript*)_Component);
	}
	else
	{
		// 이미 가지고 있는 컴포넌트인 경우
		assert(!m_Com[(UINT)Type]);
		m_Com[(UINT)Type] = _Component;

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
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
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
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
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

void CGameObject::FinalTick(bool _RigisterToLevel)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->FinalTick();
	}

	if (_RigisterToLevel)
	{
		// Layer 에 GameObject 등록하기
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLayer* pMyLayer = pCurLevel->GetLayer(m_LayerIdx);
		pMyLayer->RegisterGameObject(this);
	}

	for (vector<CGameObject*>::iterator iter = m_vecChild.begin(); iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick(_RigisterToLevel);

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

CGameObject* CGameObject::FindChild(wstring _Name)
{
	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->GetName() == _Name)
			return m_vecChild[i];
	}
	return nullptr;
}


#include "CTaskMgr.h"
void CGameObject::Destroy()
{
	CTaskMgr::GetInst()->AddTask(TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)this, (DWORD_PTR)nullptr);
}