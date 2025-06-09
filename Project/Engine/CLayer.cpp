#include "pch.h"
#include "CLayer.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"

#include "CGameObject.h"

CLayer::CLayer()
	: m_vecParentObjects{}
	, m_vecObjects{}
	, m_LayerIdx(-1)
{
}

CLayer::CLayer(const CLayer& _Other)
	: CEntity(_Other)
	, m_vecParentObjects{}
	, m_vecObjects{}
	, m_LayerIdx(_Other.m_LayerIdx)
{
	for (const auto& Object : _Other.m_vecParentObjects)
	{
		AddGameObject(Object->Clone(), false);
	}
}

CLayer::~CLayer()
{
	for (int i = 0; i < m_vecParentObjects.size(); ++i)
		CTaskMgr::GetInst()->AddTask(TASK_TYPE::DELETE_OBJECT, reinterpret_cast<DWORD_PTR>(m_vecParentObjects[i]), NULL);
	//Delete_Vec(m_vecParentObjects);
}

void CLayer::Begin()
{
	for (size_t i = 0; i < m_vecParentObjects.size(); ++i)
	{
		m_vecParentObjects[i]->Begin();
	}
}

void CLayer::End()
{
	for (size_t i = 0; i < m_vecParentObjects.size(); ++i)
	{
		m_vecParentObjects[i]->End();
	}
}

void CLayer::Tick()
{
	for (size_t i = 0; i < m_vecParentObjects.size(); ++i)
	{
		m_vecParentObjects[i]->Tick();
	}
}

void CLayer::Register()
{
	for (CGameObject* parent : m_vecParentObjects)
	{
		parent->RegisterToLevel();
	}
}

void CLayer::FinalTick()
{
	vector<CGameObject*>::iterator iter = m_vecParentObjects.begin();

	for (; iter != m_vecParentObjects.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
		{
			iter = m_vecParentObjects.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::AddGameObject(CGameObject* _Parent, bool _bChildMove)
{
	// ���޹��� ������Ʈ�� ���̾��� �θ� ������Ʈ �����̳ʿ� �����ϰ�, ������Ʈ�� ���̾� �ε��� ������ ���� ���̾� �ε����� �����Ѵ�.
	m_vecParentObjects.push_back(_Parent);
	_Parent->m_LayerIdx = m_LayerIdx;

	// �ڽ��� �������� ���, �ڽĵ� �ش� ���̾� �Ҽ����� �����Ѵ�.
	// ������Ʈ�� ������Ʈ�� �ڽĿ� ���� �۾� ó�� ť�� �����Ѵ�.
	static list<CGameObject*> queue;
	queue.clear();
	queue.push_back(_Parent);

	while (!queue.empty())
	{
		// ť���� ���� ���� ������Ʈ�� ����
		CGameObject* pObject = queue.front();
		queue.pop_front();

		// ������ ������Ʈ�� �ڽ��� �����ϴ� ���, ť�� �ڿ� �ڽ��� �߰�
		const vector<CGameObject*>& vecChild = pObject->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

		// �ֻ��� �θ� ������Ʈ 
		// or �ڽĿ�����Ʈ����, �ڽı��� ���� �̵��ϱ�� �� ��� 
		// or �ֻ����θ������Ʈx, �ڽ��̵�x ���� �Ҽ��� ���� ������Ʈ�� ���
		if (nullptr == pObject->GetParent() || _bChildMove || pObject->m_LayerIdx == -1)
		{
			pObject->m_LayerIdx = m_LayerIdx;
		}
	}

	CLevelMgr::GetInst()->SetLevelModified();
}

void CLayer::UnRegisterParentObject(CGameObject* _Parent)
{
	for (auto iter = m_vecParentObjects.begin(); iter != m_vecParentObjects.end(); ++iter)
	{
		if (*iter == _Parent)
		{
			m_vecParentObjects.erase(iter);
			CLevelMgr::GetInst()->SetLevelModified();
			break;
		}
	}
}

int CLayer::Save(fstream& _File)
{
	if (!_File.is_open())
		return E_FAIL;

	_File.write(reinterpret_cast<char*>(&m_LayerIdx), sizeof(int));

	int count = m_vecParentObjects.size();
	_File.write(reinterpret_cast<char*>(&count), sizeof(int));
	for (int i = 0; i < count; ++i)
	{
		m_vecParentObjects[i]->Save(_File);
	}

	return S_OK;
}

int CLayer::Load(fstream& _File)
{
	if (!_File.is_open())
		return E_FAIL;

	_File.read(reinterpret_cast<char*>(&m_LayerIdx), sizeof(int));

	int count = 0;
	_File.read(reinterpret_cast<char*>(&count), sizeof(int));
	m_vecParentObjects.resize(count);
	for (int i = 0; i < count; ++i)
	{
		m_vecParentObjects[i] = new CGameObject;
		m_vecParentObjects[i]->Load(_File);
	}

	return S_OK;
}
